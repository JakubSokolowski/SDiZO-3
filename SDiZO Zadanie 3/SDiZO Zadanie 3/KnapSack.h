#pragma once
#include "Vector.h"
#include "ItemSet.h"

namespace SDZ {

	class KnapSack
	{
	public:

		//Constructors 

		KnapSack();
		KnapSack(uint capacity);
		KnapSack(uint capacity, ItemSet &set);
		KnapSack(std::string filepath);
		~KnapSack();

		//Discrete KnapSack Problem

		void FillBruteForce(ItemSet &set);
		
		//Display

		void Display();
		void Sort();


	private:
		ItemSet item_set_;
		uint capacity_;
		uint max_value_;

		uint FillRecursive(ItemSet &set, DTS::Vector<bool> &included, uint item_count, uint total_weight);
	};
}

