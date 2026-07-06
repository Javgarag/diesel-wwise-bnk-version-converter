#include "reader.h"
#include "ak_types.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stack>

namespace Wwise {
	Reader::Reader(std::istream& istream)
		: stream(istream)
	{
		PushCurrentPos();
		stream.seekg(0, std::ios::end);
		this->size = stream.tellg();
		PopLastPos();
	};

	size_t Reader::Tell() {
		return stream.tellg();
	};

	void Reader::PushCurrentPos() {
		savedPositions.push(stream.tellg());
	};

	void Reader::PopLastPos() {
		stream.seekg(savedPositions.top());
		savedPositions.pop();
	};

	void Reader::Seek(size_t address) {
		stream.seekg(address);
	};

	std::string Reader::ReadNullTerminatedString() {
		std::string result;
		char c;

		while (stream.get(c) && c != '\0')
		{
			result += c;
		}

		return result;
	}

	bool Reader::HasDataLeft() {
		size_t pos = stream.tellg();

		if (pos > (size - sizeof(Header))) {
			return false;
		}
		else {
			return true;
		}
	}
};