#pragma once

namespace SDZ
{
	class Edge
	{
	public:

		typedef unsigned int uint;

		uint source_id;
		uint destination_id;
		uint weight_;

		//Used in Ford-Fulkerson
		uint residual_weight_;
		Edge();
		Edge(uint source, uint destination, uint weight);
		~Edge();
	};

}
