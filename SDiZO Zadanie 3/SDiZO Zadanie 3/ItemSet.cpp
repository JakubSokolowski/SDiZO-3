#include "stdafx.h"
#include "ItemSet.h"


ItemSet::ItemSet()
{
}

ItemSet::ItemSet(const ItemSet & set)
{
	item_set_ = set.item_set_;
	total_value_ = set.total_value_;
	total_weight_ = set.total_weight_;
}

ItemSet::ItemSet(uint item_num, uint max_item_value, uint max_item_weight)
{
	item_set_.reserve(item_num);

	std::random_device rd;
	std::mt19937 rng(rd);
	std::uniform_int_distribution<uint> random_weight(0, max_item_weight);
	std::uniform_int_distribution<uint> random_value(0, max_item_value);

	for (uint i = 0; i < item_num; i++)
	{
		uint weight = random_weight(rng);
		uint value = random_value(rng);
		AddItem(weight, value);
	}
}


ItemSet::~ItemSet()
{
}

ItemSet & ItemSet::operator=(const ItemSet & set)
{
	item_set_ = set.item_set_;
	total_value_ = set.total_value_;
	total_weight_ = set.total_weight_;
	return *this;
}

ItemSet & ItemSet::operator=(ItemSet && set)
{
	item_set_ = std::move(set.item_set_);
	total_value_ = set.total_value_;
	total_weight_ = set.total_weight_;
	return *this;
}

void ItemSet::AddItem(Item & item)
{
	item_set_.push_back(item);
	total_value_ += item.value_;
	total_weight_ += item.weight_;
}

void ItemSet::AddItem(uint weight, uint value)
{
	Item item = Item(weight, value);
	item_set_.push_back(item);
	total_value_ += value;
	total_weight_ += 
}

void ItemSet::FillRandom(uint item_num, uint max_item_value, uint max_item_weight)
{
	item_set_.clear();
	item_set_.reserve(item_num);

	std::random_device rd;
	std::mt19937 rng(rd);
	std::uniform_int_distribution<uint> random_weight(0, max_item_weight);
	std::uniform_int_distribution<uint> random_value(0, max_item_value);

	for (uint i = 0; i < item_num; i++)
	{
		uint weight = random_weight(rng);
		uint value = random_value(rng);
		AddItem(weight, value);
	}
}

//Increases the total value of items, so that it reaches min_value
void ItemSet::IncreaseTotalValue(uint min_value)
{
	//If the min_value (125% of knapsack capacity) is smaller than total, return
	if (min_value < total_value_)
		return;

	uint missing_value = min_value - total_value_;

	//Evenly distribute the missing_value in item_set
	uint value = static_cast<uint>(missing_value / item_set_.size());

	for (auto item : item_set_)
	{
		if (total_value_ > missing_value)
			break;
		item.AddValue(value);
	}
}
