#include "soundbank.h"
#include <iostream>

// todo: add optionals to auxparams, advsettingsaparams, statechunk, initial_rtpc

namespace Wwise {
	Soundbank::Soundbank(std::istream& bnk_stream)
	{
		FileReader reader(bnk_stream);
		this->Parse(reader);
	};

	Soundbank::Soundbank(uint8_t* data, size_t size)
	{
		MemoryReader reader(data, size);
		this->Parse(reader);
	}

	void Soundbank::Parse(Reader& reader) 
	{
		// BKHD		
		//std::cout << "Reading: BKHD (soundbank header)" << std::endl;
		bank_header = BKHD(reader);
		if (bank_header.section_info.header != Header::BKHD) {
			std::cerr << "[WwiseConverter] ERROR: File is not a Wwise Soundbank (.bnk)!" << std::endl;
			throw std::runtime_error("Attempted to convert a file that is not a soundbank with this converter");
		}
		switch (VERSION) {
		case BankVersion::V2013:
			break;
		case BankVersion::V2015:
			break;
		case BankVersion::V2022:
			break;
		default:
			std::cerr << "[WwiseConverter] ERROR: Unsupported Soundbank version! Supported versions: 2013 (88), 2015 (113), 2022 (145)" << std::endl;
			throw std::runtime_error("Attempted to convert soundbank to an invalid version");
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

		//std::cout << "Successful parsing of version " << (int)VERSION << std::endl;
	}

	void Soundbank::Convert(BankVersion new_version, std::ostream& bnk_stream) 
	{
		CONVERT_VERSION = new_version;
		FileWriter writer(bnk_stream);
		this->_Convert(writer);
	}

	void Soundbank::Convert(BankVersion new_version, std::vector<uint8_t>& out)
	{
		CONVERT_VERSION = new_version;
		MemoryWriter writer(out);
		this->_Convert(writer);
	}

	void Soundbank::_Convert(Writer& writer) {
		if (CONVERT_VERSION != BankVersion::V2013 && CONVERT_VERSION != BankVersion::V2015 && CONVERT_VERSION != BankVersion::V2022) {
			std::cerr << "[WwiseConverter] ERROR: Unsupported converted version; supported versions: 2013 (88), 2015 (113), 2022 (145)" << std::endl;
			throw std::runtime_error("Attempted to convert soundbank to an invalid version");
		}

		if (CONVERT_VERSION <= VERSION) {
			std::cerr << "[WwiseConverter] ERROR: Conversion version cannot be lower than or equal to the current version!" << std::endl;
			throw std::runtime_error("Attempted to convert soundbank to an invalid version");
		}

		if (objects.num_items == 0) {
			std::cerr << "[WwiseConverter] ERROR: Soundbank contains no items! Impossible to \"convert\"" << std::endl;
			throw std::runtime_error("Attempted to convert soundbank with no items");
		}

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

		std::cout << "[WwiseConverter] INFO: successfully migrated bank to version " << (int)CONVERT_VERSION << std::endl;
	}
} 