#pragma once
#include "bkhd.h"
#include "HIRC/common.h"

namespace Wwise {
    struct StateTransition {
        uint32_t state_from;
        uint32_t state_to;
        uint32_t transition_time;

        void Convert(Writer& writer);
        StateTransition() = default;
        StateTransition(Reader& reader);
    };

    struct StateGroupSTMG {
        uint32_t state_group_id;
        uint32_t default_transition_time;

        uint32_t num_transitions;
        std::vector<StateTransition> map_transitions;

        void Convert(Writer& writer);
        StateGroupSTMG() = default;
        StateGroupSTMG(Reader& reader);
    };

    struct SwitchGroup {
        uint32_t switch_group_id;
        uint32_t rtpc_id;

        // 2022
        std::optional<uint8_t> rtpc_type;

        RTPCGraph rtpc_graph;

        void Convert(Writer& writer);
        SwitchGroup() = default;
        SwitchGroup(Reader& reader);
    };

    struct AcousticTexture {
        uint32_t id;
        float absorption_offset;
        float absorption_low;
        float absorption_mid_low;
        float absorption_mid_high;
        float absorption_high;
        float scattering;

        void Convert(Writer& writer);
        AcousticTexture() = default;
        AcousticTexture(Reader& reader);
    };

    struct RTPCRamping {
        uint32_t rtpc_id;
        float value;

        // + 2013
        std::optional<uint32_t> ramp_type;
        std::optional<float> ramp_up;
        std::optional<float> ramp_down;
        std::optional<uint8_t> bind_to_built_in_param;

        void Convert(Writer& writer);
        RTPCRamping() = default;
        RTPCRamping(Reader& reader);
    };

	struct STMG {
        Section section_info;

        // 2022
        std::optional<uint16_t> filter_behavior;

        float volume_threshold;
        uint16_t max_num_voices_limit_internal;

        // 2022
        std::optional<uint16_t> max_num_dangerous_virtual_voices_limit_internal;

        uint32_t num_state_groups;
        std::vector<StateGroupSTMG> state_groups;

        uint32_t num_switch_groups;
        std::vector<SwitchGroup> switch_groups;

        uint32_t num_params;
        std::vector<RTPCRamping> rtpc_manager;

        // 2022
        std::optional<uint32_t> num_textures;
        std::optional<std::vector<AcousticTexture>> acoustic_textures;

        void Convert(Writer& writer);
        STMG() = default;
        STMG(Reader& reader);
	};
}