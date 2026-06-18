#pragma once
#include "common.h"

namespace Wwise {
	struct IDExt4 {
		bool is_bus : 1;
		unsigned char : 7;
	};

	struct ActionInitialValues {
		uint32_t id_ext;
		IDExt4 id_ext_4;

		PropertyBundle properties;
		RangedModifierPropertyBundle ranged_properties;

		void Convert(Writer& writer);
		ActionInitialValues() = default;
		ActionInitialValues(Reader& reader);
	};

	struct ActionFadeCurve {
		CurveInterpolation fade_curve : 5;
		unsigned char : 3;
	};

	struct HIRCActionPlay {
		uint32_t bank_id;
		std::optional<uint32_t> bank_type; // AkBankTypeEnum, 2022

		void Convert(Writer& writer);
		HIRCActionPlay() = default;
		HIRCActionPlay(Reader& reader);
	};

	struct ActionStopParams {
		unsigned char : 1;
		bool apply_to_state_transitions : 1;
		bool apply_to_dynamic_sequence : 1;
		unsigned char : 5;
	};

	struct ActionException {
		uint32_t id;
		uint8_t is_bus;

		void Convert(Writer& writer);
		ActionException() = default;
		ActionException(Reader& reader);
	};

	struct ActionExceptParams {
		std::variant<uint32_t, uint8_t> exception_list_size;
		std::optional<std::vector<ActionException>> exceptions;

		void Convert(Writer& writer);
		ActionExceptParams() = default;
		ActionExceptParams(Reader& reader);
	};

	struct HIRCActionStop {
		// 2022
		std::optional<ActionStopParams> params;

		// all
		ActionExceptParams except_params;

		void Convert(Writer& writer);
		HIRCActionStop() = default;
		HIRCActionStop(Reader& reader);
	};

	struct ValueSetterActionParameters {
		uint8_t value_meaning; // AkValueMeaning 

		float base;
		float min;
		float max;

		void Convert(Writer& writer);
	};

	struct HIRCActionSetGameParameter {
		// + 2013
		std::optional<uint8_t> bypass_transition;

		ValueSetterActionParameters value_set_params;
		ActionExceptParams except_params;

		void Convert(Writer& writer);
		HIRCActionSetGameParameter() = default;
		HIRCActionSetGameParameter(Reader& reader);
	};

	struct HIRCActionSetProperty {
		ValueSetterActionParameters value_set_params;
		ActionExceptParams except_params;

		void Convert(Writer& writer);
		HIRCActionSetProperty() = default;
		HIRCActionSetProperty(Reader& reader);
	};

	struct HIRCActionBypassFX {
		uint8_t is_bypass;
		uint8_t target_mask;
		ActionExceptParams except_params;

		void Convert(Writer& writer);
		HIRCActionBypassFX() = default;
		HIRCActionBypassFX(Reader& reader);
	};

	struct HIRCActionSetSwitch {
		uint32_t switch_group_id;
		uint32_t switch_state_id;

		void Convert(Writer& writer);
		HIRCActionSetSwitch() = default;
		HIRCActionSetSwitch(Reader& reader);
	};

	struct HIRCActionPauseResume {
		uint8_t flags;
		ActionExceptParams except_params;

		void Convert(Writer& writer);
		HIRCActionPauseResume() = default;
		HIRCActionPauseResume(Reader& reader);
	};

	struct HIRCActionBase {
		HIRCItemGeneric item_base;
		ActionType action_type;

		ActionInitialValues initial_values;
		ActionFadeCurve fade_curve;
		std::variant<HIRCActionPlay, HIRCActionStop, HIRCActionSetGameParameter, HIRCActionSetProperty, HIRCActionBypassFX, HIRCActionSetSwitch, HIRCActionPauseResume> action;

		void Convert(Writer& writer);
		HIRCActionBase(Reader& reader);
	};
}