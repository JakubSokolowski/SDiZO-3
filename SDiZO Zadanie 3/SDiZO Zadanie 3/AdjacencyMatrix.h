#pragma once
#include <iostream>
#include "Vector.h"

namespace SDZ
{
	using uint = unsigned int;

	enum GraphParam { DIRECTED = true, UNDIRECTED = false };

	class AdjacencyMatrix
	{
	public:
		
		// Graph Construction

		AdjacencyMatrix(uint num_vertices, GraphParam is_directed_);
		virtual ~AdjacencyMatrix();

		void AddEdge(uint source, uint destination, uint weight);

		// Display

		void DisplayMatrix();

		// Getters

		uint GetNumVertices() const;
		uint GetNumEdges() const;
	

	private:
		uint num_vertices_;
		uint num_edges_;
		GraphParam is_directed_;
		DTS::Vector<DTS::Vector<uint>> matrix_;
	};
}


