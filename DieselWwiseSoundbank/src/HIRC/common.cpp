#include "common.h"

namespace Wwise {
	HIRCItemGeneric::HIRCItemGeneric(Reader& reader) {
		reader.Read(&type);
		reader.Read(&size);
		data_start = reader.Tell();

		reader.Read(&item_id);
	}

	void HIRCItemGeneric::Convert(Writer& writer) {
		if (CONVERT_VERSION == BankVersion::V2022 && size != -1) {
			writer << ConvertType((HIRCItemTypeOld)type);
		}
		else {
			writer << type;
		}
		size_position = writer.Tell();
		writer << (uint32_t)0; // temp size, should get updated later
		writer << item_id;
	}

	void HIRCItemGeneric::UpdateSize(Writer& writer) {
		size_t final_size = writer.Tell() - (size_position + sizeof(uint32_t));

		writer.PushCurrentPos();
		writer.Seek(size_position);
		writer << (uint32_t)final_size;
		writer.PopLastPos();
	}

	HIRCUnknown::HIRCUnknown(Reader& reader)
		: item_base(reader)
	{
		// skip everything
		reader.Seek(item_base.data_start + item_base.size);
	}

	// RTPC

	RTPCGraphPoint::RTPCGraphPoint(Reader& reader) {
		reader.Read(&gp_from);
		reader.Read(&gp_to);
		reader.Read(&interpolation);
	}

	void RTPCGraphPoint::Convert(Writer& writer) {
		writer << gp_from;
		writer << gp_to;
		writer << interpolation;
	}

	RTPCGraph::RTPCGraph(Reader& reader) {
		reader.Read(&num_points);
		for (uint16_t i = 0; i < num_points; i++) {
			points.push_back(RTPCGraphPoint(reader));
		}
	}

	void RTPCGraph::Convert(Writer& writer) {
		writer << num_points;
		for (uint16_t i = 0; i < num_points; i++) {
			points[i].Convert(writer);
		}
	}

	RTPC::RTPC(Reader& reader) {
		reader.Read(&id);
		if (VERSION > BankVersion::V2013) {
			if (VERSION == BankVersion::V2015) {
				reader.Read(&rtpc_type.emplace<RTPCType2015>());;
				reader.Read(&rtpc_accum.emplace<RTPCAccum2015>());;
			}
			else {
				reader.Read(&rtpc_type.emplace<RTPCType2022>());;
				reader.Read(&rtpc_accum.emplace<RTPCAccum2022>());;
			}
		}

		if (VERSION == BankVersion::V2013) {
			reader.Read(&param_id.emplace<uint32_t>());;
		}
		else {
			reader.Read(&param_id.emplace<uint8_t>());;
		}

		reader.Read(&rtpc_curve_id);
		reader.Read(&scaling);
		rtpc_graph = RTPCGraph(reader);
	}

	void RTPC::Convert(Writer& writer) {
		writer << id;

		if (CONVERT_VERSION > BankVersion::V2013) {
			if (VERSION == BankVersion::V2013) {
				writer << (uint8_t)0; // RTPCType; GameParameter. None of the Diesel games use MIDI and Modulators were introduced in 2014.
				writer << (uint8_t)0; // RTPCAccum; try with additive for now for conversions from 2013.

				if (CONVERT_VERSION == BankVersion::V2015) {
					writer << ConvertType((RTPCParameterID_2013)std::get<uint32_t>(param_id));
				}
				else { // 2022
					writer << ConvertType_2013_to_2022((RTPCParameterID_2013)std::get<uint32_t>(param_id));
				}
			}
			else { // 2015 -> 2022
				writer << ConvertType(std::get<RTPCType2015>(rtpc_type));
				writer << ConvertType(std::get<RTPCAccum2015>(rtpc_accum));
				writer << ConvertType((RTPCParameterID_2015)std::get<uint8_t>(param_id));
			}
		}

		writer << rtpc_curve_id;
		writer << scaling;
		rtpc_graph.Convert(writer);
	}

	InitialRTPC::InitialRTPC(Reader& reader) {
		reader.Read(&num_rtpc);
		for (uint16_t i = 0; i < num_rtpc; i++) {
			rtpcs.push_back(RTPC(reader));
		}
	}

	void InitialRTPC::Convert(Writer& writer) {
		writer << num_rtpc;
		for (uint16_t i = 0; i < num_rtpc; i++) {
			rtpcs[i].Convert(writer);
		}
	}

	// End RTPC

	// Sound

	Plugin::Plugin(Reader& reader) {
		reader.Read(&id);

		type = (PluginType)((uint32_t)id & 0x0000000F);
		company = (PluginCompany)(((uint32_t)id >> 1) & 0x00000FFF);
	}

	void Plugin::Convert(Writer& writer) {
		writer << id;
	}

	SourceData::SourceData(Reader& reader)
		: plugin(reader)
	{
		if (VERSION == BankVersion::V2013) {
			source_type = reader.Read<SourceTypeOld>();
		}
		else {
			source_type = reader.Read<SourceTypeNew>();
		}
		reader.Read(&source_id);

		if (VERSION == BankVersion::V2013) {
			reader.Read(&file_id); // removed in 2015+
			if (std::get<SourceTypeOld>(source_type) != SourceTypeOld::Streaming) {
				reader.Read(&file_offset);
				reader.Read(&in_memory_media_size);
			}
		}
		else {
			reader.Read(&in_memory_media_size);
		}

		if (VERSION == BankVersion::V2013) {
			source_bits = reader.Read<SourceBitsOld>();
		}
		else {
			source_bits = reader.Read<SourceBitsNew>();
		}

		if (plugin.type == PluginType::Source || plugin.type == PluginType::MotionSource) {
			reader.Read(&plugin_params_size);

			// temp. only a couple of plugins change between versions tho
			plugin_param_data.resize(plugin_params_size);
			reader.Read(plugin_param_data.data(), plugin_params_size);
		}
		else {
			plugin_params_size = 0;
		}
	}

