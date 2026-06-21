#include "stmg.h"

namespace Wwise {
	StateTransition::StateTransition(Reader& reader) {
        reader.Read(&state_from);
        reader.Read(&state_to);
        reader.Read(&transition_time);
	}

    void StateTransition::Convert(Writer& writer) {
        writer << state_from;
        writer << state_to;
        writer << transition_time;
    }
	
    StateGroupSTMG::StateGroupSTMG(Reader& reader) {
        reader.Read(&state_group_id);
        reader.Read(&default_transition_time);

        reader.Read(&num_transitions);
        for (uint32_t i = 0; i < num_transitions; i++) {
            map_transitions.push_back(StateTransition(reader));
        }
	}

    void StateGroupSTMG::Convert(Writer& writer) {
        writer << state_group_id;
        writer << default_transition_time;
        writer << num_transitions;
        for (uint32_t i = 0; i < num_transitions; i++) {
            map_transitions[i].Convert(writer);
        }
    }

	SwitchGroup::SwitchGroup(Reader& reader) {
        reader.Read(&switch_group_id);
        reader.Read(&rtpc_id);

        if (VERSION == BankVersion::V2022) {
            rtpc_type = reader.Read<uint8_t>();
        }

        rtpc_graph = RTPCGraph(reader);
	}

    void SwitchGroup::Convert(Writer& writer) {
        writer << switch_group_id;
        writer << rtpc_id;

        if (CONVERT_VERSION == BankVersion::V2022) {
            writer << (uint8_t)0; // GameParameter
        }

        rtpc_graph.Convert(writer);
    }

	AcousticTexture::AcousticTexture(Reader& reader) {
        reader.Read(&id);
        reader.Read(&absorption_offset);
        reader.Read(&absorption_low);
        reader.Read(&absorption_mid_low);
        reader.Read(&absorption_mid_high);
        reader.Read(&absorption_high);
        reader.Read(&scattering);
	}

    void AcousticTexture::Convert(Writer& writer) {
        writer << id;
        writer << absorption_offset;
        writer << absorption_low;
        writer << absorption_mid_low;
        writer << absorption_mid_high;
        writer << absorption_high;
        writer << scattering;
    }

	RTPCRamping::RTPCRamping(Reader& reader) {
        reader.Read(&rtpc_id);
        reader.Read(&value);

        if (VERSION > BankVersion::V2013) {
            ramp_type = reader.Read<uint32_t>();
            ramp_up = reader.Read<float>();
            ramp_down = reader.Read<float>();
            bind_to_built_in_param = reader.Read<uint8_t>();
        }
	}

    void RTPCRamping::Convert(Writer& writer) {
        writer << rtpc_id;
        writer << value;
        
        if (VERSION == BankVersion::V2013) {
            writer << (uint32_t)0;
            writer << (float)0;
            writer << (float)0;
            writer << (uint8_t)0;
        }
        else { // 2015 -> 2022
            writer << ramp_type.value();
            writer << ramp_up.value();
            writer << ramp_down.value();
            writer << bind_to_built_in_param.value();
        }
    }

	STMG::STMG(Reader& reader) {
        std::cout << "Reading: STMG (global settings)" << std::endl;
        section_info = Section(reader);

        if (VERSION == BankVersion::V2022) {
            filter_behavior = reader.Read<uint16_t>();
        }

        reader.Read(&volume_threshold);
        reader.Read(&max_num_voices_limit_internal);

        if (VERSION == BankVersion::V2022) {
            max_num_dangerous_virtual_voices_limit_internal = reader.Read<uint16_t>();
        }

        reader.Read(&num_state_groups);
        for (uint32_t i = 0; i < num_state_groups; i++) {
            state_groups.push_back(StateGroupSTMG(reader));
        }

        reader.Read(&num_switch_groups);
        for (uint32_t i = 0; i < num_switch_groups; i++) {
            switch_groups.push_back(SwitchGroup(reader));
        }

        reader.Read(&num_params);
        for (uint32_t i = 0; i < num_params; i++) {
            rtpc_manager.push_back(RTPCRamping(reader));
        }

        if (VERSION == BankVersion::V2022) {
            num_textures = reader.Read<uint32_t>();
            acoustic_textures.emplace();
            for (uint32_t i = 0; i < num_textures; i++) {
                acoustic_textures.value().push_back(AcousticTexture(reader));
            }
        }
	}

    void STMG::Convert(Writer& writer) {
        section_info.Convert(writer);

        if (CONVERT_VERSION == BankVersion::V2022) {
            writer << (uint16_t)0; //filter_behavior default
        }

        writer << volume_threshold;
        writer << max_num_voices_limit_internal;

        if (CONVERT_VERSION == BankVersion::V2022) {
            writer << (uint16_t)50; //max_num_dangerous_virtual_voices_limit_internal default
        }

        writer << num_state_groups;
        for (uint32_t i = 0; i < num_state_groups; i++) {
            state_groups[i].Convert(writer);
        }

        writer << num_switch_groups;
        for (uint32_t i = 0; i < num_switch_groups; i++) {
            switch_groups[i].Convert(writer);
        }
        
        writer << num_params;
        for (uint32_t i = 0; i < num_params; i++) {
            rtpc_manager[i].Convert(writer);
        }

        if (CONVERT_VERSION == BankVersion::V2022) {
            writer << (uint32_t)0; // num_textures
        }

        section_info.UpdateSize(writer);
    }
}
