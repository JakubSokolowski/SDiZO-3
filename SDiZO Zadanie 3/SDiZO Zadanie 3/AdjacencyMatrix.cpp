#include "stdafx.h"
#include "AdjacencyMatrix.h"

using namespace SDZ;


SDZ::AdjacencyMatrix::AdjacencyMatrix(uint num_vertices, GraphParam is_directed)
	: num_vertices_{ num_vertices }
	, is_directed_{ is_directed }
	, num_edges_{0}
	, matrix_( num_vertices, DTS::Vector<uint>( num_vertices,0 ))
{}

AdjacencyMatrix::~AdjacencyMatrix()
{}

uint SDZ::AdjacencyMatrix::GetNumVertices() const
{
	return num_vertices_;
}

uint SDZ::AdjacencyMatrix::GetNumEdges() const
{
	return num_edges_;
}

void SDZ::AdjacencyMatrix::AddEdge(uint source, uint destination, uint weight)
{
	if (source >= num_vertices_ || destination >= num_vertices_)
		throw std::out_of_range{ "negative values" };

	if (matrix_[source][destination] == 0)
		num_edges_++;
	if (is_directed_)
	{
		matrix_[source][destination] = weight;
	}
	else
	{
		matrix_[source][destination] = weight;
		matrix_[destination][source] = weight;
	}
}

void SDZ::AdjacencyMatrix::DisplayMatrix()
{
	for (auto row : matrix_)
	{
		for (auto edge : row)
			std::cout << edge << " ";
		std::cout << std::endl;
	}
}
