#pragma once
#include "Vector.h"
#include "ItemSet.h"

namespace SDZ {

	class KnapSack
	{
	public:
		enum Heuristic  { BRUTEFORCE = 1, GREEDY = 2, DYNAMIC = 3 };
		//Constructors 

		KnapSack();
		KnapSack(uint capacity);
		KnapSack(uint capacity, ItemSet &set);
		KnapSack(std::string filepath);
		~KnapSack();

		//Discrete KnapSack Problem

		void FillKnapsack(ItemSet &set, Heuristic h);

		//Display

		void Display();
		void Sort();


	private:

		ItemSet item_set_;
		uint capacity_;
		uint max_value_;

		uint FillBruteForce(ItemSet &set, DTS::Vector<bool> &included, uint item_count, uint total_weight);

		uint FillDynamic(ItemSet &set, DTS::Vector<bool> &included, uint item_count, uint total_weight);
		void PickItem(ItemSet &set, DTS::Vector<bool> &included, DTS::Vector<DTS::Vector<uint>> &vec, uint item_count, uint total_weight);

		uint FillGreedy(ItemSet &set, uint item_count, uint total_weight);
	};
}

