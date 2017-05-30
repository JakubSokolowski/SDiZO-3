#pragma once
#include "Vector.h"

namespace SDZ
{
	using uint = unsigned int;

	class AdjacencyMatrix
	{
	public:
		AdjacencyMatrix(uint num_vertices, bool is_directed_);
		virtual ~AdjacencyMatrix();

		uint GetNumVertices() const;
		uint GetNumEdges() const;
		void AddEdge(uint source, uint destination, uint weight);

	private:
		uint num_vertices_;
		uint num_edges_;
		bool is_directed_;
		DTS::Vector<DTS::Vector<uint>> matrix_;
	};
}


