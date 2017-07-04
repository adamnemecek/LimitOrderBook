#ifndef SNAPSHOT_H
#define SNAPSHOT_H
#include <unordered_map>
#include <initializer_list>
#include <iostream>
#include "book.h"
#include "order.h"
#include "message.h"


class Snapshot {

public:
	Snapshot() : last_update_time_(-1) {}
	Snapshot(std::initializer_list<std::string> stocks) : Snapshot() {
		for (std::string stock : stocks) {
			books_.insert({ stock, Book(stock) });
		}
	}

	double LastUpdateTime() const { return last_update_time_; }

	std::unordered_map<std::string, Book>::const_iterator BooksBeginIt() { return books_.begin(); }
	std::unordered_map<std::string, Book>::const_iterator BooksEndIt() { return books_.end(); }

	double MaxBidPrice(const std::string& stock) const {
		auto it = books_.find(stock);
		return (it == books_.end()) ? -1 : it->second.MaxBidPrice();
	}
	double MinAskPrice(const std::string& stock) const { 
		auto it = books_.find(stock);
		return (it == books_.end()) ? -1 : it->second.MinAskPrice();
	}
	int MaxBidVolume(const std::string& stock) const { 
		auto it = books_.find(stock);
		return (it == books_.end()) ? -1 : it->second.MaxBidVolume();
	}
	int MinAskVolume(const std::string& stock) const { 
		auto it = books_.find(stock);
		return (it == books_.end()) ? -1 : it->second.MinAskVolume();
	}


	// return stock if input is relevant
	// return "" otherwise 
	std::string Snapshot::ProcessMessage(const Message* pmessage) {
		SetTime(pmessage->Time());
		char messageType = pmessage->Type();
		switch (messageType) {
		case 'A': case 'F':
			return AddOrder(pmessage->Ref(), pmessage->BSInd(), pmessage->Volume(), pmessage->Stock(), pmessage->Price());
		case 'E': case 'C': case 'X':
			return DecrOrderVol(pmessage->Ref(), pmessage->Volume());
		case 'D':
			return DeleteOrder(pmessage->Ref());
		case 'U':
			return ReplaceOrder(pmessage->Ref(), pmessage->NewRef(), pmessage->Volume(), pmessage->Price());
		}
		return "";
	}

	std::string AddOrder(OrderRef order_ref, char BS_ind, int volume, const std::string& stock, double price) {
		auto it = books_.find(stock);
		if (it == books_.end()) return "";
		orders_.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(order_ref),
			std::forward_as_tuple(BS_ind, volume, price, &(it->second), it->second.AddOrder(BS_ind, order_ref, volume, price)));
		//if (BS_ind == 'B' && price == 170.79) 
		//	std::cout << last_update_time_ << " add " << order_ref << " " << volume << std::endl;
		////if (BS_ind == 'B')
		////	std::cout << BS_ind << price << "(" << MaxBidPrice(stock) << ")" << std::endl;
		////else
		////	std::cout << "                                       " << BS_ind << price << "(" << MinAskPrice(stock) << ")" << std::endl;
		return stock;
	}

	std::string DeleteOrder(OrderRef order_ref) {
		return DeleteOrder(orders_.find(order_ref));
	}

	std::string DecrOrderVol(OrderRef order_ref, int to_decr) {
		return DecrOrderVol(orders_.find(order_ref), to_decr);
	}

	std::string ReplaceOrder(OrderRef order_ref, OrderRef new_order_ref, int new_order_volume, double new_order_price) {
		auto it = orders_.find(order_ref);
		if (it == orders_.end()) return "";

		char BSInd = it->second.BS_ind_;
		std::string stock = it->second.Stock();
		DeleteOrder(order_ref);

		AddOrder(new_order_ref, BSInd, new_order_volume, stock, new_order_price);
		return stock;
	}

private:
	double last_update_time_;
	std::unordered_map<OrderRef, Order> orders_;
	std::unordered_map<std::string, Book> books_;

	void SetTime(double new_time) { last_update_time_ = new_time; }


	// return stock if input is relevant
	// return "" otherwise 
	std::string DeleteOrder(std::unordered_map<OrderRef, Order>::iterator it) {
		if (it == orders_.end()) return "";

		return DecrOrderVol(it, it->second.Volume());
	}

	std::string DecrOrderVol(std::unordered_map<OrderRef, Order>::iterator it, int to_decr) {
		if (it == orders_.end()) return "";

		Order& order = it->second;
		std::string stock = order.Stock();
		//if (order.BS_ind_ == 'B' && order.price_ == 170.79)
		//	std::cout << last_update_time_ << " " << it->first << " " << order.Volume() << " " << to_decr << " " << order.Volume() - to_decr << std::endl;
		order.pbook_->DecrOrderVol(order.BS_ind_, order.lit_, order.Volume(), order.price_, to_decr);
		if (order.DecrVol(to_decr) == 0) orders_.erase(it);
		return stock;
	}



};


#endif
