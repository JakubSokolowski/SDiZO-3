#include "stdafx.h"
#include "ItemSet.h"
#include <random>
using namespace SDZ;

ItemSet::ItemSet()
	: total_value_(0)
	, total_weight_(0)
{
}

ItemSet::ItemSet(const ItemSet & other)
	: item_set_{other.item_set_}
	, total_value_{other.total_value_}
	, total_weight_{other.total_weight_}
{}

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

void SDZ::ItemSet::AddToWeight(uint index, uint weight)
{
	item_set_.at(index).AddWeight(weight);
	total_weight_ += weight;
}

void ItemSet::FillRandom(uint item_num, uint max_item_value, uint max_item_weight)
{
	item_set_.clear();
	total_value_ = 0;
	total_weight_ = 0;
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

//Increases the total weight of items, so that it reaches min_weight
void ItemSet::IncreaseTotalWeight(uint min_weight)
{
	//If the min_weight (125% of knapsack capacity) is smaller than total, return
	if (min_weight < total_weight_)
		return;

	uint missing_weight = min_weight - total_weight_;
	//Evenly distribute the missing_weight in item_set
	uint value = static_cast<uint>(missing_weight / item_set_.size()) + 1;

	for (uint i = 0; i < GetSize(); i++)
	{
		if (total_weight_ > min_weight)
			break;
		AddToWeight(i, value);			
	}
}

Item SDZ::ItemSet::At(uint index)
{
	return item_set_.at(index);
}

void SDZ::ItemSet::DisplaySet()
{
	std::cout.precision(3);
	for (auto item : item_set_)
	{
		std::cout << std::fixed 
		<< "War: " << item.value_ << "  \tWaga: " << item.weight_ 
		<< "     \t" << item.value_per_weight_ << std::endl;
	}	
}

void SDZ::ItemSet::DisplayInfo()
{
	std::cout << "\nZestaw przedmiotow zawiera " << GetSize() << " przedmiotow z calkowita waga "
		<< total_weight_ << " i calkowita wartoscia " << total_value_ << std::endl;
}

void SDZ::ItemSet::Sort()
{
	QuickSort(0, GetSize() - 1);
}

void SDZ::ItemSet::WriteToFile(uint knapsack_capacity,std::string filepath)
{
	if (knapsack_capacity > total_weight_)
		IncreaseTotalWeight(static_cast<uint>(knapsack_capacity*1.3));

	std::ofstream file(filepath);

	//Write the capacity and number of items
	file << knapsack_capacity <<" "<< GetSize() << "\n";

	//Write all the items
	for (auto item : item_set_)
	{
		file << item.weight_ << " " << item.value_ << "\n";
	}

}

void SDZ::ItemSet::ReadFromFile(std::string filepath)
{
	std::fstream file;
	file.open(filepath, std::ios_base::in);

	if (!file)
		throw std::runtime_error("Could not open the file");

	//Clear the item other
	item_set_.clear();

	uint num_items, capacity;
	file >> capacity >> num_items;
	
	item_set_.reserve(num_items);

	uint weight, value;

	while (file >> weight >> value)	
		AddItem(weight, value);	

	file.close();
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

inline uint SDZ::ItemSet::Partirion(uint p, uint r)
{
	Item x = item_set_.at(p);

	Item w;

	uint i = p, j = r;

	while (true)
	{
		while (item_set_.at(j) > x)
			j--;
		while (item_set_.at(i) < x)
			i++;
		if (i < j)
		{
			//Swap the elements
			w = item_set_.at(i);
			item_set_.at(i) = item_set_.at(j);
			item_set_.at(j) = w;
			i++;
			j--;
		}
		else
			return j;
	}
}
