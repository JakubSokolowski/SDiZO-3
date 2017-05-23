#include "stdafx.h"
#include "KnapSack.h"

using namespace SDZ;
KnapSack::KnapSack()
	: capacity_(10)
	, knapsack_(10)
{}

KnapSack::KnapSack(uint capacity, uint item_count, uint max_item_size)
	: capacity_(capacity)
{
}

KnapSack::KnapSack(std::string filepath)
{
}


KnapSack::~KnapSack()
{
}



