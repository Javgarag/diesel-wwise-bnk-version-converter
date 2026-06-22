#include "bus.h"

namespace Wwise {
	BusDuckInfo::BusDuckInfo(Reader& reader) {
		reader.Read(&bus_id);
		reader.Read(&duck_volume);
		reader.Read(&fade_out_time);
		reader.Read(&fade_in_time);
		reader.Read(&fade_curve);

		switch (VERSION) {
		case BankVersion::V2013:
			target_prop_id.emplace<AkPropID_2013>();
			reader.Read(&std::get<AkPropID_2013>(target_prop_id), sizeof(AkPropID_2013));
			break;
		case BankVersion::V2015:
			target_prop_id.emplace<AkPropID_2015>();
			reader.Read(&std::get<AkPropID_2015>(target_prop_id), sizeof(AkPropID_2015));
			break;
		case BankVersion::V2022:
			target_prop_id.emplace<AkPropID_2022>();
			reader.Read(&std::get<AkPropID_2022>(target_prop_id), sizeof(AkPropID_2022));
			break;
		}
	}

	void BusDuckInfo::Convert(Writer& writer) {
		writer << bus_id;
		writer << duck_volume;
		writer << fade_out_time;
		writer << fade_in_time;
		writer << fade_curve;
		
		switch (VERSION) {
		case BankVersion::V2013:
			if (CONVERT_VERSION == BankVersion::V2015) {
				writer << ConvertType(std::get<AkPropID_2013>(target_prop_id));
			}
			else { // 2013 -> 2022
				writer << ConvertType_2013_to_2022(std::get<AkPropID_2013>(target_prop_id));
			}
			break;
		case BankVersion::V2015:
			writer << ConvertType(std::get<AkPropID_2015>(target_prop_id));
			break;
		}
	}

	BusInitialFXParams::BusInitialFXParams(Reader& reader) {
		reader.Read(&count_fx);
		if (count_fx > 0) {
			bits_fx_bypass = reader.Read<uint8_t>();
			fx_chunks.emplace();
			for (uint32_t i = 0; i < count_fx; i++) {
				fx_chunks.value().push_back(FXChunkFXParams(reader));
			}
		}

		if (VERSION > BankVersion::V2013) {
			fx_id_0 = reader.Read<uint32_t>();
			is_shareset_0 = reader.Read<uint8_t>();
		}
	}

	void BusInitialFXParams::Convert(Writer& writer) {
		writer << count_fx;
		if (count_fx > 0) {
			writer << bits_fx_bypass.value();
			for (uint32_t i = 0; i < count_fx; i++) {
				fx_chunks.value()[i].Convert(writer);
			}
		}

		// +2013
		if (VERSION == BankVersion::V2015) {
			writer << fx_id_0.value();
			writer << is_shareset_0.value();
		}
		else { // 2013
			writer << (uint32_t)0;
			writer << (uint8_t)0;
		}
	}

	BusMetadataParams::BusMetadataParams(Reader& reader) {
		reader.Read(&num_fx);
		if (num_fx > 0) {
			bits_fx_bypass = reader.Read<uint8_t>();
			std::vector<FXChunkMetadata> fx_chunks;
			for (uint32_t i = 0; i < num_fx; i++) {
				fx_chunks.push_back(FXChunkMetadata(reader));
			}
		}
	}

	HIRCBus::HIRCBus(Reader& reader) 
		: item_base(reader)
	{
		reader.Read(&override_bus_id);
		if (VERSION == BankVersion::V2022 && override_bus_id == 0) {
			device_share_set = reader.Read<uint32_t>();
		}
		property_bundle = PropertyBundle(reader);

		if (VERSION == BankVersion::V2013) {
			positioning_enabled = reader.Read<uint8_t>();
			positioning_enable_panner = reader.Read<uint8_t>();
			kill_newest = reader.Read<uint8_t>();
			use_virtual_behavior = reader.Read<uint8_t>();

			reader.Read(&max_num_instances);

			is_max_num_instances_override_parent = reader.Read<uint8_t>();

			channel_config.emplace<ChannelFormat>();

			reader.PushCurrentPos();
			reader.Read(&std::get<ChannelFormat>(channel_config), sizeof(uint16_t));
			reader.PopLastPos();
			channel_config_raw = reader.Read<uint16_t>();

			unused1 = reader.Read<uint8_t>();
			unused2 = reader.Read<uint8_t>();
			is_hdr_bus = reader.Read<uint8_t>();
			hdr_release_mode_exponential = reader.Read<uint8_t>();
		}
		else {
			if (VERSION == BankVersion::V2022) {
				positioning_params = PositioningParams(reader);
				aux_params = AuxParams(reader);
			}

			flags.emplace();
			switch (VERSION) {
			case BankVersion::V2015:
				flags.value().emplace<BusFlags_2015>();
				break;
			case BankVersion::V2022:
				flags.value().emplace<BusFlags_2022>();
				break;
			}

			std::visit([&](auto& flags) {
				reader.Read(&flags, sizeof(uint8_t));
			}, flags.value());

			if (VERSION == BankVersion::V2015) {
				flags_2_2015.emplace();
				reader.Read(&(flags_2_2015.value()), sizeof(uint8_t));
			}

			reader.Read(&max_num_instances);

			channel_config.emplace<BusChannelConfig>();
			std::get<BusChannelConfig>(channel_config) = reader.Read<BusChannelConfig>(sizeof(uint32_t));
			hdr_flags = reader.Read<BusHDRFlags>(sizeof(uint8_t));
		}

		reader.Read(&recovery_time);
		reader.Read(&max_duck_volume);
		reader.Read(&num_ducks);
		for (uint32_t i = 0; i < num_ducks; i++) {
			ducks.push_back(BusDuckInfo(reader));
		}

		fx_params = BusInitialFXParams(reader);
		if (VERSION > BankVersion::V2013) {
			override_attachment_params = reader.Read<uint8_t>();
		}

		if (VERSION == BankVersion::V2022) {
			metadata_params = BusMetadataParams(reader);
		}

		initial_rtpc = InitialRTPC(reader);

		if (VERSION < BankVersion::V2022) {
			state_chunk.emplace<ParameterNodeStateChunkOld>();
			std::get<ParameterNodeStateChunkOld>(state_chunk) = ParameterNodeStateChunkOld(reader);
		}
		else {
			state_chunk.emplace<ParameterNodeStateChunkNew>();
			std::get<ParameterNodeStateChunkNew>(state_chunk) = ParameterNodeStateChunkNew(reader);
		}
	}

