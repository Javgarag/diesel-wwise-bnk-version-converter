#include "writer.h"

namespace Wwise {
	FileWriter::FileWriter(std::ostream& bnk_stream)
		: stream(bnk_stream)
	{}

	long long FileWriter::Tell() {
		return stream.tellp();
	};

	void FileWriter::PushCurrentPos() {
		savedPositions.push(stream.tellp());
	};

	void FileWriter::PopLastPos() {
		stream.seekp(savedPositions.top());
		savedPositions.pop();
	};

	void FileWriter::Seek(long long address) {
		stream.seekp(address);
	};

	void FileWriter::WriteBytes(const char* value, size_t size) {
		stream.write(value, size);
	}

	MemoryWriter::MemoryWriter(std::vector<uint8_t>& out)
		: out(out)
	{}

	long long MemoryWriter::Tell() {
		return pos;
	};

	void MemoryWriter::PushCurrentPos() {
		savedPositions.push(pos);
	};

	void MemoryWriter::PopLastPos() {
		pos = savedPositions.top();
		savedPositions.pop();
	};

	void MemoryWriter::Seek(long long address) {
		pos = address;
	};

	void MemoryWriter::WriteBytes(const char* value, size_t size) {
		if (pos + size > out.size())
			out.resize(pos + size);

		memcpy(out.data() + pos, value, size);
		pos += size;
	}
}