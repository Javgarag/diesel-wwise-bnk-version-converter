#include "writer.h"

namespace Wwise {
	Writer::Writer(std::ostream& bnk_stream)
		: stream(bnk_stream)
	{}

	long Writer::Tell() {
		return stream.tellp();
	};

	void Writer::PushCurrentPos() {
		savedPositions.push(stream.tellp());
	};

	void Writer::PopLastPos() {
		stream.seekp(savedPositions.top());
		savedPositions.pop();
	};

	void Writer::Seek(long address) {
		stream.seekp(address);
	};
}