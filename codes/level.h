#ifndef LEVEL_H
#define LEVEL_H
#include <list>

using namespace std;
typedef uint64_t OrderRef;

class Level {
private:
	list<OrderRef> orders_;
	int volume_;

public:
	Level() : orders_(list<OrderRef>()), volume_(0) {};

	int Volume() const { return volume_; }
    OrderRef FrontOrder() const {return orders_.front();}
    int NumOrders() const {	return orders_.size();}

    // get const_iterators
	// get all orderRefs in the list
	list<OrderRef>::const_iterator BeginIt() const { return orders_.cbegin(); }
	list<OrderRef>::const_iterator Level::EndIt() const { return orders_.cend(); }

	// Add order, return iterator in the list
	list<OrderRef>::iterator AddOrder(OrderRef order_ref, int order_volume) {
		volume_ += order_volume;
		return orders_.insert(orders_.end(), order_ref);
	}

	// Remove the order from list
	// Decrease the level's volume as well 
	list<OrderRef>::iterator EraseOrderIterator(list<OrderRef>::iterator order_lit) {return orders_.erase(order_lit);}

	void DecreaseVolume(int to_dec) {volume_ -= to_dec;}
};






#endif
