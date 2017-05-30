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

// Fills the knapsack with items from set using Algorithm
void SDZ::KnapSack::FillKnapsack(ItemSet & set, Algorithm alg)
{
	//Empty the knapsack
	item_set_ = ItemSet();

	// Bruteforce and dynamic algorithm need a bool array that shows
	// which items were included
	DTS::Vector<bool> included;

	switch (alg)
	{
		case BRUTEFORCE:
		{
			included.assign(set.GetSize(), false);
			max_value_ = FillBruteForce(set, included);
		}break;
		
		case GREEDY:
		{
			max_value_ = FillGreedy(set);
		}break;
		
		case DYNAMIC:
		{
			included.assign(set.GetSize(), false);
			max_value_ = FillDynamic(set, included);			
		}break;
	}

	//Add the items to the backpack
	for (uint it = 0; it < included.size(); it++)
	{
		if (included.at(it))
			item_set_.AddItem(set.At(it));
	}
}


uint SDZ::KnapSack::FillBruteForce(ItemSet & set, DTS::Vector<bool> & included)
{
	uint item_count = set.GetSize();
	uint capacity = capacity_;
	return FillRecursive(set, included, item_count, capacity);
}

uint SDZ::KnapSack::FillRecursive(ItemSet & set, DTS::Vector<bool>& included, uint item_count, uint capacity)
{
	// Base case
	if (item_count == 0 || capacity <= 0)
		return 0;

	// Extract the item, for easier comparision
	Item item = set.At(item_count - 1);

	// If the weight of the item is more than total_weight, the item cannot be included in optimal solution
	if (item.weight_ > capacity)
	{
		included.at(item_count - 1) = false;
		return FillRecursive(set, included, item_count - 1, capacity);
	}
	else
	{
		uint total_with_item = FillRecursive(set, included, item_count - 1, capacity - item.weight_) + item.value_;
		// Included needs to be copied, so that the changes made by FillRecursive are recorded
		DTS::Vector<bool> included_copy = included;
		uint total_without_item = FillRecursive(set, included, item_count - 1, capacity);

		// If the total value is bigger with item
		if (total_with_item > total_without_item)
		{
			// Include the item
			included = included_copy;
			included.at(item_count - 1) = true;
			return total_with_item;
		}
		else
		{
			// Exclude the item
			included.at(item_count - 1) = false;
			return total_without_item;
		}
	}
}

uint SDZ::KnapSack::FillDynamic(ItemSet & set, DTS::Vector<bool>& included)
{
	uint total_weight = capacity_;
	uint item_count = set.GetSize();

	DTS::Vector<DTS::Vector<uint>> cost_vector(item_count + 1, DTS::Vector<uint>(total_weight + 1, 0));

	
	for (uint i = 0; i <= item_count; ++i)
	{
		for (uint w = 0; w <= total_weight; ++w)
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

	PickItem(set, included, cost_vector, item_count, total_weight);

	return cost_vector[item_count][total_weight];
}

void SDZ::KnapSack::PickItem(ItemSet &set, DTS::Vector<bool>& included, DTS::Vector<DTS::Vector<uint>> &vec, uint i, uint w)
{
	
	if (i <= 0 || w <= 0) 
		return;
		
	int k = vec[i][w];
	// An item is selected, if the value in the vec directly above is not equal to current position in vec
	if (k != vec[i - 1][w])  
	{
		// Include the item
		included.at(i-1) = true;
		// Capacity Decreases
		PickItem(set, included, vec, i - 1, w - set.At(i-1).weight_);
	}
	else 
	{
		// Move on to next item; total_weight no change
		PickItem(set, included, vec, i - 1, w);
	}	
}

uint SDZ::KnapSack::FillGreedy(ItemSet & set)
{
	// Sort the sot by the ratio of value to weight
	set.Sort();
	uint total_weight = capacity_;
	uint item_count = set.GetSize();

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




