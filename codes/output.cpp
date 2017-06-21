#include "output.h"

std::string Output::LabelStr(const Label& label) {
	switch (label) {
	case MAX_BID: return "MaxBid";
	case MIN_ASK: return "MinAsk";
	case MAX_BID_VOL: return "MaxBidVol";
	case MIN_ASK_VOL: return "MinAskVol";
	}
	return "";
}

TSWriter* Output::NewTSWriter(std::string stock, Label label, std::string towrite_file) {
	switch (label) {
	case MAX_BID:
	case MIN_ASK: return new TypedTSWriter<double>(stock, label, towrite_file);
	case MAX_BID_VOL:
	case MIN_ASK_VOL: return new TypedTSWriter<int>(stock, label, towrite_file);
	}
	return nullptr;
}

void Output::Update(TSWriter* writer, const Snapshot* snapshot) {
	switch (writer->label_) {
	case MAX_BID: {
		auto val = snapshot->MaxBidPrice(writer->stock_);
		if (val >= 0) dynamic_cast<TypedTSWriter<double>*>(writer)->Receive(snapshot->LastUpdateTime(), val);
		break;
	}
	case MIN_ASK: {
		auto val = snapshot->MinAskPrice(writer->stock_);
		if (val >= 0) dynamic_cast<TypedTSWriter<double>*>(writer)->Receive(snapshot->LastUpdateTime(), val); 
		break;
	}
	case MAX_BID_VOL: {
		auto val = snapshot->MaxBidVolume(writer->stock_);
		if (val > 0) dynamic_cast<TypedTSWriter<int>*>(writer)->Receive(snapshot->LastUpdateTime(), val);
		break;
	}
	case MIN_ASK_VOL: {
		auto val = snapshot->MinAskVolume(writer->stock_);
		if (val > 0) dynamic_cast<TypedTSWriter<int>*>(writer)->Receive(snapshot->LastUpdateTime(), val);
		break;
	}
	}
}