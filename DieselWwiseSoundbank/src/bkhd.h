#pragma once
#include "reader.h"
#include "writer.h"
#include "globals.h"

#include <iostream>
#include <optional>
#include <array>

namespace Wwise {
	struct Section {
		Header header;
		uint32_t size;

		long long size_position = -1;

		Section() = default;
		Section(Reader& reader);
		void Convert(Writer& writer);
		void UpdateSize(Writer& writer);
	};

	struct BKHD {
		Section section_info;

		BankVersion version;
		uint32_t bank_id;
		uint32_t language_id;
		uint32_t feedback_in_bank;
		uint32_t project_id;

		// 2022
		std::optional<uint32_t> soundbank_type;
		std::optional<std::array<uint8_t, 16>> bank_hash;

		void Convert(Writer& writer);

		BKHD() = default;
		BKHD(Reader& reader);
	};
}