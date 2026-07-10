#pragma once
#include <iostream>
#include <stack>
#include <vector>

namespace Wwise {
	class Writer {
	public:
		virtual ~Writer() = default;

		template<typename T>
		void Write(const T& value, size_t size = sizeof(T))
		{
			WriteBytes(reinterpret_cast<const char*>(&value), size);
		}

		template<typename T>
		Writer& operator<<(const T& value)
		{
			Write(value);
			return *this;
		}

		
		virtual long long Tell() = 0;
		virtual void PushCurrentPos() = 0;
		virtual void PopLastPos() = 0;
		virtual void Seek(long long address) = 0;
	protected:
		virtual void WriteBytes(const char* value, size_t size) = 0;
		std::stack<long long> savedPositions;
	};
	
	class FileWriter : public Writer {
	public:
		FileWriter(std::ostream& bnk_stream);

		long long Tell();
		void PushCurrentPos();
		void PopLastPos();
		void Seek(long long address);
	private:
		void WriteBytes(const char* value, size_t size);
		std::ostream& stream;
	};

	class MemoryWriter : public Writer {
	public:
		MemoryWriter(std::vector<uint8_t>& out);

		long long Tell();
		void PushCurrentPos();
		void PopLastPos();
		void Seek(long long address);
	private:
		void WriteBytes(const char* value, size_t size);
		std::vector<uint8_t>& out;
		size_t pos = 0;
	};
}