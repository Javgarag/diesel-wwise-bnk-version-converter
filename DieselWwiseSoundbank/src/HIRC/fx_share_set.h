#pragma once
#include "common.h"

namespace Wwise {
	struct ConvolutionReverbFXParams {
		float pre_delay;
		float front_rear_delay;
		float stereo_width;
		float input_center_level;
		float input_lfe_level;
		
		// 2022
		std::optional<float> input_stereo_width;

		float front_level;
		float rear_level;
		float center_level;
		float lfe_level;
		float dry_level;
		float wet_level;
		uint32_t algo_type;

		// 2022
		uint8_t unknown;
	};

	struct MeterFXParams {
		float rtpc_attack;
		float rtpc_release;
		float rtpc_min;
		float rtpc_max;
		float rtpc_hold;

		// 2022
		std::optional<uint8_t> rtpc_infinite_hold;
		// + 2013
		std::optional<uint8_t> non_rtpc_mode; // Peak or RMS (default; value "1")
		std::optional<uint8_t> non_rtpc_scope; // default 0, Global

		uint8_t non_rtpc_apply_downstream_volume;
		uint32_t non_rtpc_game_param_id;
	};

	struct PluginParameters {
		uint32_t size;

		// Pad the rest for plugins whose parameters dont change between versions
		std::vector<unsigned char> data;

		// Some specific plugin types do change (not implemented)
		std::variant<ConvolutionReverbFXParams, MeterFXParams> parameters;

		void Convert(Writer& writer);
		PluginParameters() = default;
		PluginParameters(Reader& reader);
	};

	struct MediaMap {
		uint8_t index;
		uint32_t source_id;

		void Convert(Writer& writer);
		MediaMap() = default;
		MediaMap(Reader& reader);
	};

	struct RTPCInit2015 {
		uint8_t param_id; // AkRTPC_ParameterID
		float init_value;

		void Convert(Writer& writer);
		RTPCInit2015() = default;
		RTPCInit2015(Reader& reader);
	};

	struct PluginPropertyValue {
		uint8_t property_id;
		uint8_t rtpc_accum;
		float value;

		void Convert(Writer& writer);
		PluginPropertyValue() = default;
		PluginPropertyValue(Reader& reader);
	};
	
	struct HIRCFxShareSet {
		HIRCItemGeneric item_base;
		Plugin fx_id;
		PluginParameters parameters;

		uint8_t num_bank_data;
		std::vector<MediaMap> media;
		InitialRTPC initial_rtpc;

		// 2015
		std::optional<uint16_t> num_init;
		std::optional<std::vector<RTPCInit2015>> rtpc_inits;

		// 2022
		std::optional<ParameterNodeStateChunkNew> state_chunk;
		std::optional<uint16_t> num_values;
		std::optional<std::vector<PluginPropertyValue>> property_values;

		void Convert(Writer& writer);
		HIRCFxShareSet() = default;
		HIRCFxShareSet(Reader& reader);
	};
}