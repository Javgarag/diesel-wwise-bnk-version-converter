#pragma once
#include "ak_types.h"
#include <cstdint>
#include <istream>
#include <stack>

namespace Wwise {
	class Reader {
	public:
		virtual ~Reader() = default;

		template<typename T>
		void Read(T* value, size_t size = sizeof(T))
		{
			ReadBytes(value, size);
		}

		template<typename T>
		T Read(size_t size = sizeof(T))
		{
			T value;
			ReadBytes(&value, size);
			return value;
		}

		virtual void Seek(size_t address) = 0;
		virtual size_t Tell() = 0;
		virtual void PushCurrentPos() = 0;
		virtual void PopLastPos() = 0;
		virtual bool HasDataLeft() = 0;
		virtual std::string ReadNullTerminatedString() = 0;
	protected:
		virtual void ReadBytes(void* dest, size_t size) = 0;
		std::stack<size_t> savedPositions;
	};

	class FileReader : public Reader {
	public:
		FileReader(std::istream& istream);

		void Seek(size_t address);
		size_t Tell();
		void PushCurrentPos();
		void PopLastPos();
		bool HasDataLeft();
		std::string ReadNullTerminatedString();
	private:
		void ReadBytes(void* dest, size_t size);
		std::istream& stream;
		std::streamsize size;
	};

	class MemoryReader : public Reader {
	public:
		MemoryReader(uint8_t* data, size_t size);

		void Seek(size_t address);
		size_t Tell();
		void PushCurrentPos();
		void PopLastPos();
		bool HasDataLeft();
		std::string ReadNullTerminatedString();
	private:
		void ReadBytes(void* dest, size_t size);
		size_t pos = 0;
		size_t size;
		uint8_t* data;
	};
}