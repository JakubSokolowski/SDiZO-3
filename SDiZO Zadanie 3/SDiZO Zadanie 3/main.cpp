
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include "ItemSet.h"
#include "KnapSack.h"
#include "Vector.h"

using namespace SDZ;
int main()
{
	ItemSet set2 = ItemSet();
	//Should be 90
	set2.AddItem(1, 1);
	set2.AddItem(3, 4);
	set2.AddItem(4, 5);
	set2.AddItem(5, 7);
	//set2.AddItem(12, 2);
	//set2.AddItem(2, 10);
	set2.FillRandom(4, 20, 5);
	set2.DisplayInfo();
	set2.DisplaySet();
	KnapSack sack = KnapSack(7);
	sack.FillKnapsack(set2, KnapSack::BRUTEFORCE);
	sack.Display();
	sack.FillKnapsack(set2, KnapSack::DYNAMIC);
	sack.Display();
	sack.FillKnapsack(set2, KnapSack::GREEDY);
	sack.Display();
	
	


	getchar();
    return 0;
}

