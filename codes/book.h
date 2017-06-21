#ifndef BOOK_H
#define BOOK_H
#include <map>
#include <list>
#include <memory>
#include <assert.h>
#include "level.h"

using namespace std;

class Book {
private: 
	map<double, unique_ptr<Level>> bid_levels_;
	map<double, unique_ptr<Level>> ask_levels_;

public:

	double MaxBidPrice() const { return bid_levels_.empty() ? -1 : bid_levels_.rbegin()->first;}
	double MinAskPrice() const { return ask_levels_.empty() ? -1 : ask_levels_.begin()->first;}
	int MaxBidVolume() const { return bid_levels_.empty() ? 0 : bid_levels_.rbegin()->second->Volume();}
	int MinAskVolume() const { return ask_levels_.empty() ? 0 : ask_levels_.begin()->second->Volume();}
	OrderRef BidFrontOrder() const { return bid_levels_.empty() ? 0 : bid_levels_.rbegin()->second->FrontOrder();}
	OrderRef AskFrontOrder() const { return ask_levels_.empty() ? 0: ask_levels_.begin()->second->FrontOrder();}

	// Functions return iterators of bidLevels / askLevels
	// Useful to navigate all levels or find details in one price level
	map<double, unique_ptr<Level>>::const_iterator BidLevelsBeginIt() const { return bid_levels_.begin(); }
	map<double, unique_ptr<Level>>::const_iterator BidLevelsEndIt() const { return bid_levels_.end(); }
	map<double, unique_ptr<Level>>::const_iterator BidLevelsFind(double price) const { return bid_levels_.find(price); }
	map<double, unique_ptr<Level>>::const_iterator AskLevelsBeginIt() const { return ask_levels_.begin(); }
	map<double, unique_ptr<Level>>::const_iterator AskLevelsEndIt() const { return ask_levels_.end(); }
	map<double, unique_ptr<Level>>::const_iterator AskLevelsFind(double price) const { return ask_levels_.find(price); }

	// add order to limit order book
	// return the iterator in queue
	list<OrderRef>::iterator AddOrder(char BS_ind, OrderRef order_ref, int order_volume, double order_price) {
		assert(BS_ind == 'B' || BS_ind == 'S');
		map<double, unique_ptr<Level>> &levels = (BS_ind == 'B') ? bid_levels_ : ask_levels_;
		auto level_it = levels.find(order_price);
		if (level_it == levels.end()) 
			level_it = levels.insert({ order_price, unique_ptr<Level>(new Level()) }).first;
		return level_it->second->AddOrder(order_ref, order_volume);
	}


	// Decrease order volume
	// Remove order if volume become 0
	// Remove level if volume become 0
	void DecreaseOrderVolume(char BSInd, list<OrderRef>::iterator order_lit, int order_volume, double order_price, int to_decr) {
		assert(order_volume >= to_decr);
		map<double, unique_ptr<Level>> &levels = (BSInd == 'B') ? bid_levels_ : ask_levels_;
		auto itLevel = levels.find(order_price);
		itLevel->second->DecreaseVolume(to_decr);
		if (order_volume == to_decr) {
			itLevel->second->EraseOrderIterator(order_lit);
			if (!itLevel->second->Volume())
				levels.erase(itLevel);
		}
	}

};



#endif