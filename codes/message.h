
#ifndef MESSAGE_H
#define MESSAGE_H
#pragma once
#include <string>

using namespace std;
typedef uint64_t OrderRef;

class Message
{
public:
	Message(char type, double time) : type_(type), time_(time) {};
	virtual ~Message() {};
	char Type() const { return type_; }
	double Time() const { return time_; }
	virtual OrderRef Ref() const {return 0;}
	virtual char BSInd() const { return 'N'; }
	virtual int32_t Volume() const { return 0; }
	virtual string Stock() const { return ""; }
	virtual double Price() const { return -1; }
	virtual OrderRef NewRef() const { return 0; }
protected:
	char type_;
	double time_;
};


class MessageA : public Message
	// add order w.o. MPID
{
private:
	OrderRef order_ref_;
	char BS_ind_;
	int32_t volume_;
	string stock_;
	double price_;
public:
	MessageA(double time, OrderRef order_ref, char BS_ind, int32_t volume, string stock, double price) :
		Message('A',time), order_ref_(order_ref), BS_ind_(BS_ind), volume_(volume), stock_(stock), price_(price) {}
	OrderRef Ref() const { return order_ref_; }
	char BSInd() const { return BS_ind_; }
	int32_t Volume() const { return volume_; }
	string Stock() const { return stock_; }
	double Price() const{ return price_; }
};

class MessageF : public MessageA
	// add order w. MPID
{
private:
	string mpid_;
public:
	MessageF(double time, OrderRef order_ref, char BS_ind, int32_t volume, string stock, double price, string mpid) :
		MessageA(time, order_ref, BS_ind, volume, stock, price), mpid_(mpid) {
		type_ = 'F';
	}
	string Mpid() const { return mpid_; }
};

class MessageD : public Message
	// order delete
{
private:
	OrderRef order_ref_;
public:
	MessageD(double time, OrderRef order_ref) : Message('D', time), order_ref_(order_ref) {}
	OrderRef Ref() const { return order_ref_; }
};

class MessageX : public Message
	// order cancel
{
private:
	OrderRef order_ref_;
	int32_t volume_;
public:
	MessageX(double time, OrderRef order_ref, int32_t volume) : Message('X', time), order_ref_(order_ref), volume_(volume) {}
	OrderRef Ref() const { return order_ref_; }
	int32_t Volume() const { return volume_; }
};



class MessageE : public Message
	// order executed
{
private:
	OrderRef order_ref_;
	int32_t volume_;
	OrderRef execution_ref_;
public:
	MessageE(double time, OrderRef order_ref, int32_t volume, OrderRef execution_ref) :
		Message('E', time), order_ref_(order_ref), volume_(volume), execution_ref_(execution_ref) {}
	OrderRef Ref() const { return order_ref_; }
	int32_t Volume() const { return volume_; }
	OrderRef ExecutionRef() const { return execution_ref_; }
};


class MessageC : public MessageE
	// order executed w. price message
{
private:
	char printable_;
	double execution_price_;
public:
	MessageC(double time, OrderRef order_ref, int32_t volume, OrderRef execution_ref, char printable, double execution_price) :
		MessageE(time, order_ref, volume, execution_ref), printable_(printable), execution_price_(execution_price) {
		type_ = 'C';
	}
};


class MessageU : public Message
	// order replace
{
private:
	OrderRef order_ref_;
	OrderRef new_order_ref_;
	int32_t volume_;
	double price_;
public:
	MessageU(double time, OrderRef order_ref, OrderRef new_order_ref, int32_t volume, double price) :
		Message('U', time), order_ref_(order_ref), new_order_ref_(new_order_ref), volume_(volume), price_(price) {};
	~MessageU() {};
	OrderRef Ref() const { return order_ref_; }
	OrderRef NewOrderRef() const { return new_order_ref_; }
	int32_t Volume() const { return volume_; }
	double Price() const { return price_; }
};

#endif


