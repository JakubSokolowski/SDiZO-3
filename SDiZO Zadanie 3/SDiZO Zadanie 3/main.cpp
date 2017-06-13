
#include "stdafx.h"
#include <iostream>
#include <string>
#include "ItemSet.h"
#include "KnapSack.h"
#include "Vector.h"
#include "Tests.h"
#include <conio.h>

using namespace SDZ;

void KnapSackTest()
{
	ItemSet set2 = ItemSet();
	//Should be 90
	set2.AddItem(10, 60);
	set2.AddItem(20, 100);
	set2.AddItem(30, 120);
	set2.DisplayInfo();
	set2.DisplaySet();
	KnapSack sack = KnapSack(200);
	std::cout << "\nBruteforce: ";
	sack.FillKnapsack(set2, KnapSack::BRUTEFORCE);
	sack.Display();
	std::cout << "\nDynamic: ";
	sack.FillKnapsack(set2, KnapSack::DYNAMIC);
	sack.Display();
	std::cout << "\nGreedy: ";
	sack.FillKnapsack(set2, KnapSack::GREEDY);
	sack.Display();
}
void Menu()
{
	
	
	ItemSet set;
	KnapSack sack;
	char option = '1';
	do
	{
		std::cout << "--MENU--" << std::endl;
		std::cout << "Dyskrety problem plecakowy" << std::endl;	
		std::cout << "1. Wygeneruj zestaw" << std::endl;
		std::cout << "2. Wczytaj z pliku" << std::endl;
		std::cout << "3. Wyswietl przedmioty" << std::endl;
		std::cout << "4. Wyswietl plecak" << std::endl;
		std::cout << "5. Wybor algorytmu" << std::endl;
		std::cout << "T. Testy" << std::endl;
		std::cout << "0. Koniec" << std::endl;
		
		option = _getche();
		switch (option)
		{
			uint num_items, capacity;
			// Random Item Set
			case '1':
			{
				std::cout << "\nPodaj ilosc elementow: ";
				std::cin >> num_items;
				std::cout << "Poda pojemnosc plecaka: ";
				std::cin >> capacity;
				sack = KnapSack(capacity);
				set.FillRandom(num_items, 20, 20);

			}break;
			// File input
			case '2':
			{
				std::cout << "\nPodaj nazwe pliku: " << std::flush;
				std::string filename;				
				std::getline(std::cin, filename);
				std::cout << "lmao";
				if (FileExists(filename))
				{
					ReadProblemFromFile(filename, set, sack);
				}			

			}break;
			// Display itemset
			case '3':
			{
				set.DisplayInfo();
				set.DisplaySet();

			}break;
			// Display sack
			case '4':
			{
				sack.Display();

			}break;
			case '5':
			{
				char option2 = '1';				
				
				do
				{
					
					std::cout << "\n1. Przeglad zupelny" << std::endl;
					std::cout << "2. Algorytm dynamiczny" << std::endl;
					std::cout << "3. Algorytm zachlanny" << std::endl;
					std::cout << "0. Koniec" << std::endl;

					option2 = _getche();
					switch (option2)
					{
						
					// Bruteforce
					case '1':
					{
						sack.FillKnapsack(set, KnapSack::BRUTEFORCE);
						sack.Display();

					}break;
					// Dynamic
					case '2':
					{
						sack.FillKnapsack(set, KnapSack::DYNAMIC);
						sack.Display();

					}break;
					// Greedy
					case '3':
					{
						sack.FillKnapsack(set, KnapSack::GREEDY);
						sack.Display();
					}break;					
					default:
						break;
					}

				} while (option2 != '0');	

			}break;

			case 'T':
			{
				std::cout << "\nWybrano Opcje testy, moze zajac to bardzo dlugo, nacisnij y aby potwierdzic" << std::endl;
				char opt;
				std::cin >> opt;
				if (opt == 'y')
				{
					BruteforceTests("brtests1.txt", 10);
					DynamicGreedyTests("dyngrtests1.txt", 100);
				}
				
			}break;
			default:
				break;
		}

	} while (option != '0');

}
void Sample()
{
	ItemSet set = ItemSet(30, 20, 20);
	set.WriteToFile(200, "TestSample.txt");
}
int main()
{
	
	Menu();	
	getchar();
    return 0;
}

