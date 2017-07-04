#ifndef BOOK_H
#define BOOK_H
#include <map>
#include <list>
#include <string>
#include <iostream>
#include "level.h"

class Book {
private:
	friend class Snapshot;
	std::map<double, Level> bid_levels_;
	std::map<double, Level> ask_levels_;

public:
	const std::string stock_;
	Book(std::string stock) : stock_(stock) {}

	std::map<double, Level>& Levels(char BS_ind) {
		return (BS_ind == 'B') ? bid_levels_ : ask_levels_;
	}

	double MaxBidPrice() const { return bid_levels_.empty() ? -1 : bid_levels_.rbegin()->first;}
	double MinAskPrice() const { return ask_levels_.empty() ? -1 : ask_levels_.begin()->first;}
	int MaxBidVolume() const { return bid_levels_.empty() ? 0 : bid_levels_.rbegin()->second.Volume();}
	int MinAskVolume() const { return ask_levels_.empty() ? 0 : ask_levels_.begin()->second.Volume();}
	OrderRef BidFrontOrder() const { return bid_levels_.empty() ? 0 : bid_levels_.rbegin()->second.FrontOrder();}
	OrderRef AskFrontOrder() const { return ask_levels_.empty() ? 0: ask_levels_.begin()->second.FrontOrder();}

	// add order to limit order book
	// return the iterator in queue
	std::list<OrderRef>::iterator AddOrder(char BS_ind, OrderRef order_ref, int order_volume, double order_price) {
		std::map<double, Level> &levels = Levels(BS_ind);
		return levels[order_price].AddOrder(order_ref, order_volume);
	}

	// Decrease order volume
	// Remove level if volume become 0
	void DecrOrderVol(char BS_ind, std::list<OrderRef>::iterator order_lit, int order_volume, double order_price, int to_decr) {
		std::map<double, Level> &levels = Levels(BS_ind);
		std::map<double, Level>::iterator level_it = levels.find(order_price);
		if (level_it->second.DecrOrderVol(order_lit, order_volume, to_decr) == 0)
			levels.erase(level_it);
	}




	
	
	// Functions return iterators of bidLevels / askLevels
	// Useful to navigate all levels or find details in one price level
	std::map<double, Level>::const_iterator BidLevelsBeginIt() const { return bid_levels_.begin(); }
	std::map<double, Level>::const_iterator BidLevelsEndIt() const { return bid_levels_.end(); }
	std::map<double, Level>::const_iterator BidLevelsFind(double price) const { return bid_levels_.find(price); }
	std::map<double, Level>::const_iterator AskLevelsBeginIt() const { return ask_levels_.begin(); }
	std::map<double, Level>::const_iterator AskLevelsEndIt() const { return ask_levels_.end(); }
	std::map<double, Level>::const_iterator AskLevelsFind(double price) const { return ask_levels_.find(price); }
};



#endif