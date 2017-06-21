#ifndef ORDER_H
#define ORDER_H
#pragma once
#define NDEBUG
#include <string>
#include <assert.h>
#include <memory>
using namespace std;

class Order
{
private:
	const char BS_ind_;
	int volume_;
	const string stock_;
	const double price_;
	const list<OrderRef>::iterator lit_;

public:
	Order(char BSInd, int volume, const string& stock, double price, const list<OrderRef>::iterator& listIt) :
		BS_ind_(BSInd), volume_(volume), stock_(stock), price_(price), lit_(listIt) { assert(BS_ind_ == 'B' || BS_ind_ == 'S'); };

	char BSInd() const { 
		return BS_ind_; 
	}

	int Volume() const { 
		return volume_; 
	}

	string Stock() const { 
		return stock_; 
	}

	double Price() const { 
		return price_; 
	}

	list<OrderRef>::iterator ListIt() const {
		return lit_;
	}

	void DecreaseVolume(int to_decr) {
		volume_ -= to_decr;
	}
};

#endif
