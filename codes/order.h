#ifndef ORDER_H
#define ORDER_H
#pragma once
#define NDEBUG
#include <string>
#include <assert.h>
#include <memory>
#include "book.h"

class Order
{
	int volume_;

public:
	const char BS_ind_;
	const double price_;
	Book* const pbook_;
	const std::list<OrderRef>::iterator lit_;

	Order(char BSInd, int volume, double price, Book* pbook, std::list<OrderRef>::iterator& lit) :
		BS_ind_(BSInd), volume_(volume), price_(price), pbook_(pbook), lit_(lit) {}

	int Volume() const { 
		return volume_; 
	}

	std::string Stock() const {
		return pbook_->stock_; 
	}

	// decrease and return volume_
	int DecrVol(int to_decr) {
		return volume_ -= to_decr;
	}
};

#endif
