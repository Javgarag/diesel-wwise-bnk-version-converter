#include "action.h"

namespace Wwise {
	ActionInitialValues::ActionInitialValues(Reader& reader) {
		reader.Read(&id_ext);
		reader.Read(&id_ext_4, sizeof(uint8_t));

		properties = PropertyBundle(reader);
		ranged_properties = RangedModifierPropertyBundle(reader);
	}

	void ActionInitialValues::Convert(Writer& writer) {
		writer << id_ext;
		writer.Write(id_ext_4, sizeof(uint8_t));

		properties.Convert(writer);
		ranged_properties.Convert(writer);
	}

	HIRCActionPlay::HIRCActionPlay(Reader& reader) {
		reader.Read(&bank_id);
		if (VERSION == BankVersion::V2022) {
			reader.Read(&bank_type);
		}
	}

	void HIRCActionPlay::Convert(Writer& writer) {
		writer << bank_id;
		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint32_t)0; // guessed default
		}
	}

	HIRCActionStop::HIRCActionStop(Reader& reader) {
		if (VERSION == BankVersion::V2022) {
			params = reader.Read<ActionStopParams>(sizeof(uint8_t));
		}
		except_params = ActionExceptParams(reader);
	}

	void HIRCActionStop::Convert(Writer& writer) {
		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint8_t)6; // 0110; ActionStopParams
		}

		except_params.Convert(writer);
	}

	ActionException::ActionException(Reader& reader) {
		reader.Read(&id);
		reader.Read(&is_bus);
	}

	void ActionException::Convert(Writer& writer) {
		writer << id;
		writer << is_bus;
	}

	ActionExceptParams::ActionExceptParams(Reader& reader) {
		if (VERSION < BankVersion::V2022) {
			reader.Read(&exception_list_size.emplace<uint32_t>());
		}
		else {
			reader.Read(&exception_list_size.emplace<uint8_t>());
		}

		std::visit([&](auto& exception_list_size) {
			if (exception_list_size == 0) {
				return;
			}

			exceptions.emplace();
			for (unsigned int i = 0; i < exception_list_size; i++) {
				exceptions.value().push_back(ActionException(reader));
			}
		}, exception_list_size);
	}

	void ActionExceptParams::Convert(Writer& writer) {
		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint8_t)std::get<uint32_t>(exception_list_size);
		}
		else {
			writer << std::get<uint32_t>(exception_list_size);
		}

		std::visit([&](auto& exception_list_size) {
			if (exception_list_size == 0) {
				return;
			}

			for (unsigned int i = 0; i < exception_list_size; i++) {
				exceptions.value()[i].Convert(writer);
			}
		}, exception_list_size);
	}

	HIRCActionSetGameParameter::HIRCActionSetGameParameter(Reader& reader) {
		if (VERSION > BankVersion::V2013) {
			bypass_transition = reader.Read<uint8_t>();
		}

		reader.Read(&value_set_params.value_meaning);
		reader.Read(&value_set_params.base);
		reader.Read(&value_set_params.min);
		reader.Read(&value_set_params.max);

		except_params = ActionExceptParams(reader);
	}

	void HIRCActionSetGameParameter::Convert(Writer& writer) {
		if (bypass_transition.has_value()) {
			writer << bypass_transition.value();
		}
		else {
			writer << (uint8_t)0; //default
		}

		value_set_params.Convert(writer);
		except_params.Convert(writer);
	}

	void ValueSetterActionParameters::Convert(Writer& writer) {
		writer << value_meaning;
		writer << base;
		writer << min;
		writer << max;
	}

	HIRCActionSetProperty::HIRCActionSetProperty(Reader& reader) {
		reader.Read(&value_set_params.value_meaning);
		reader.Read(&value_set_params.base);
		reader.Read(&value_set_params.min);
		reader.Read(&value_set_params.max);

		except_params = ActionExceptParams(reader);
	}

	void HIRCActionSetProperty::Convert(Writer& writer) {
		value_set_params.Convert(writer);
		except_params.Convert(writer);
	}

	HIRCActionBypassFX::HIRCActionBypassFX(Reader& reader) {
		reader.Read(&is_bypass);
		reader.Read(&target_mask);
		except_params = ActionExceptParams(reader);
	}
	
	void HIRCActionBypassFX::Convert(Writer& writer) {
		writer << is_bypass;
		writer << target_mask;
		except_params.Convert(writer);
	}

	HIRCActionSetSwitch::HIRCActionSetSwitch(Reader& reader) {
		reader.Read(&switch_group_id);
		reader.Read(&switch_state_id);
	}

	void HIRCActionSetSwitch::Convert(Writer& writer) {
		writer << switch_group_id;
		writer << switch_state_id;
	}

	HIRCActionPauseResume::HIRCActionPauseResume(Reader& reader) {
		reader.Read(&flags);
		except_params = ActionExceptParams(reader);
	}

	void HIRCActionPauseResume::Convert(Writer& writer) {
		writer << flags;
		except_params.Convert(writer);
	}

	HIRCActionBase::HIRCActionBase(Reader& reader)
		: item_base(reader)
	{
		reader.Read(&action_type);

		initial_values = ActionInitialValues(reader);

		int action_type_simple = (uint16_t)(action_type) & 0xFF00;
		switch (action_type_simple) {
		case 0x1C00:
			// "Break" action
			return;
		case 0x1000:
			// "Use State" action
			return;
		case 0x1100:
			// "Use State" action
			return;
		case 0x1D00:
			// "Trigger" action
			return;
		case 0x2100:
			// "PlayEvent" action
			return;
		case 0x1900:
			// "SetSwitch" action
			action = HIRCActionSetSwitch(reader);
			return;
		}

		switch (action_type_simple) {
		case 0x1A00: // bypass
			action = HIRCActionBypassFX(reader);
			break;
		case 0x1B00: // bypass reset
			action = HIRCActionBypassFX(reader);
			break;
		default:
			reader.Read(&fade_curve, sizeof(uint8_t));
		}

		switch (action_type_simple) {
		case 0x0100:
			action = HIRCActionStop(reader);
			break;
		case 0x0200: // Pause
			action = HIRCActionPauseResume(reader);
			break;
		case 0x0300:
			action = HIRCActionPauseResume(reader);
			break;
		case 0x0400:
			action = HIRCActionPlay(reader);
			break;
		case 0x1300:
			action = HIRCActionSetGameParameter(reader);
			break;
		case 0x1400:
			action = HIRCActionSetGameParameter(reader);
			break;
		case 0x0800: // pitch
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0900: // pitch reset
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0A00: // volume
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0B00: // volume reset
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0C00: // bus volume
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0D00: // bus volume reset
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0E00: // lpf
			action = HIRCActionSetProperty(reader);
			break;
		case 0x0F00: // lpf reset
			action = HIRCActionSetProperty(reader);
			break;
		case 0x2000: // hpf
			action = HIRCActionSetProperty(reader);
			break;
		case 0x3000: // hpf reset
			action = HIRCActionSetProperty(reader);
			break;
		}

		// safety padding
		reader.Seek(item_base.data_start + item_base.size);
	}

	void HIRCActionBase::Convert(Writer& writer) {
		item_base.Convert(writer);

		writer << (uint16_t)action_type;
		initial_values.Convert(writer);

		int action_type_simple = (uint16_t)(action_type) & 0xFF00;
		switch (action_type_simple) {
		case 0x1C00:
			// "Break" action
			item_base.UpdateSize(writer);
			return;
		case 0x1000:
			// "Use State" action
			item_base.UpdateSize(writer);
			return;
		case 0x1100:
			// "Use State" action
			item_base.UpdateSize(writer);
			return;
		case 0x1D00:
			// "Trigger" action
			item_base.UpdateSize(writer);
			return;
		case 0x2100:
			// "PlayEvent" action
			item_base.UpdateSize(writer);
			return;
		case 0x1900:
			// "SetSwitch" action
			std::get<HIRCActionSetSwitch>(action).Convert(writer);
			item_base.UpdateSize(writer);
			return;
		}

		switch (action_type_simple) {
		case 0x1A00: // bypass
			break;
		case 0x1B00: // bypass reset
			break;
		default:
			writer.Write(fade_curve, sizeof(uint8_t));
		}
		
		std::visit([&](auto& action) {
			action.Convert(writer);
		}, action);

		item_base.UpdateSize(writer);
	}
}