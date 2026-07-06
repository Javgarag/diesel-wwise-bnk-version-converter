#pragma once
#include "ak_types.h"
#include <cstdint>
#include <istream>
#include <stack>

namespace Wwise {
	class Reader {
	public:
		template<typename T>
		void Read(T* value, size_t size = sizeof(T))
		{
			stream.read(reinterpret_cast<char*>(value), size);
		}

		template<typename T>
		T Read(size_t size = sizeof(T))
		{
			T value;
			stream.read(reinterpret_cast<char*>(&value), size);
			return value;
		}

		Reader(std::istream& istream);
		size_t Tell();
		void PushCurrentPos();
		void PopLastPos();
		void Seek(size_t address);
		bool HasDataLeft();
		std::string ReadNullTerminatedString();
	private:
		std::istream& stream;
		std::streamsize size;
		std::stack<size_t> savedPositions;
	};
}