#ifndef TIME_SERIES_WRITER
#define TIME_SERIES_WRITER
#include <string>
#include <fstream>
#include "snapshot.h"

typedef double Time;
enum Label;

// TSWriter classes have protected / private construct
// Constructors can only be called using the method Output::NewTSWriter

class TSWriter {
public:
	virtual ~TSWriter() { out_.close(); }
	friend struct Output;
protected:
	std::string stock_;
	Label label_;
	std::ofstream out_;
	Time last_update_time_;
	TSWriter(std::string stock, Label label, std::string to_write_file) : stock_(stock), label_(label), out_(to_write_file), last_update_time_(-1) {}
};

template <typename T>
class TypedTSWriter : public TSWriter {
public:
	~TypedTSWriter() {}
	friend struct Output;
private:
	T value_;
	TypedTSWriter(std::string stock, Label label, std::string to_write_file) : TSWriter(stock, label, to_write_file) {}
	void Receive(Time time, const T& new_value) {
		if (last_update_time_ < 0 || new_value != value_) {
			last_update_time_ = time;
			value_ = new_value;
			out_ << std::to_string(last_update_time_) << " " << std::to_string(value_) << "\n";
		}
	}
};




#endif
