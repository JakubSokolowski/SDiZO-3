#pragma once

#include <random>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
#include "Vector.h"



typedef unsigned int uint;

namespace SDZ {
	//Struct representing items in knapsack
	class Item
	{
	public:

		//Types	

		uint weight_;
		uint value_;
		double value_per_weight_;

		// Constructors
		Item()
			: weight_(1)
			, value_(1)
			, value_per_weight_(1)
		{}
		Item(uint weight, uint value)
			: weight_(weight)
			, value_(value)
			, value_per_weight_(double(value) / double(weight))
		{}
		Item(const Item &item)
			: weight_(item.weight_)
			, value_(item.value_)
			, value_per_weight_(item.value_per_weight_)
		{}

		// Adding to set
		void AddValue(uint value)
		{
			value_ += value;
			value_per_weight_ = double(value_) / double(weight_);
		}

		// Comparision
		
		bool operator < (const Item &val) const
		{
			return value_per_weight_ > val.value_per_weight_;
		}
		bool operator > (const Item &val) 
		{
			return value_per_weight_ < val.value_per_weight_;
		}
	};

	//Set of items that is held by the knapsack
	class ItemSet
	{
	public:

		//Types
		typedef Item *										iterator;
		typedef const Item *								const_iterator;
		typedef std::reverse_iterator<iterator>				reverse_iterator;
		typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;

		// Constructors

		ItemSet();
		ItemSet(const ItemSet &set);
		ItemSet(uint item_num, uint max_item_value , uint max_item_weight );
		~ItemSet();


		// Iterators

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator cbegin() const noexcept;	
		const_iterator cend() const noexcept;

		// Reverse Iterators

		reverse_iterator rbegin() noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// Operators

		ItemSet & operator = (const ItemSet& set);
		ItemSet & operator = (ItemSet && set);

		// Adding

		void AddItem(Item &item);
		void AddItem(uint weight, uint value);
		void AddToValue(uint index, uint value);

		void FillRandom(uint item_num, uint max_item_value = 10, uint max_item_weight = 10);
		void IncreaseTotalValue(uint min_value);

		// Getters

		uint GetSize() { return item_set_.size(); }		
		uint GetTotalWeight() { return total_weight_; }
		uint GetTotalValue() { return total_value_; }
		Item At(uint index);

		// Display

		void DisplaySet();
		void DisplayInfo();

		// Discrete Knapsack Problem

		void Sort();

		// File input and output

		void WriteToFile(uint knapsack_capacity,std::string filepath);
		void ReadFromFile(std::string filepath);

		

	private:

		DTS::Vector<Item> item_set_;
		uint total_value_ = 0;
		uint total_weight_ = 0;

		//Sorting

		void QuickSort(uint p, uint r);
		uint Partirion(uint p, uint r);
	};
}

