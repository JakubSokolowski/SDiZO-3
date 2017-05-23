#pragma once

#include <random>
#include "Vector.h"



typedef unsigned int uint;

namespace SDZ {
	//Struct representing items in knapsack
	class Item
	{
	public:

		uint weight_;
		uint value_;
		double value_per_weight_;

		Item()
			: weight_(0)
			, value_(0)
			, value_per_weight_(0)
		{	}
		Item(uint weight, uint value)
			: weight_(weight)
			, value_(value)
			, value_per_weight_(value / weight)
		{}
		Item(const Item &item)
			: weight_(item.weight_)
			, value_(item.value_)
			, value_per_weight_(item.value_per_weight_)
		{}
		void AddValue(uint value)
		{
			value_ += value;
			value_per_weight_ = value_ / weight_;
		}
		bool operator > (const Item &val) const
		{
			return value_per_weight_ > val.value_per_weight_;
		}
	};

	//Set of items that is held by the knapsack
	class ItemSet
	{
	public:

		//Constructors

		ItemSet();
		ItemSet(const ItemSet &set);
		ItemSet(uint item_num, uint max_item_value = 10, uint max_item_weight = 10);
		~ItemSet();

		//Operators

		ItemSet & operator = (const ItemSet& set);
		ItemSet & operator = (ItemSet && set);

		//Adding

		void AddItem(Item &item);
		void AddItem(uint weight, uint value);

		void FillRandom(uint item_num, uint max_item_value = 10, uint max_item_weight = 10);
		void IncreaseTotalValue(uint min_value);

	private:

		DTS::Vector<Item> item_set_;
		uint total_value_ = 0;
		uint total_weight_ = 0;
	};
}

