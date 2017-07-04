#include "binary_file_parser.h"

void BinaryFileParser::Read(int k) {
	if (k > 0) {
		auto d = div(k, kBufferSize);
		for (int i = 0; i < d.quot; i++) {
			bin_.read(buffer_, kBufferSize);
		}
		bin_.read(buffer_, d.rem);
	}
}

int32_t BinaryFileParser::GetInt32() const {
	int32_t ret = 0;
	for (int i = 0; i < 4; ++i) {
		ret = (ret << 8) + (unsigned char)buffer_[i];
	}
	return ret;
}

uint64_t BinaryFileParser::GetUint64() const {
	uint64_t ret = 0;
	for (int i = 0; i < 8; ++i) {
		ret = (ret << 8) + (unsigned char)buffer_[i];
	}
	return ret;
}

void BinaryFileParser::UpdateSecond() {
    bin_.read(buffer_, 4);
	second_ = GetInt32();
}

double BinaryFileParser::ReadTime() {
	bin_.read(buffer_, 4);
	return (double)second_ + (double)GetInt32() / kTimeStampScale;
}

OrderRef BinaryFileParser::ReadRef() {
	bin_.read(buffer_, 8);
	return GetUint64();
}

char BinaryFileParser::ReadChar() {
	bin_.read(buffer_, 1);
	return *buffer_;
}

int32_t BinaryFileParser::ReadVolume() {
	bin_.read(buffer_, 4);
	return GetInt32();
}

std::string BinaryFileParser::ReadStock() {
	bin_.read(buffer_, 8);
	for (size_t i = 0; i < kBufferSize; i++) 
		if (isspace(buffer_[i])) {
		    buffer_[i] = '\0'; break;
	    }
	buffer_[8] = '\0';
	return std::string(buffer_);
}

double BinaryFileParser::ReadPrice() {
	bin_.read(buffer_, 4);
	return (double)GetInt32() / kPriceScale;
}

std::string BinaryFileParser::ReadMpid() {
	bin_.read(buffer_, 4);
	buffer_[4] = '\0';
	return std::string(buffer_);
}

std::unique_ptr<Message> BinaryFileParser::ReadMessage() {
	bin_.read(buffer_, 1);
	unsigned char messageLength = (unsigned char)ReadChar();
	char messageType = ReadChar();
	messageLength--;
	switch (messageType) {
	case 'T': 
		UpdateSecond(); break;
	case 'A': {
		double time = ReadTime();
		OrderRef orderRef = ReadRef();
		char BSInd = ReadChar();
		int32_t volume = ReadVolume();
		std::string stock = ReadStock();
		double price = ReadPrice();
		return std::unique_ptr<Message>(new MessageA(time, orderRef, BSInd, volume, stock, price));
	}
	case 'F': {
		double time = ReadTime();
		OrderRef orderRef = ReadRef();
		char BSInd = ReadChar();
		int32_t volume = ReadVolume();
		std::string stock = ReadStock();
		double price = ReadPrice();
		std::string mpid = ReadMpid();
		return std::unique_ptr<Message>(new MessageF(time, orderRef, BSInd, volume, stock, price, mpid));
	}
	case 'E': {
		double time = ReadTime();;
		OrderRef orderRef = ReadRef();
		int32_t volume = ReadVolume();
		OrderRef executionRef = ReadRef();
		return std::unique_ptr<Message>(new MessageE(time, orderRef, volume, executionRef));
	}
	case 'C': {
		double time = ReadTime();;
		OrderRef orderRef = ReadRef();
		int32_t volume = ReadVolume();
		OrderRef executionRef = ReadRef();
		char printable = ReadChar();
		double executionPrice = ReadPrice();
		return std::unique_ptr<Message>(new MessageC(time, orderRef, volume, executionRef, printable, executionPrice));
	}
	case 'X': {
		double time = ReadTime();;
		OrderRef orderRef = ReadRef();
		int32_t volume = ReadVolume();
		return std::unique_ptr<Message>(new MessageX(time, orderRef, volume));
	}
	case 'D': {
		double time = ReadTime();;
		OrderRef orderRef = ReadRef();
		return std::unique_ptr<Message>(new MessageD(time, orderRef));
	}
	case 'U': {
		double time = ReadTime();;
		OrderRef orderRef = ReadRef();
		OrderRef newOrderRef = ReadRef();
		int32_t volume = ReadVolume();
		double price = ReadPrice();
		return std::unique_ptr<Message>(new MessageU(time, orderRef, newOrderRef, volume, price));
	}
	default: Read(messageLength);
	}
	return std::unique_ptr<Message>(nullptr);
}

