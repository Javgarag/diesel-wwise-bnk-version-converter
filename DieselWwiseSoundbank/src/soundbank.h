#pragma once
#include "globals.h"
#include "ak_types.h"

#include "bkhd.h"
#include "didx.h"
#include "data.h"
#include "stmg.h"
#include "hirc.h"
#include "envs.h"
#include "stid.h"

#include "reader.h"
#include "writer.h"

#include <string>
#include <filesystem>
#include <variant>
#include <array>
#include <optional>

namespace Wwise {
	class Soundbank {
	public:
		BKHD bank_header;
		std::optional<DIDX> data_index;
		std::optional<DATA> sound_data;
		std::optional<STMG> global_settings;
		HIRC objects;
		std::optional<ENVS> enviroment_settings;
		std::optional<STID> string_mapping;
		
		Soundbank(std::istream& bnk_stream);
		Soundbank(uint8_t* data, size_t size);

		void Parse(Reader& reader);
		void Convert(BankVersion new_version, std::ostream& bnk_stream);
		void Convert(BankVersion new_version, std::vector<uint8_t>& out);
	private:
		void _Convert(Writer& writer);
	};
}