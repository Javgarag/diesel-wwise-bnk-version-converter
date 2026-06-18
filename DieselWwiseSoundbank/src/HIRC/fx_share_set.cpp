#include "fx_share_set.h"

namespace Wwise {
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

	HIRCFxShareSet::HIRCFxShareSet(Reader& reader) 
		: item_base(reader),
		fx_id(reader),
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

		// safety padding
		reader.Seek(item_base.data_start + item_base.size);
	}

	void HIRCFxShareSet::Convert(Writer& writer) {
		item_base.Convert(writer);

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

		item_base.UpdateSize(writer);
	}
}