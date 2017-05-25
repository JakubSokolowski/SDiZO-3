#include "stdafx.h"
#include "KnapSack.h"

using namespace SDZ;

inline uint Max(uint a, uint b)
{
	return a > b ? a : b;
}

KnapSack::KnapSack()
{}

SDZ::KnapSack::KnapSack(uint capacity)
{
	capacity_ = capacity;
	/*cost_table_ = new uint*[capacity];
	for (uint i = 0; i < capacity; i++)
	{
		cost_table_[i] = new uint[capacity_];
	}*/
}


KnapSack::KnapSack(std::string filepath)
{
	std::fstream file;
	file.open(filepath, std::ios_base::in);

	if (!file)
		throw std::runtime_error("Could not open the file");

}


KnapSack::~KnapSack()
{
}

void SDZ::KnapSack::FillBruteForce(ItemSet &set)
{
	//Empty the knapsack
	item_set_ = ItemSet();

	//Vector that shows which elements are included
	DTS::Vector<bool> included(set.GetSize(),false);
	//Calculate the value, fill the vector
	max_value_ = FillRecursive(set, included, set.GetSize()-1 , capacity_);

	//Add the items to the backpack
	for (auto it = 0; it < included.size(); it++)
	{
		if (included.at(it))
			item_set_.AddItem(set.At(it));
	}
}

void SDZ::KnapSack::Display()
{
	item_set_.DisplayInfo();
	item_set_.DisplaySet();	
}

void SDZ::KnapSack::Sort()
{
	item_set_.Sort();
}

uint SDZ::KnapSack::FillRecursive(ItemSet & set, DTS::Vector<bool> & included, uint item_count, uint capacity)
{
	//Base case
	if (item_count== 0 || capacity == 0)
	{
		return 0;
	}

	Item item = set.At(item_count);
	//If the weight of the item is more than capacity, the item cannot be included in optimal solution
	if (item.weight_ > capacity)
	{
		included.at(item_count) = false;
		return FillRecursive(set, included, item_count-1, capacity);	
	}
		
	else
	{
		uint total_with_item = FillRecursive(set, included, item_count - 1, capacity - item.weight_) + item.value_;
		DTS::Vector<bool> arr_with = included;
		uint total_without_item = FillRecursive(set, included, item_count - 1, capacity);

		
		if (total_with_item > total_without_item)
		{
			included = arr_with;
			included.at(item_count) = true;
			return total_with_item;
		}
		else
		{
			included.at(item_count) = false;
			return total_without_item;
		}
	}
}




