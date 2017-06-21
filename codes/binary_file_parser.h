#ifndef BINARY_FILE_PARSER_H
#define BINARY_FILE_PARSER_H
#include <string>
#include <fstream>
#include <memory>
#include "message.h"

class BinaryFileParser
{
public:
	BinaryFileParser(const std::string file): buffer_(new char[kBufferSize]) {
		bin_.open(file, std::ios::in | std::ios::binary);
	}
	~BinaryFileParser() {
		delete[] buffer_;
		bin_.close();
	}
	bool IsEnd() { return bin_.eof(); }
	std::unique_ptr<Message> ReadMessage();
	int GetSecond() const { return second_; }

private:
	const size_t kBufferSize = 9;
	const double kTimeStampScale = 1000000000;
	const double kPriceScale = 10000;

	std::ifstream bin_;
	int second_;
	char* buffer_;

	int32_t GetInt32() const;
	uint64_t GetUint64() const;

	void Read(int);
	void UpdateSecond();
	double ReadTime();
	OrderRef ReadRef();
	char ReadChar();
	int32_t ReadVolume();
	std::string ReadStock();
	double ReadPrice();
	std::string ReadMpid();
};

#endif