#include "soundbank.h"
#include <iostream>

// todo: add optionals to auxparams, advsettingsaparams, statechunk, initial_rtpc

namespace Wwise {
	Soundbank::Soundbank(const std::filesystem::path& file_path)
	{
		reader = Reader(file_path);

		// BKHD		
		bank_header = BKHD(reader);
		if (bank_header.section_info.header != Header::BKHD) {
			throw std::runtime_error("File is not a Wwise Soundbank (.bnk)!");
		}
		switch (VERSION) {
		case BankVersion::V2013:
			break;
		case BankVersion::V2015:
			break;
		case BankVersion::V2022:
			break;
		default:
			throw std::runtime_error("Unsupported Soundbank version! Supported versions: 2013 (88), 2015 (113), 2022 (145)");
		}

		// STMG (init.bnk)
		if (size_t stmg_address = reader.SearchAddress(Header::STMG)) {
			reader.Seek(stmg_address);
			global_settings = STMG(reader);
		}

		// DIDX
		if (size_t didx_address = reader.SearchAddress(Header::DIDX)) {
			reader.Seek(didx_address);
			data_index = DIDX(reader);
		}

		// DATA
		if (size_t data_address = reader.SearchAddress(Header::DATA)) {
			reader.Seek(data_address);
			sound_data = DATA(reader);
		}

		// HIRC
		std::cout << "Searching for HIRC header..." << std::endl;
		size_t hirc_address = reader.SearchAddress(Header::HIRC);
		if (!hirc_address) {
			throw std::runtime_error("Soundbank has no readable objects!");
		}
		reader.Seek(hirc_address);
		objects = HIRC(reader);

		// STID
		std::cout << "Searching for STID header..." << std::endl;
		if (size_t stid_address = reader.SearchAddress(Header::STID)) {
			reader.Seek(stid_address);
			string_mapping = STID(reader);
		}

		reader.CloseFile();
		std::cout << "Successful parsing of version " << (int)VERSION << std::endl;
	};

	bool Soundbank::Convert(BankVersion new_version, const std::filesystem::path& file_path) {
		// Todo: last few sections on init.bnk

		CONVERT_VERSION = new_version;

		if (CONVERT_VERSION <= VERSION) {
			throw std::runtime_error("Converted bank version cannot be lower than or equal to the current version!");
		}

		writer = Writer(file_path);

		bank_header.Convert(writer);

		if (global_settings) {
			global_settings.value().Convert(writer);
		}

		if (data_index) {
			data_index.value().Convert(writer);
		}

		if (sound_data) {
			sound_data.value().Convert(writer);
		}

		objects.Convert(writer);

		if (string_mapping) {
			string_mapping.value().Convert(writer);
		}

		writer.CloseFile();

		std::cout << std::endl << "Successful conversion to version " << (int)new_version << std::endl;
		return true;
	}
} 