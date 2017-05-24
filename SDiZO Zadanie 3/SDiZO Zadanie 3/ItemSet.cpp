#include "stdafx.h"
#include "ItemSet.h"
#include <random>
using namespace SDZ;

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
	std::mt19937 rng(rd());
	std::uniform_int_distribution<uint> random_weight(1, max_item_weight);
	std::uniform_int_distribution<uint> random_value(1, max_item_value);

	for (uint i = 0; i < item_num; i++)
	{
		uint weight = random_weight(rng);
		uint value = random_value(rng);
		AddItem(weight, value);
	}
	item_set_.shrink_to_fit();
}


ItemSet::~ItemSet()
{
}

ItemSet::iterator SDZ::ItemSet::begin() noexcept
{
	return item_set_.begin();
}

ItemSet::const_iterator SDZ::ItemSet::cbegin() const noexcept
{
	return item_set_.cbegin();
}

ItemSet::iterator SDZ::ItemSet::end() noexcept
{
	return item_set_.end();
}

ItemSet::const_iterator SDZ::ItemSet::cend() const noexcept
{
	return item_set_.cend();
}

ItemSet::reverse_iterator SDZ::ItemSet::rbegin() noexcept
{
	return item_set_.rbegin();
}

ItemSet::reverse_iterator SDZ::ItemSet::rend() noexcept
{
	return item_set_.rend();
}

ItemSet::const_reverse_iterator SDZ::ItemSet::crbegin() const noexcept
{
	return item_set_.crbegin();
}

ItemSet::const_reverse_iterator SDZ::ItemSet::crend() const noexcept
{
	return item_set_.crend();
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
	total_weight_ += weight;
}

void ItemSet::FillRandom(uint item_num, uint max_item_value, uint max_item_weight)
{
	item_set_.clear();
	item_set_.reserve(item_num);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<uint> random_weight(1, max_item_weight);
	std::uniform_int_distribution<uint> random_value(1, max_item_value);

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

void SDZ::ItemSet::DisplaySet()
{
	std::cout.precision(3);
	for (auto item : item_set_)
	{
		std::cout << std::fixed 
		<< "Val: " << item.value_ << "\tWeight: " << item.weight_ 
		<< "\t" << item.value_per_weight_ << std::endl;
	}	
}

void SDZ::ItemSet::DisplayInfo()
{
	std::cout << "\nItem set contains " << GetSize() << " items with total weight of "
		<< total_weight_ << " and total value of " << total_value_ << std::endl;
}

void SDZ::ItemSet::Sort()
{
	QuickSort(0, GetSize() - 1);
}

void SDZ::ItemSet::QuickSort(uint p, uint r)
{
	int q;
	if (p < r)
	{
		q = Partirion(p, r);
		QuickSort(p, q);
		QuickSort(q + 1, r);
	}
}
