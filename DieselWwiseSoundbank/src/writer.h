#pragma once
#include <iostream>
#include <stack>

namespace Wwise {
	class Writer {
	public:
		template<typename T>
		void Write(const T& value, size_t size = sizeof(T))
		{
			stream.write(reinterpret_cast<const char*>(&value), size);
		}

		template<typename T>
		Writer& operator<<(const T& value)
		{
			Write(value);
			return *this;
		}

		Writer(std::ostream& bnk_stream);
		long Tell();
		void PushCurrentPos();
		void PopLastPos();
		void Seek(long address);
	private:
		std::ostream& stream;
		std::stack<long> savedPositions;
	};
}