#ifndef LEVEL_H
#define LEVEL_H
#include <list>

typedef uint64_t OrderRef;

class Level {
private:
	std::list<OrderRef> orders_;
	int volume_;

public:
	Level() : orders_(std::list<OrderRef>()), volume_(0) {};

	int Volume() const { return volume_; }
    OrderRef FrontOrder() const {return orders_.front();}
    int NumOrders() const {	return orders_.size();}

    // get const_iterators
	// get all orderRefs in the list
	std::list<OrderRef>::const_iterator BeginIt() const { return orders_.cbegin(); }
	std::list<OrderRef>::const_iterator Level::EndIt() const { return orders_.cend(); }

	// Add order, return iterator in the list
	std::list<OrderRef>::iterator AddOrder(OrderRef order_ref, int order_volume) {
		volume_ += order_volume;
		return orders_.insert(orders_.end(), order_ref);
	}

	// Remove the order from list
	// Decrease the level's volume as well 
	std::list<OrderRef>::iterator EraseOrderIterator(std::list<OrderRef>::iterator order_lit) {return orders_.erase(order_lit);}

	void DecreaseVolume(int to_dec) {volume_ -= to_dec;}
};






#endif
