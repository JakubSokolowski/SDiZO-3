#include "stdafx.h"
#include "Edge.h"

using namespace SDZ;

Edge::Edge()
	: source_id(0)
	, destination_id(0)
	, weight_(0)
	, residual_weight_(0)
{}

SDZ::Edge::Edge(uint source, uint destination, uint weight)
	: source_id(source)
	, destination_id(destination)
	, weight_(weight)
	, residual_weight_(weight)
{}


Edge::~Edge()
{}
