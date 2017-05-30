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


void SDZ::KnapSack::Display()
{
	std::cout << "\nKnapsack has capacity of " << capacity_;
	std::cout << "\nMax item value " << max_value_;
	item_set_.DisplayInfo();
	item_set_.DisplaySet();
}

void SDZ::KnapSack::Sort()
{
	item_set_.Sort();
}


void SDZ::KnapSack::FillKnapsack(ItemSet & set, Heuristic h)
{
	//Empty the knapsack
	item_set_ = ItemSet();

	//Vector that shows which elements are included
	DTS::Vector<bool> included(set.GetSize(), false);

	switch (h)
	{
		case BRUTEFORCE:
		{
			max_value_ = FillBruteForce(set, included, set.GetSize()-1, capacity_);
		}break;
		
		case GREEDY:
		{
			max_value_ = FillGreedy(set, set.GetSize()-1, capacity_);
		}break;
		
		case DYNAMIC:
		{
			max_value_ = FillDynamic(set, included, set.GetSize()-1, capacity_);			
		}break;
	}

	//Add the items to the backpack
	for (uint it = 0; it < included.size(); it++)
	{
		if (included.at(it))
			item_set_.AddItem(set.At(it));
	}
}


uint SDZ::KnapSack::FillBruteForce(ItemSet & set, DTS::Vector<bool> & included, uint item_count, uint capacity)
{
	//Base case
	if (item_count == 0 || capacity == 0)	
		return 0;

	Item item = set.At(item_count - 1);
	//If the weight of the item is more than capacity, the item cannot be included in optimal solution
	if (item.weight_ > capacity)
	{
		included.at(item_count - 1) = false;
		return FillBruteForce(set, included, item_count-1, capacity);	
	}
		
	else
	{
		uint total_with_item = FillBruteForce(set, included, item_count - 1, capacity - item.weight_) + item.value_;
		DTS::Vector<bool> arr_with = included;
		uint total_without_item = FillBruteForce(set, included, item_count - 1, capacity);

		
		if (total_with_item > total_without_item)
		{
			included = arr_with;
			included.at(item_count-1) = true;
			return total_with_item;
		}
		else
		{
			included.at(item_count - 1) = false;
			return total_without_item;
		}
	}
}

uint SDZ::KnapSack::FillDynamic(ItemSet & set, DTS::Vector<bool>& included, uint item_count, uint capacity)
{
	DTS::Vector<DTS::Vector<uint>> cost_vector(item_count + 1, DTS::Vector<uint>(capacity + 1));

	for (uint i = 0; i <= item_count; ++i)
	{
		for (uint w = 0; w <= capacity; ++w)
		{			
			if (i == 0 || w == 0)
				cost_vector[i][w] = 0;
			else
			{
				Item item = set.At(i-1);
				if (item.weight_ <= w)
					cost_vector[i][w] = Max(item.value_ + cost_vector[i - 1][w - item.weight_], cost_vector[i - 1][w]);
				else
					cost_vector[i][w] = cost_vector[i - 1][w];
			}					
		}
	}

	PickItem(set, included, cost_vector, item_count, capacity);

	return cost_vector[item_count][capacity];
}

void SDZ::KnapSack::PickItem(ItemSet &set, DTS::Vector<bool>& included, DTS::Vector<DTS::Vector<uint>> &vec, uint item_count, uint total_weight)
{
	if (item_count <= 0 || total_weight <= 0) return;
		
	int k = vec[item_count][total_weight];
	if (k != vec[item_count - 1][total_weight])  
	{
		// Add item to included
		included.at(item_count) = true;
		// Capacity Decreases
		PickItem(set, included, vec, item_count - 1, total_weight - set.At(item_count).weight_);
	}
	else {
		// Move on to next item; capacity no change
		PickItem(set, included, vec, item_count - 1, total_weight);
	}	
}

uint SDZ::KnapSack::FillGreedy(ItemSet & set, uint item_count, uint total_weight)
{
	// Sort the sot by the ratio of value to weight
	set.Sort();

	// Keep adding the values with the best ratio to the knapsack
	for (uint i = 0; i < item_count; i++)
	{
		if (item_set_.GetTotalWeight() + set.At(i).weight_ <= total_weight)
			item_set_.AddItem(set.At(i));
		else
			break;
	}
	return item_set_.GetTotalValue();
}




