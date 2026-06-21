#pragma once
#include "../DieselWwiseSoundbank.h"
#include "globals.h"

#include "bkhd.h"
#include "didx.h"
#include "data.h"
#include "stmg.h"
#include "hirc.h"
#include "stid.h"

#include "reader.h"
#include "writer.h"

#include <string>
#include <filesystem>
#include <variant>
#include <array>
#include <optional>

namespace Wwise {
	class DIESELWWISESOUNDBANK_API Soundbank {
	public:
		BKHD bank_header;
		std::optional<DIDX> data_index;
		std::optional<DATA> sound_data;
		std::optional<STMG> global_settings;
		HIRC objects;
		std::optional<STID> string_mapping;

		bool Convert(BankVersion new_version, const std::filesystem::path& file_path);
		Soundbank(const std::filesystem::path& file_path);
	private:
		Reader reader;
		Writer writer;
	};
}