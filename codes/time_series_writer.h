#ifndef TIME_SERIES_WRITER
#define TIME_SERIES_WRITER
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
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
	bool observe_dup_value_;
	TSWriter(std::string stock, Label label, std::string to_write_file, bool observe_dup_value) : stock_(stock), label_(label), out_(to_write_file), last_update_time_(-1), observe_dup_value_(observe_dup_value) {}
};

template <typename T>
class TypedTSWriter : public TSWriter {
public:
	~TypedTSWriter() {}
	friend struct Output;
private:
	T value_;
	TypedTSWriter(std::string stock, Label label, std::string to_write_file, bool observe_dup_value = true) : TSWriter(stock, label, to_write_file, observe_dup_value) {}
	void Receive(Time time, const T& new_value) {
		if (observe_dup_value_ || last_update_time_ < 0 || new_value != value_) {
			last_update_time_ = time;
			value_ = new_value;
			out_ << boost::lexical_cast<std::string>(last_update_time_) << " " << std::to_string(value_) << "\n";
		}
	}
};




#endif
