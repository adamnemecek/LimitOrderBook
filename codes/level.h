#ifndef LEVEL_H
#define LEVEL_H
#include <list>

typedef uint64_t OrderRef;

class Order;

class Level {
private:
	std::list<OrderRef> orders_;
	int volume_;

public:
	Level() : volume_(0) {};

	int Volume() const { return volume_; }
    OrderRef FrontOrder() const {return orders_.front();}
    int NumOrders() const {	return orders_.size();}

	// Add order
	// Return list::iterator
	std::list<OrderRef>::iterator AddOrder(OrderRef order_ref, int order_volume) {
		volume_ += order_volume;
		return orders_.insert(orders_.end(), order_ref);
	}

	// Decrease order volume
	// return volume_
	int DecrOrderVol(std::list<OrderRef>::iterator lit, int order_vol, int to_decr) {
		if (order_vol == to_decr) orders_.erase(lit);
		return volume_ -= to_decr;
	}






	// get const_iterators
	// get all orderRefs in the list
	std::list<OrderRef>::const_iterator BeginIt() const { return orders_.cbegin(); }
	std::list<OrderRef>::const_iterator Level::EndIt() const { return orders_.cend(); }
};






#endif