	void SourceData::Convert(Writer& writer) {
		plugin.Convert(writer);

		// Force-convert zero latency sounds to normal streaming (old IMA ADPCM prefetch segments don't play correctly on modern Wwise)
		switch (VERSION) {
		case BankVersion::V2013:
			if (std::get<SourceTypeOld>(source_type) == SourceTypeOld::PrefetchStreaming) {
				writer << SourceTypeNew::Streaming;
			}
			else {
				writer << ConvertType(std::get<SourceTypeOld>(source_type));
			}
			break;
		case BankVersion::V2015:
			if (std::get<SourceTypeNew>(source_type) == SourceTypeNew::PrefetchStreaming) {
				writer << SourceTypeNew::Streaming;
			}
			else {
				writer << std::get<SourceTypeNew>(source_type);
			}
		}

		writer << source_id;
		writer << in_memory_media_size; // Write old value if it exists, otherwise write 0; doesn't seem to matter much for memory usage nor sound playback

		if (VERSION == BankVersion::V2013) {
			uint8_t bit_flags = 0;
			bit_flags |= (std::get<SourceBitsOld>(source_bits).is_language_specific << 0);  // language_specific

			// prefetch (assumed)
			if (std::get<SourceTypeOld>(source_type) == SourceTypeOld::PrefetchStreaming) {
				bit_flags |= (1 << 1);
			}
			else {
				bit_flags |= (0 << 1);
			}

			// non_cachable defaults to 0

			bit_flags |= (std::get<SourceBitsOld>(source_bits).has_source << 7); // has_source

			writer << bit_flags;
		}
		else {
			writer.Write(std::get<SourceBitsNew>(source_bits), sizeof(uint8_t));
		}

		if (plugin.type == PluginType::Source || plugin.type == PluginType::MotionSource) {
			writer << plugin_params_size;
			writer.Write(*plugin_param_data.data(), plugin_param_data.size());
		}
	}

	// End Sound

	// Start Base-Parameters

	FXChunkFXParams::FXChunkFXParams(Reader& reader) {
		reader.Read(&fx_index);
		reader.Read(&fx_id);
		reader.Read(&is_share_set);
		reader.Read(&is_rendered);
	}

	void FXChunkFXParams::Convert(Writer& writer) {
		writer << fx_index;
		writer << fx_id;
		writer << is_share_set;
		writer << is_rendered;
	}

	ParameterNodeFXParams::ParameterNodeFXParams(Reader& reader) {
		reader.Read(&is_override_parent_fx);
		reader.Read(&num_fx);
		if (num_fx > 0) {
			bits_fx_bypass = reader.Read<uint8_t>();
			fx_chunks.emplace();
			for (uint32_t i = 0; i < num_fx; i++) {
				fx_chunks.value().push_back(FXChunkFXParams(reader));
			}
		}
	}

	void ParameterNodeFXParams::Convert(Writer& writer) {
		writer << is_override_parent_fx;
		writer << num_fx;
		if (num_fx > 0) {
			writer << bits_fx_bypass.value();
			for (uint32_t i = 0; i < num_fx; i++) {
				fx_chunks.value()[i].Convert(writer);
			}
		}
	}

	// repeat it (too lazy to come up with something better)

	FXChunkMetadata::FXChunkMetadata(Reader& reader) {
		reader.Read(&fx_index);
		reader.Read(&fx_id);
		reader.Read(&is_share_set);
	}

	ParameterNodeMetadataParams::ParameterNodeMetadataParams(Reader& reader) {
		reader.Read(&is_override_parent_fx);
		reader.Read(&num_fx);
		if (num_fx > 0) {
			bits_fx_bypass = reader.Read<uint8_t>();
			std::vector<FXChunkMetadata> fx_chunks;
			for (uint32_t i = 0; i < num_fx; i++) {
				fx_chunks.push_back(FXChunkMetadata(reader));
			}
		}
	}