	void HIRCBus::Convert(Writer& writer) {
		item_base.Convert(writer);

		writer << override_bus_id;
		if (CONVERT_VERSION == BankVersion::V2022 && override_bus_id == 0) {
			writer << (uint32_t)0xE611314A; // FNV hash for "System"; this is the default Audio Device created when converting older soundbanks and the default value for this property (device_share_set).
		}
		property_bundle.Convert(writer);

		if (CONVERT_VERSION == BankVersion::V2022) {
			PositioningParams new_positioning{};
			new_positioning.is_2d_positioning_available = 0;
			new_positioning.is_3d_positioning_available = 0;
			if (VERSION == BankVersion::V2013) {
				BitPositioning2013& bits_positioning = new_positioning.bits_positioning.emplace<BitPositioning2013>();
				bits_positioning.positioning_info_override_parent = true; // always true, and the only one set
			}
			else { // 2015 -> 2022
				BitPositioning2015& bits_positioning = new_positioning.bits_positioning.emplace<BitPositioning2015>();
				bits_positioning.positioning_info_override_parent = true; // always true, and the only one set
			}
			
			new_positioning.Convert(writer);

			AuxParams new_aux{};
			new_aux.by_bit_vector = AuxParamsByBitVector{
				true, // override_game_aux_sends
				false,
				true, // override_user_aux_sends
				false,
				true // override_reflections_aux_bus
			};
			new_aux.Convert(writer);
		}
			
		uint8_t new_flags = 0;
		if (VERSION == BankVersion::V2013) {
			// for 2015 we do nothing. both values irrelevant and can be left on 0
			if (CONVERT_VERSION == BankVersion::V2022) {
				new_flags |= (kill_newest.value() << 0);
				new_flags |= (use_virtual_behavior.value() << 1);
				new_flags |= (is_max_num_instances_override_parent.value() << 2);
			}
		}
		else { // 2015 -> 2022
			new_flags |= (flags_2_2015.value().kill_newest << 0);
			new_flags |= (flags_2_2015.value().use_virtual_behavior << 1);
			new_flags |= ((int)(max_num_instances != 0) << 2);
			new_flags |= (std::get<BusFlags_2015>(flags.value()).is_background_music << 3);
		}
		writer << new_flags;

		if (CONVERT_VERSION == BankVersion::V2015) { // 2013 -> 2015
			uint8_t new_flags_2_2015 = 0;
			new_flags_2_2015 |= (kill_newest.value() << 0);
			new_flags_2_2015 |= (use_virtual_behavior.value() << 1);
			writer << new_flags_2_2015;
		}

		writer << max_num_instances;

		// https://www.audiokinetic.com/fr/public-library/2022.1.19_8584/?source=Help&id=understanding_channel_configurations
		if (VERSION == BankVersion::V2013) {
			uint32_t new_channel_config = 0; // Based on BusChannelConfig

			uint8_t num_channels = 0;
			for (int i = 0; i < 16; ++i) {
				num_channels += (channel_config_raw.value() >> i) & 1;
			}

			new_channel_config |= (num_channels << 0);
			new_channel_config |= ((num_channels > 0 ? 1 : 0) << 8);
			new_channel_config |= (channel_config_raw.value() << 12);

			writer << new_channel_config;
		}
		else { // 2015
			writer.Write(std::get<BusChannelConfig>(channel_config), sizeof(uint32_t));
		}
			
		if (VERSION == BankVersion::V2013) {
			BusHDRFlags new_hdr_flags{};

			new_hdr_flags.is_hdr_bus = is_hdr_bus.value();
			new_hdr_flags.hdr_release_mode_exponential = hdr_release_mode_exponential.value();

			writer.Write(new_hdr_flags, sizeof(uint8_t));
		}
		else { // 2015
			writer.Write(hdr_flags.value(), sizeof(uint8_t));
		}
		
		writer << recovery_time;
		writer << max_duck_volume;
		writer << num_ducks;
		for (uint32_t i = 0; i < num_ducks; i++) {
			ducks[i].Convert(writer);
		}

		fx_params.Convert(writer);
		writer << (uint8_t)0; // override_attachment_params; not used on RAID 2015 nor 2022; safe to assume 0 always.

		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint8_t)0; // num_fx for metadataParams
		}

		initial_rtpc.Convert(writer);
		std::get<ParameterNodeStateChunkOld>(state_chunk).Convert(writer);

		item_base.UpdateSize(writer);
	}
}