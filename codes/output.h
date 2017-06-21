#ifndef OUTPUT_H
#define OUTPUT_H
#include <string>
#include "time_series_writer.h"

enum Label {
	MAX_BID,
	MIN_ASK,
	MAX_BID_VOL,
	MIN_ASK_VOL
};

struct Output {

	static std::string LabelStr(const Label& label);
	static void Update(TSWriter* writer, const Snapshot* snapshot);
	static TSWriter* NewTSWriter(std::string stock, Label label, std::string towrite_file);

};



#endif
