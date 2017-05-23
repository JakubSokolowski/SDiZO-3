#pragma once
#include "List.h"
#include "Edge.h"
#include <iostream>

typedef unsigned int uint;


namespace SDZ
{
	class Vertex
	{
	public:

		uint vertex_id_;
		bool visited_, is_open_, is_closed_, is_separated;
	
		DTS::List<SDZ::Edge> list_;

		uint x_;
		uint y_;

		Vertex();
		Vertex(uint id);
		~Vertex();

		void AddEdge(uint destination, uint weight);
		uint GetEdgeWeight(uint destination);
		void AddToResidualWeight(uint destination, int weight);
		void DisplayListWithWeights();
		void DisplayList();

		bool IsConnected(uint destination);
		uint GetConnectionWeight(uint destination);
		uint GetResidualWeight(uint destination);
		void CopyResidual();
		void SetCoordinates(uint x, uint y);
		Vertex & operator = (const Vertex&);


	private:
		
		
	};
}