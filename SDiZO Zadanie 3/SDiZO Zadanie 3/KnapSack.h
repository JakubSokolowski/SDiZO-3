#pragma once
#include "Vector.h"
#include "ItemSet.h"

namespace SDZ {

	class KnapSack
	{
	public:

		//Constructors 

		KnapSack();
		KnapSack(uint capacity, uint iten_num, uint max_item_weight);
		KnapSack(std::string filepath);
		~KnapSack();


		//Discrete KnapSack Problem

	private:
		DTS::Vector<Item> knapsack_;
		uint capacity_;
	};
}