	PropertyBundle::PropertyBundle(Reader& reader) {
		reader.Read(&num_properties);

		switch (VERSION) {
		case BankVersion::V2013:
			property_ids.emplace<std::vector<AkPropID_2013>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2013>>(property_ids).push_back(reader.Read<AkPropID_2013>());
			}
			break;
		case BankVersion::V2015:
			property_ids.emplace<std::vector<AkPropID_2015>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2015>>(property_ids).push_back(reader.Read<AkPropID_2015>());
			}
			break;
		case BankVersion::V2022:
			property_ids.emplace<std::vector<AkPropID_2022>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2022>>(property_ids).push_back(reader.Read<AkPropID_2022>());
			}
			break;
		}
		
		for (uint8_t i = 0; i < num_properties; i++) {
			property_values.push_back(reader.Read<float>());
		}
	}

	void PropertyBundle::AddNewProperty(int propId, float propValue) {
		std::visit(overload{ 
			[&](std::vector<AkPropID_2013>& property_ids) {
				property_ids.push_back((AkPropID_2013)propId);
			},
			[&](std::vector<AkPropID_2015>& property_ids) {
				property_ids.push_back((AkPropID_2015)propId);
			},
			[&](std::vector<AkPropID_2022>& property_ids) {
				property_ids.push_back((AkPropID_2022)propId);
			}
		}, property_ids);
		num_properties++;

		property_values.push_back(propValue);
	}

	void PropertyBundle::Convert(Writer& writer) {
		writer << num_properties;

		std::visit([&](auto& property_ids) {
			if (VERSION == BankVersion::V2013) {
				if (CONVERT_VERSION == BankVersion::V2015) {
					for (uint8_t i = 0; i < num_properties; i++) {
						writer << ConvertType((AkPropID_2013)property_ids[i]);
					}
				}
				else { // 2013 -> 2022
					for (uint8_t i = 0; i < num_properties; i++) {
						writer << ConvertType_2013_to_2022((AkPropID_2013)property_ids[i]);
					}
				}
			}
			else { // converting from 2015
				for (uint8_t i = 0; i < num_properties; i++) {
					writer << ConvertType((AkPropID_2015)property_ids[i]);
				}
			}
		}, property_ids);

		for (uint8_t i = 0; i < num_properties; i++) {
			writer << property_values[i];
		}
	}

	RangedProperty::RangedProperty(Reader& reader) {
		reader.Read(&min);
		reader.Read(&max);
	}

	void RangedProperty::Convert(Writer& writer) {
		writer << min;
		writer << max;
	}

	RangedModifierPropertyBundle::RangedModifierPropertyBundle(Reader& reader) {
		reader.Read(&num_properties);

		switch (VERSION) {
		case BankVersion::V2013:
			property_ids.emplace<std::vector<AkPropID_2013>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2013>>(property_ids).push_back(reader.Read<AkPropID_2013>());
			}
			break;
		case BankVersion::V2015:
			property_ids.emplace<std::vector<AkPropID_2015>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2015>>(property_ids).push_back(reader.Read<AkPropID_2015>());
			}
			break;
		case BankVersion::V2022:
			property_ids.emplace<std::vector<AkPropID_2022>>();
			for (uint8_t i = 0; i < num_properties; i++) {
				std::get<std::vector<AkPropID_2022>>(property_ids).push_back(reader.Read<AkPropID_2022>());
			}
			break;
		}

		for (uint8_t i = 0; i < num_properties; i++) {
			property_values.push_back(RangedProperty(reader));
		}
	}

	void RangedModifierPropertyBundle::Convert(Writer& writer) {
		writer << num_properties;

		std::visit([&](auto& property_ids) {
			for (uint8_t i = 0; i < num_properties; i++) {
				writer << property_ids[i];
			}
		}, property_ids);

		for (uint8_t i = 0; i < num_properties; i++) {
			property_values[i].Convert(writer);
		}
	}

	ParameterNodeInitialParams::ParameterNodeInitialParams(Reader& reader)
		: property_bundle(reader),
		ranged_modifier_property_bundle(reader)
	{}

	void ParameterNodeInitialParams::Convert(Writer& writer) {
		property_bundle.Convert(writer);
		ranged_modifier_property_bundle.Convert(writer);
	}

	PathVertex::PathVertex(Reader& reader) {
		reader.Read(&x);
		reader.Read(&y);
		reader.Read(&z);
		reader.Read(&duration);
	}

	void PathVertex::Convert(Writer& writer) {
		writer << x;
		writer << y;
		writer << z;
		writer << duration;
	}

	PathListItemOffset::PathListItemOffset(Reader& reader) {
		reader.Read(&vertices_offset);
		reader.Read(&num_vertices);
	}

	void PathListItemOffset::Convert(Writer& writer) {
		writer << vertices_offset;
		writer << num_vertices;
	}

	AutomationParam3D::AutomationParam3D(Reader& reader) {
		reader.Read(&fXRange);
		reader.Read(&fYRange);

		if (VERSION > BankVersion::V2013) {
			reader.Read(&fZRange);
		}
	}

	void AutomationParam3D::Convert(Writer& writer) {
		writer << fXRange;
		writer << fYRange;
		writer << fZRange;
	}

	PositioningParams::PositioningParams(Reader& reader) {
		auto parse_automation = [&]() {
			num_vertices = reader.Read<uint32_t>();

			for (uint32_t i = 0; i < num_vertices; i++) {
				vertices.push_back(PathVertex(reader));
			}

			num_playlist_items = reader.Read<uint32_t>();
			for (uint32_t i = 0; i < num_playlist_items; i++) {
				playlist_items.push_back(PathListItemOffset(reader));
			}

			for (uint32_t i = 0; i < num_playlist_items; i++) {
				automation_params_3d.push_back(AutomationParam3D(reader));
			}
		};

		switch (VERSION) {
		case BankVersion::V2013:
			reader.Read(&bits_positioning.emplace<BitPositioning2013>(), sizeof(uint8_t));

			if (std::get<BitPositioning2013>(bits_positioning).positioning_info_override_parent != 1) {
				// only continue parsing if positioning params override parent...
				break;
			}

			is_2d_positioning_available = reader.Read<uint8_t>();
			is_3d_positioning_available = reader.Read<uint8_t>();

			if (is_2d_positioning_available == 1) {
				positioning_enable_panner = reader.Read<uint8_t>();
			}
			if (is_3d_positioning_available == 1) {
				type = reader.Read<PositioningType>();
				attenuation_id = reader.Read<uint32_t>();
				is_spatialized = reader.Read<uint8_t>();

				if (type == PositioningType::Positioning2D) {
					is_dynamic = reader.Read<uint8_t>();
				}
				else { // has_automation
					// modern listener with automation
					reader.Read(&path_mode.emplace().emplace<uint32_t>(), sizeof(uint32_t));
					is_looping = reader.Read<uint8_t>();
					transition_time = reader.Read<uint32_t>();
					follow_orientation = reader.Read<uint8_t>();

					parse_automation();
				}
			}

			break;
		case BankVersion::V2015:
			reader.Read(&bits_positioning.emplace<BitPositioning2015>(), sizeof(uint8_t));

			if (std::get<BitPositioning2015>(bits_positioning).positioning_info_override_parent != 1) {
				break;
			}

			if (std::get<BitPositioning2015>(bits_positioning).is_3d_positioning_available != 1) {
				// 2015+ holds 2d information in bits_positioning; rest of parsing is 3d
				break;
			}

			reader.Read(&bits_3d.emplace().emplace<Bits3D2015>(), sizeof(uint8_t));
			attenuation_id = reader.Read<uint32_t>();

			if (std::get<Bits3D2015>(bits_3d.value()).spatialization_mode != SpatializationMode::PositionOnly) {
				reader.Read(&path_mode.emplace().emplace<uint8_t>(), sizeof(uint8_t));
				transition_time = reader.Read<int32_t>();
				parse_automation();
			}

			break;
		case BankVersion::V2022:
			reader.Read(&bits_positioning.emplace<BitPositioning2022>(), sizeof(uint8_t));

			if (std::get<BitPositioning2022>(bits_positioning).positioning_info_override_parent != 1) {
				break;
			}

			if (std::get<BitPositioning2022>(bits_positioning).has_listener_relative_routing != 1) {
				// 2015+ holds 2d information in bits_positioning; rest of parsing is 3d
				break;
			}

			reader.Read(&bits_3d.emplace().emplace<Bits3D2022>(), sizeof(uint8_t));

			if (std::get<BitPositioning2022>(bits_positioning).position_3d_type != ThreeDPositionType::Emitter) {
				reader.Read(&path_mode.emplace().emplace<uint8_t>(), sizeof(uint8_t));
				transition_time = reader.Read<int32_t>();
				parse_automation();
			}

			break;
		}
	}

	void PositioningParams::Convert(Writer& writer) {
		uint8_t bits_positioning_new = 0;
		std::visit([&](auto& v) {
			bits_positioning_new |= (v.positioning_info_override_parent << 0);
			}, bits_positioning);

		if (!(bits_positioning_new & 1)) {
			writer << bits_positioning_new;
			return; // no parent override
		}

		if (CONVERT_VERSION == BankVersion::V2015) {
			// leave the unknown fields blank. they seem to go mostly unused anyways
			bits_positioning_new |= (std::get<BitPositioning2013>(bits_positioning).has_listener_relative_routing << 3); // is_3d_positioning_available 

			writer << bits_positioning_new;
			if (!(bits_positioning_new & (1 << 3))) { // no 3d
				return;
			}
		}
		else { // 2022
			// has_listener_relative_routing
			if (VERSION == BankVersion::V2013) {
				if (is_3d_positioning_available.value() == 1 || is_2d_positioning_available.value() == 1) {
					bits_positioning_new |= (1 << 1);
				}
			}
			else {
				if (std::get<BitPositioning2015>(bits_positioning).is_3d_positioning_available == 1 || std::get<BitPositioning2015>(bits_positioning).is_2d_positioning_available == 1) {
					bits_positioning_new |= (1 << 1);
				}	
			}
			bits_positioning_new |= ((int)SpeakerPanningType::DirectSpeakerAssignment << 2); // panner_type

			// position_3d_type 
			switch (VERSION) {
			case BankVersion::V2013:
				if (type.has_value() && type.value() != PositioningType::Positioning2D) {
					bits_positioning_new |= ((int)ThreeDPositionType::EmitterWithAutomation << 5);
				}
				else if (follow_orientation.has_value() && follow_orientation.value() == 1) {
					bits_positioning_new |= ((int)ThreeDPositionType::ListenerWithAutomation << 5);
				}
				else {
					bits_positioning_new |= ((int)ThreeDPositionType::Emitter << 5);
				}
				break;
			case BankVersion::V2015:
				if (bits_3d.has_value() && std::get<Bits3D2015>(bits_3d.value()).spatialization_mode != SpatializationMode::PositionOnly) {
					bits_positioning_new |= ((int)ThreeDPositionType::EmitterWithAutomation << 5);
				}
				else {
					bits_positioning_new |= ((int)ThreeDPositionType::Emitter << 5);
				}
				break;
			}

			writer << bits_positioning_new;
			if (!(bits_positioning_new & (1 << 1))) { // no 3d
				return;
			}
		}

		uint8_t bits_3d_new = 0;

		// EXCLUSIVELY 3D parameters
		if ((VERSION == BankVersion::V2013 && is_3d_positioning_available == 1) 
			|| (VERSION == BankVersion::V2015 && std::get<BitPositioning2015>(bits_positioning).is_3d_positioning_available == 1)) {
			if (VERSION == BankVersion::V2013) { 
				bits_3d_new |= ((int)SpatializationMode::PositionAndOrientation << 0); // always the case if 3D on 2013
			}
			else { // 2015 to 2022
				bits_3d_new |= ((int)std::get<Bits3D2015>(bits_3d.value()).spatialization_mode << 0);
			}

			// hold emitter pos and orientation will always be 0 when converting from 2013/2015 (introduced Wwise 2018.1)
			// https://www.audiokinetic.com/fr/community/blog/out-with-the-old-in-with-the-new-positioning-revamped-in-wwise-2018.1/

			// hold listener orient
			if (VERSION == BankVersion::V2013 && CONVERT_VERSION == BankVersion::V2015 && type.value() != PositioningType::Positioning2D) { // has listener automation
				bits_3d_new |= (follow_orientation.value() << 4);
			}
			else if (VERSION == BankVersion::V2013 && CONVERT_VERSION == BankVersion::V2022 && type.value() != PositioningType::Positioning2D) { // has listener automation
				bits_3d_new |= (follow_orientation.value() << 5);
			}
			else if (VERSION != BankVersion::V2013) { // 2015 to 2022
				bits_3d_new |= ((int)std::get<Bits3D2015>(bits_3d.value()).hold_listener_orient << 5);
			}
		}
		// End exclusively 3D parameters

		// enable_attenuation 
		// Both 3D and old 2D stuff get assigned an attenuation in 2022 (in the latter case it'll be an empty one since older versions had no toggle for 2D attenuation)
		// if set, the old inline attenuation ID gets moved to a property in the PropertyBundle in 2022 (see BaseParams::Convert())
		if (CONVERT_VERSION == BankVersion::V2022 && (attenuation_id.has_value() 
			|| (VERSION == BankVersion::V2015 && std::get<BitPositioning2015>(bits_positioning).is_2d_positioning_available == 1)
			|| (VERSION == BankVersion::V2013 && is_2d_positioning_available.value() == 1))) {
			bits_3d_new |= (1 << 3);
		}

		// as for enable_diffraction in 2022, default value is false so we do nothing (introduced 2017+)

		writer << bits_3d_new;
		
		if (CONVERT_VERSION == BankVersion::V2015) {
			writer << attenuation_id.value();
		}

		auto write_automation = [&]() {
			if (VERSION == BankVersion::V2013) {
				writer << (uint8_t)(std::get<uint32_t>(path_mode.value()));
			}
			else {
				writer << (std::get<uint8_t>(path_mode.value()));
			}

			writer << (int32_t)transition_time.value();

			writer << num_vertices.value();
			for (uint32_t i = 0; i < num_vertices; i++) {
				vertices[i].Convert(writer);
			}

			writer << num_playlist_items.value();
			for (uint32_t i = 0; i < num_playlist_items; i++) {
				playlist_items[i].Convert(writer);
			}

			for (uint32_t i = 0; i < num_playlist_items; i++) {
				automation_params_3d[i].Convert(writer);
			}
		};

		// automation
		switch (CONVERT_VERSION) {
		case BankVersion::V2015:
			if ((bits_3d_new & 1) != (int)SpatializationMode::PositionOnly) {
				write_automation();
			}
			break;
		case BankVersion::V2022:
			if (((bits_positioning_new >> 5) & 1) != (int)ThreeDPositionType::Emitter) {
				write_automation();
			}
			break;
		}
	}

	AuxParams::AuxParams(Reader& reader) {
		if (VERSION == BankVersion::V2013) {
			reader.Read(&override_game_aux_sends);
			reader.Read(&use_game_aux_sends);
			reader.Read(&override_user_aux_sends);
			reader.Read(&has_aux);

			if (has_aux) {
				reader.Read(aux_id.data(), sizeof(uint32_t) * 4);
			}
		}
		else {
			by_bit_vector = reader.Read<AuxParamsByBitVector>(sizeof(uint8_t));
			if (by_bit_vector.value().has_aux) {
				reader.Read(aux_id.data(), sizeof(uint32_t) * 4);
			}

			if (VERSION == BankVersion::V2022) {
				reflections_aux_bus = reader.Read<uint32_t>();
			}
		}
	}

	void AuxParams::Convert(Writer& writer) {
		if (VERSION == BankVersion::V2013 && !(by_bit_vector.has_value())) { // if by_bit_vector has value, this is coming from a Bus conversion
			uint8_t bit_flags = 0;
			bit_flags |= (override_game_aux_sends << 0);
			bit_flags |= (use_game_aux_sends << 1);
			bit_flags |= (override_user_aux_sends << 2);
			bit_flags |= (has_aux << 3);

			writer << bit_flags;

			if (has_aux == 1) {
				writer.Write(*aux_id.data(), sizeof(uint32_t) * 4);
			}
		}
		else {
			writer.Write(by_bit_vector, sizeof(uint8_t));

			if (by_bit_vector.value().has_aux) {
				writer.Write(*aux_id.data(), sizeof(uint32_t) * 4);
			}
		}

		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint32_t)0; // Reflect plugin released in 2017; old stuff has this blank (see Raid)
		}
	}

	AdvSettingsParams::AdvSettingsParams(Reader& reader) {
		if (VERSION == BankVersion::V2013) {
			reader.Read(&virtual_queue_behavior);
			reader.Read(&kill_newest); 
			reader.Read(&use_virtual_behavior); 
			reader.Read(&max_num_instance); 
			reader.Read(&is_global_limit); 
			reader.Read(&below_threshold_behavior); 
			reader.Read(&is_max_num_inst_override_parent);
			reader.Read(&is_vvoices_opt_override_parent); 
			reader.Read(&override_hdr_envelope); 
			reader.Read(&override_analysis); 
			reader.Read(&normalize_loudness); 
			reader.Read(&enable_envelope); 
		}
		else {
			reader.Read(&by_bit_vector_1, sizeof(uint8_t));

			reader.Read(&virtual_queue_behavior);
			reader.Read(&max_num_instance);
			reader.Read(&below_threshold_behavior);

			reader.Read(&by_bit_vector_2, sizeof(uint8_t));
		}
	}

	void AdvSettingsParams::Convert(Writer& writer) {
		if (VERSION == BankVersion::V2013) {
			uint8_t bit_flags_1 = 0;
			bit_flags_1 |= (kill_newest << 0);
			bit_flags_1 |= (use_virtual_behavior << 1);
			bit_flags_1 |= (is_max_num_inst_override_parent << 3);
			bit_flags_1 |= (is_vvoices_opt_override_parent << 4);

			writer << bit_flags_1;
		}
		else { // convert from 2015
			writer << by_bit_vector_1;
		}

		writer << virtual_queue_behavior;
		writer << max_num_instance;
		writer << below_threshold_behavior;

		if (VERSION == BankVersion::V2013) {
			uint8_t bit_flags_2 = 0;
			bit_flags_2 |= (override_hdr_envelope << 0);
			bit_flags_2 |= (override_analysis << 1);
			bit_flags_2 |= (normalize_loudness << 2);
			bit_flags_2 |= (enable_envelope << 3);

			writer << bit_flags_2;
		}
		else { // convert from 2015
			writer << by_bit_vector_2;
		}
	}

	State::State(Reader& reader) {
		reader.Read(&state_id);
		reader.Read(&state_instance_id);
	}

	void State::Convert(Writer& writer) {
		writer << state_id;
		writer << state_instance_id;
	}

	StateGroup::StateGroup(Reader& reader) {
		reader.Read(&state_group_id);
		reader.Read(&state_sync_type);
		if (VERSION == BankVersion::V2022) {
			auto& v = num_states.emplace<uint8_t>();
			reader.Read(&v);;
		}
		else {
			auto& v = num_states.emplace<uint16_t>();
			reader.Read(&v);;
		}

		std::visit([&](auto& num_states) {
			for (unsigned int i = 0; i < num_states; i++) {
				states.push_back(State(reader));
			}
		}, num_states);
	}

	void StateGroup::Convert(Writer& writer) {
		writer << state_group_id;
		writer << state_sync_type;
		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint8_t)std::get<uint16_t>(num_states);
		}
		else {
			writer << std::get<uint16_t>(num_states);
		}
		
		std::visit([&](auto& num_states) {
			for (unsigned int i = 0; i < num_states; i++) {
				states[i].Convert(writer);
			}
		}, num_states);
	}

	// 2022
	StatePropertyNew::StatePropertyNew(Reader& reader) {
		reader.Read(&property_id);
		reader.Read(&accum_type);
		reader.Read(&in_Db);
	}

	ParameterNodeStateChunkOld::ParameterNodeStateChunkOld(Reader& reader) {
		reader.Read(&num_state_groups);
		for (uint32_t i = 0; i < num_state_groups; i++) {
			groups.push_back(StateGroup(reader));
		}
	}

	void ParameterNodeStateChunkOld::Convert(Writer& writer) {
		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint8_t)0; // 2022 num_state_props
			writer << (uint8_t)num_state_groups; // cast to new
		}
		else {
			writer << num_state_groups;
		}
		for (uint32_t i = 0; i < num_state_groups; i++) {
			groups[i].Convert(writer);
		}
	}

	// 2022
	ParameterNodeStateChunkNew::ParameterNodeStateChunkNew(Reader& reader) {
		reader.Read(&num_state_props);
		for (uint32_t i = 0; i < num_state_props; i++) {
			state_props.push_back(StatePropertyNew(reader));
		}

		reader.Read(&num_state_groups);
		for (uint32_t i = 0; i < num_state_groups; i++) {
			state_groups.push_back(StateGroup(reader));
		}
	}

	BaseParams::BaseParams(Reader& reader)
		: initial_fx_params(reader)
	{
		if (VERSION == BankVersion::V2022) {
			metadata_params = ParameterNodeMetadataParams(reader);
		}
		if (VERSION > BankVersion::V2013) {
			override_attachment_params = reader.Read<uint8_t>();
		}

		reader.Read(&override_bus_id);
		reader.Read(&direct_parent_id);

		if (VERSION == BankVersion::V2013) {
			priority_override_parent = reader.Read<uint8_t>();
			priority_apply_dist_factor = reader.Read<uint8_t>();
		}
		else {
			by_bit_vector = reader.Read<ParameterNodeByBitVector>(sizeof(uint8_t));
		}

		parameter_node_initial_params = ParameterNodeInitialParams(reader);
		positioning_params = PositioningParams(reader);
		auxiliary_params = AuxParams(reader);
		adv_settings_params = AdvSettingsParams(reader);

		if (VERSION < BankVersion::V2022) {
			state_chunk = ParameterNodeStateChunkOld(reader);
		}
		else {
			state_chunk = ParameterNodeStateChunkNew(reader);
		}

		initial_rtpc = InitialRTPC(reader);

		if (VERSION < BankVersion::V2022 && FEEDBACK_IN_BANK) {
			feedback_bus_id = reader.Read<uint32_t>();
		}
	}

	void BaseParams::Convert(Writer& writer) {

		// before we start check for additional properties added to PropertyBundle
		if (positioning_params.attenuation_id.has_value() && CONVERT_VERSION == BankVersion::V2022) {
			parameter_node_initial_params.property_bundle.AddNewProperty((int)AkPropID_2022::AttenuationID, *reinterpret_cast<float*>(&positioning_params.attenuation_id.value()));
		}

		initial_fx_params.Convert(writer);

		if (CONVERT_VERSION == BankVersion::V2022) {
			// not only is the metadata tab hidden by defualt on Wwise; none of Raid's updated soundbanks seem to use any of its parameters, safe to assume 2013 Payday Wwise doesn't as well
			writer << (uint8_t)0; // is_override_parent_metadata
			writer << (uint8_t)0; // num_fx
		}
		if (CONVERT_VERSION > BankVersion::V2013) {
			if (override_attachment_params.has_value()) {
				writer << override_attachment_params.value();
			}
			else {
				writer << (uint8_t)0; // default
			}
		}

		writer << override_bus_id;
		writer << direct_parent_id;

		if (VERSION == BankVersion::V2013) {
			uint8_t bit_flags = 0;
			bit_flags |= (priority_override_parent.value() << 0);
			bit_flags |= (priority_apply_dist_factor.value() << 1);
			// rest of flags are midi-related (introduced 2014)
			writer << bit_flags;
		}
		else {
			writer.Write(by_bit_vector, sizeof(uint8_t));
		}
		
		parameter_node_initial_params.Convert(writer);
		positioning_params.Convert(writer);
		auxiliary_params.Convert(writer);
		adv_settings_params.Convert(writer);

		std::get<ParameterNodeStateChunkOld>(state_chunk).Convert(writer);

		initial_rtpc.Convert(writer);
		if (CONVERT_VERSION == BankVersion::V2015 && FEEDBACK_IN_BANK) {
			writer << feedback_bus_id.value();
		}
	}

	// End Base-Parameters

	// Other Shared

	Children::Children(Reader& reader) {
		reader.Read(&num_children);
		for (uint32_t i = 0; i < num_children; i++) {
			child_ids.push_back(reader.Read<uint32_t>());
		}
	}

	void Children::Convert(Writer& writer) {
		writer << num_children;
		for (uint32_t i = 0; i < num_children; i++) {
			writer << child_ids[i];
		}
	}

	MusicStinger::MusicStinger(Reader& reader) {
		reader.Read(&trigger_id);
		reader.Read(&segment_id);
		reader.Read(&sync_play_at);
		reader.Read(&cue_filter_hash);
		reader.Read(&dont_repeat_time);
		reader.Read(&num_segment_look_ahead);
	}

	void MusicStinger::Convert(Writer& writer) {
		writer << trigger_id;
		writer << segment_id;
		writer << sync_play_at;
		writer << cue_filter_hash;
		writer << dont_repeat_time;
		writer << num_segment_look_ahead;
	}

	MusicNodeParams::MusicNodeParams(Reader& reader) {
		if (VERSION > BankVersion::V2013) {
			flags = reader.Read<MusicTrackFlags>(sizeof(uint8_t));
		}

		base_params = BaseParams(reader);
		children = Children(reader);

		reader.Read(&grid_period);
		reader.Read(&grid_offset);
		reader.Read(&tempo);
		reader.Read(&time_signature_num_beats_bar);
		reader.Read(&time_signature_beat_value);
		reader.Read(&meter_info_flag);
		reader.Read(&num_stingers);
		for (uint32_t i = 0; i < num_stingers; i++) {
			stingers.push_back(MusicStinger(reader));
		}
	}

	void MusicNodeParams::Convert(Writer& writer) {
		writer << (uint8_t)0; // flags

		base_params.Convert(writer);
		children.Convert(writer);

		writer << grid_period;
		writer << grid_offset;
		writer << tempo;
		writer << time_signature_num_beats_bar;
		writer << time_signature_beat_value;
		writer << meter_info_flag;
		writer << num_stingers;
		for (uint32_t i = 0; i < num_stingers; i++) {
			stingers[i].Convert(writer);
		}
	}

	MusicTransitionRule::MusicTransitionRule(Reader& reader) {
		reader.Read(&num_sources);
		for (uint32_t i = 0; i < num_sources; i++) {
			source_ids.push_back(reader.Read<int32_t>());
		}

		reader.Read(&num_destinations);
		for (uint32_t i = 0; i < num_destinations; i++) {
			destination_ids.push_back(reader.Read<int32_t>());
		}

		reader.Read(&transition_time_dst);
		reader.Read(&fade_curve_dst);
		reader.Read(&fade_offset_dst);
		reader.Read(&sync_type_dst);
		reader.Read(&cue_filter_hash_dst);
		reader.Read(&play_post_exit_dst);

		reader.Read(&transition_time_src);
		reader.Read(&fade_curve_src);
		reader.Read(&fade_offset_src);
		reader.Read(&cue_filter_hash_src);
		reader.Read(&jump_to_id);
		if (VERSION == BankVersion::V2022) {
			jump_to_type = reader.Read<uint16_t>();
		}
		reader.Read(&entry_type);
		reader.Read(&play_pre_entry);
		reader.Read(&destination_match_source_cue_name);
		reader.Read(&allocate_transition_object_flag);

		if (allocate_transition_object_flag != 0) {
			trans_object_segment_id = reader.Read<uint32_t>();
			
			trans_object_fadein_time = reader.Read<uint32_t>();
			trans_object_fadein_fade_curve = reader.Read<CurveInterpolation>();
			trans_object_fadein_fade_offset = reader.Read<uint32_t>();

			trans_object_fadeout_time = reader.Read<uint32_t>();
			trans_object_fadeout_fade_curve = reader.Read<CurveInterpolation>();
			trans_object_fadeout_fade_offset = reader.Read<uint32_t>();

			trans_object_play_pre_entry = reader.Read<uint8_t>();
			trans_object_play_post_exit = reader.Read<uint8_t>();
		}
	}

	void MusicTransitionRule::Convert(Writer& writer) {
		writer << num_sources;
		for (uint32_t i = 0; i < num_sources; i++) {
			writer << source_ids[i];
		}

		writer << num_destinations;
		for (uint32_t i = 0; i < num_sources; i++) {
			writer << destination_ids[i];
		}

		writer << transition_time_dst;
		writer << fade_curve_dst;
		writer << fade_offset_dst;
		writer << sync_type_dst;
		writer << cue_filter_hash_dst;
		writer << play_post_exit_dst;

		writer << transition_time_src;
		writer << fade_curve_src;
		writer << fade_offset_src;
		writer << cue_filter_hash_src;
		writer << jump_to_id;

		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint16_t)0; // jump_to_type default for older
		}

		writer << entry_type;
		writer << play_pre_entry;
		writer << destination_match_source_cue_name;
		writer << allocate_transition_object_flag;
		if (allocate_transition_object_flag != 0) {
			writer << trans_object_segment_id.value();

			writer << trans_object_fadein_time.value();
			writer << trans_object_fadein_fade_curve.value();
			writer << trans_object_fadein_fade_offset.value();

			writer << trans_object_fadeout_time.value();
			writer << trans_object_fadeout_fade_curve.value();
			writer << trans_object_fadeout_fade_offset.value();

			writer << trans_object_play_pre_entry.value();
			writer << trans_object_play_post_exit.value();
		}
	}

	MusicTransitionNode::MusicTransitionNode(Reader& reader) 
		: node_params(reader)
	{
		reader.Read(&num_rules);
		for (uint32_t i = 0; i < num_rules; i++) {
			transition_rules.push_back(MusicTransitionRule(reader));
		}
	}

	void MusicTransitionNode::Convert(Writer& writer) {
		node_params.Convert(writer);
		writer << num_rules;
		for (uint32_t i = 0; i < num_rules; i++) {
			transition_rules[i].Convert(writer);
		}
	}

	// FX

	PluginParameters::PluginParameters(Reader& reader) {
		reader.Read(&size);

		data.resize(size);
		reader.Read(data.data(), size);
	}

	void PluginParameters::Convert(Writer& writer) {
		writer << size;
		writer.Write(*data.data(), size);
	}

	MediaMap::MediaMap(Reader& reader) {
		reader.Read(&index);
		reader.Read(&source_id);
	}

	void MediaMap::Convert(Writer& writer) {
		writer << index;
		writer << source_id;
	}

	RTPCInit2015::RTPCInit2015(Reader& reader) {
		reader.Read(&param_id);
		reader.Read(&init_value);
	}

	void RTPCInit2015::Convert(Writer& writer) {
		writer << param_id;
		writer << init_value;
	}

	PluginPropertyValue::PluginPropertyValue(Reader& reader) {
		reader.Read(&property_id);
		reader.Read(&rtpc_accum);
		reader.Read(&value);
	}

	void PluginPropertyValue::Convert(Writer& writer) {
		writer << property_id;
		writer << rtpc_accum;
		writer << value;
	}

	FXBase::FXBase(Reader& reader) 
		: fx_id(reader),
		parameters(reader)
	{
		reader.Read(&num_bank_data);
		for (uint8_t i = 0; i < num_bank_data; i++) {
			media.push_back(MediaMap(reader));
		}
		initial_rtpc = InitialRTPC(reader);

		if (VERSION == BankVersion::V2015) {
			num_init = reader.Read<uint16_t>();
			if (num_init > 0) {
				rtpc_inits.emplace();
				for (uint8_t i = 0; i < num_init; i++) {
					rtpc_inits.value().push_back(RTPCInit2015(reader));
				}
			}
		}
		else if (VERSION == BankVersion::V2022) {
			state_chunk = ParameterNodeStateChunkNew(reader);
			num_values = reader.Read<uint16_t>();
			if (num_values > 0) {
				property_values.emplace();
				for (uint8_t i = 0; i < num_values; i++) {
					property_values.value().push_back(PluginPropertyValue(reader));
				}
			}
		}
	}

	void FXBase::Convert(Writer& writer) {
		fx_id.Convert(writer);
		parameters.Convert(writer);

		writer << num_bank_data;
		for (uint8_t i = 0; i < num_bank_data; i++) {
			media[i].Convert(writer);
		}
		initial_rtpc.Convert(writer);

		if (CONVERT_VERSION == BankVersion::V2015) {
			writer << (uint16_t)0;
		}
		else { // 2022
			writer << (uint8_t)0; // num state props
			writer << (uint8_t)0; // num state groups

			writer << (uint16_t)0; // num_values
		}
	}
}