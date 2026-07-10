#include "bkhd.h"

namespace Wwise {
	BKHD::BKHD(Reader& reader) {
		section_info = Section(reader);

		reader.Read(&version);
		reader.Read(&bank_id);
		reader.Read(&language_id);
		reader.Read(&feedback_in_bank);
		reader.Read(&project_id);

		VERSION = version;
		FEEDBACK_IN_BANK = feedback_in_bank;

		if (VERSION == BankVersion::V2022) {
			soundbank_type = reader.Read<uint32_t>();
			reader.Read(bank_hash.emplace().data(), sizeof(char) * 16);
		}
	}

	void BKHD::Convert(Writer& writer) {
		section_info.Convert(writer);

		writer << CONVERT_VERSION;
		writer << bank_id;
		writer << language_id;
		writer << feedback_in_bank;
		writer << project_id; // project ID is changed when migrating between versions but it doesn't affect loading

		if (CONVERT_VERSION == BankVersion::V2022) {
			writer << (uint32_t)0; // soundbank_type. 0 == User
			writer << std::array<char, 16>{}; // bank_hash. Only used inside Wwise for tracking changes.
		}

		section_info.UpdateSize(writer);
	}

	Section::Section(Reader& reader) {
		reader.Read(&header);
		reader.Read(&size);
	}

	void Section::Convert(Writer& writer) {
		writer << header;
		size_position = writer.Tell();
		writer << (uint32_t)0; // temp, will get set later during conversion
	}

	void Section::UpdateSize(Writer& writer) {
		size_t final_size = writer.Tell() - (size_position + sizeof(uint32_t));

		writer.PushCurrentPos();
		writer.Seek(size_position);
		writer << (uint32_t)final_size;
		writer.PopLastPos();
	}
}