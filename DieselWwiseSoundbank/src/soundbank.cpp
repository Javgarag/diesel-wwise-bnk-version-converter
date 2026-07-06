#include "soundbank.h"
#include <iostream>

// todo: add optionals to auxparams, advsettingsaparams, statechunk, initial_rtpc

namespace Wwise {
	Soundbank::Soundbank(std::istream& bnk_stream)
	{
		Reader reader(bnk_stream);

		// BKHD		
		//std::cout << "Reading: BKHD (soundbank header)" << std::endl;
		bank_header = BKHD(reader);
		if (bank_header.section_info.header != Header::BKHD) {
			std::cerr << "ERROR: File is not a Wwise Soundbank (.bnk)!" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		switch (VERSION) {
		case BankVersion::V2013:
			break;
		case BankVersion::V2015:
			break;
		case BankVersion::V2022:
			break;
		default:
			std::cerr << "ERROR: Unsupported Soundbank version! Supported versions: 2013 (-v 88), 2015 (-v 113), 2022 (-v 145)" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		int next_header;
		while (reader.HasDataLeft()) {
			reader.Read(&next_header);

			switch ((Header)next_header) {
			case Header::STMG:
				reader.Seek(reader.Tell() - sizeof(int));
				global_settings = STMG(reader);
				break;
			case Header::DIDX:
				reader.Seek(reader.Tell() - sizeof(int));
				data_index = DIDX(reader);
				break;
			case Header::DATA:
				reader.Seek(reader.Tell() - sizeof(int));
				sound_data = DATA(reader);
				break;
			case Header::HIRC:
				reader.Seek(reader.Tell() - sizeof(int));
				objects = HIRC(reader);
				break;
			case Header::ENVS:
				reader.Seek(reader.Tell() - sizeof(int));
				enviroment_settings = ENVS(reader);
				break;
			case Header::STID:
				reader.Seek(reader.Tell() - sizeof(int));
				string_mapping = STID(reader);
				break;
			}
		}

		std::cout << "Successful parsing of version " << (int)VERSION << std::endl;
	};

	bool Soundbank::Convert(BankVersion new_version, std::ostream& bnk_stream) 
	{
		CONVERT_VERSION = new_version;
		if (CONVERT_VERSION != BankVersion::V2013 && CONVERT_VERSION != BankVersion::V2015 && CONVERT_VERSION != BankVersion::V2022) {
			std::cerr << "ERROR: Unsupported converted version; supported versions: 2013 (-v 88), 2015 (-v 113), 2022 (-v 145)" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (CONVERT_VERSION <= VERSION) {
			std::cerr << "ERROR: Conversion version cannot be lower than or equal to the current version!" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		if (objects.num_items == 0) {
			std::cerr << "ERROR: Soundbank contains no items! Impossible to \"convert\"" << std::endl;
			std::exit(EXIT_FAILURE);
		}

		Writer writer(bnk_stream);

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

		if (enviroment_settings) {
			enviroment_settings.value().Convert(writer);
		}

		if (string_mapping) {
			string_mapping.value().Convert(writer);
		}

		std::cout << "Successful conversion to version " << (int)new_version << std::endl;
		return true;
	}
} 