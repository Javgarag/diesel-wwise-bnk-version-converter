#include "reader.h"
#include "ak_types.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stack>

namespace Wwise {
	FileReader::FileReader(std::istream& istream)
		: stream(istream)
	{
		PushCurrentPos();
		stream.seekg(0, std::ios::end);
		this->size = stream.tellg();
		PopLastPos();
	};

	void FileReader::Seek(size_t address) {
		stream.seekg(address);
	};

	size_t FileReader::Tell() {
		return stream.tellg();
	};

	void FileReader::PushCurrentPos() {
		savedPositions.push(stream.tellg());
	};

	void FileReader::PopLastPos() {
		stream.seekg(savedPositions.top());
		savedPositions.pop();
	};

	std::string FileReader::ReadNullTerminatedString() {
		std::string result;
		char c;

		while (stream.get(c) && c != '\0')
		{
			result += c;
		}

		return result;
	}

	bool FileReader::HasDataLeft() {
		size_t pos = stream.tellg();

		if (pos > (size - sizeof(Header))) {
			return false;
		}
		else {
			return true;
		}
	}

	void FileReader::ReadBytes(void* dest, size_t size) {
		stream.read(reinterpret_cast<char*>(dest), size);
	};

	MemoryReader::MemoryReader(uint8_t* data, size_t size)
		: data(data), size(size)
	{};

	void MemoryReader::Seek(size_t address) {
		pos = address;
	};

	size_t MemoryReader::Tell() {
		return pos;
	};

	void MemoryReader::PushCurrentPos() {
		savedPositions.push(pos);
	};

	void MemoryReader::PopLastPos() {
		pos = savedPositions.top();
		savedPositions.pop();
	};

	std::string MemoryReader::ReadNullTerminatedString() {
		std::string result;
		char c;

		ReadBytes(&c, sizeof(c));
		while (c != '\0')
		{
			result += c;
		}

		return result;
	}

	bool MemoryReader::HasDataLeft() {
		if (pos > (size - sizeof(Header))) {
			return false;
		}
		else {
			return true;
		}
	}

	void MemoryReader::ReadBytes(void* dest, size_t sizeToRead) {
		if (pos > size)
			throw std::out_of_range("[WwiseConverter] ERROR: Read past end of buffer");

		memcpy(dest, data + pos, sizeToRead);
		pos += sizeToRead;
	};
};