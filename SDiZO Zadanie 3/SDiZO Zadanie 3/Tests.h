#pragma once
#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "KnapSack.h"
#include "ItemSet.h"


namespace SDZ
{

	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	// 1000.0 - ms
	double time_unit = 1000.0;

	void StartCounter()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			std::cout << "QueryPerformanceFrequency failed!\n";

		PCFreq = double(li.QuadPart) / time_unit;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}

	double GetCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}

	bool FileExists(std::string &filename)
	{
		struct stat buffer;
		return(stat(filename.c_str(), &buffer) == 0);
	}

	void UpperBoundTest()
	{
		double time = 0.0;
		// 32 = ~10 min
		uint num_items = 28;
		uint capacity = 300;
		ItemSet set = ItemSet(num_items, 30, 30);
		KnapSack sack = KnapSack(capacity);
		StartCounter();
		sack.FillKnapsack(set, KnapSack::BRUTEFORCE);
		time += GetCounter();
		std::cout << "Brute force algorithm time for " << num_items << " items and capacity " << capacity << " : " << time << std::endl;
		std::fstream file("test.txt");
	}

	const uint knapsack_sizes[] = { 100, 200, 400 };
	const uint item_num_bruteforce[] = { 10, 15, 20, 25, 30 };
	const uint item_num_all[] = { 25, 50, 100, 200, 400 };

	void BruteforceTests(std::string filename, uint repetition)
	{
		std::ofstream file(filename);

		file << "Brute\tDynamic\tGreedy\t\n";
		double bf_time = 0.0;
		double dyn_time = 0.0;
		double gr_time = 0.0;

		std::cout << "\nTest Start: All algorithms\n";
		for (uint i = 0; i < 3; i++)
		{
			std::cout << "Start for capacity: " << knapsack_sizes[i] << std::endl;

			for (uint j = 0; j < 5; j++)
			{
				std::cout << "\tnumber of items: " << item_num_bruteforce[j] << std::endl;

				
				KnapSack sack = KnapSack(knapsack_sizes[i]);

				for (uint r = 0; r < repetition; r++)
				{
					ItemSet set = ItemSet(item_num_bruteforce[j], 50, 50);
					set.IncreaseTotalWeight(knapsack_sizes[i]);

					StartCounter();
					sack.FillKnapsack(set, KnapSack::BRUTEFORCE);
					bf_time += GetCounter();

					StartCounter();
					sack.FillKnapsack(set, KnapSack::DYNAMIC);
					dyn_time += GetCounter();

					StartCounter();
					sack.FillKnapsack(set, KnapSack::GREEDY);
					gr_time += GetCounter();
				}

				file << std::fixed << bf_time / repetition << "\t";
				file << std::fixed << dyn_time / repetition << "\t";
				file << std::fixed << gr_time / repetition << "\n";

				bf_time = dyn_time = gr_time = 0.0;

			}
		}
		std::cout << "Finished all tests! Results in file " << filename.c_str() << std::endl;
		file.close();
		
	}
	void DynamicGreedyTests(std::string filename, uint repetition)
	{
		std::ofstream file(filename);

		file << "Dynamic\tGreedy\t\n";

		double dyn_time = 0.0;
		double gr_time = 0.0;
		uint sum_dynamic = 0;
		uint sum_greedy = 0;

		uint dyn_total = 0;
		uint greedy_total = 0;
		std::cout << "\nTest Start: Dynamic & Greedy\n ";
		for (uint i = 0; i < 3; i++)
		{
			std::cout << "Start for capacity: " << knapsack_sizes[i] << std::endl;

			for (uint j = 0; j < 5; j++)
			{
				std::cout << "\tnumber of items: " << item_num_all[j] << std::endl;
				
				KnapSack sack = KnapSack(knapsack_sizes[i]);

				for (uint r = 0; r < repetition; r++)
				{
					ItemSet set = ItemSet(item_num_all[j], 20, 20);
					set.IncreaseTotalWeight(knapsack_sizes[i]);
					
					
					StartCounter();
					sack.FillKnapsack(set, KnapSack::DYNAMIC);
					dyn_time += GetCounter();
					sum_dynamic += sack.GetTotalValue();

					StartCounter();
					sack.FillKnapsack(set, KnapSack::GREEDY);
					gr_time += GetCounter();
					sum_greedy += sack.GetTotalValue();
				}

				file << std::fixed << dyn_time / repetition << "\t";
				file << std::fixed << gr_time / repetition << "\n";
				dyn_time = gr_time = 0.0;

				//std::cout << sum_dynamic << " " << sum_greedy << std::endl;
				//std::cout << "Mean value loss: " << (sum_dynamic - sum_greedy) / repetition << std::endl;
				greedy_total += sum_greedy;
				dyn_total += sum_dynamic;
				sum_dynamic = sum_greedy = 0;
			}
		
		}
		std::cout << "Mean difference: " << ((dyn_total - greedy_total) / 15 ) / repetition << std::endl;
		std::cout << "Finished all tests! Results in file " << filename.c_str() << std::endl;
		file.close();

	}

	void ReadProblemFromFile(std::string filepath, ItemSet &set, KnapSack &sack)
	{
		std::fstream file;
		file.open(filepath, std::ios_base::in);

		if (!file)
			throw std::runtime_error("Could not open the file");

		//Clear the item other
		set.Clear();
		uint num_items, capacity;
		file >> capacity >> num_items;

		set.Reserve(num_items);
		sack = KnapSack(capacity);

		uint weight, value;

		while (file >> weight >> value)
			set.AddItem(weight, value);

		file.close();
	}
}

