#pragma once

#include "Vertex.h"
#include "List.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "FibonacciHeap.h"
#include "Vector.h"
#include <iostream>
#include <cmath>
#include <random>
#include <Windows.h>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <limits.h>

#define INF UINT32_MAX
	
namespace SDZ
{
	enum Heuristic { EUCLIDEAN = 4, MANHATTAN = 1, DIJKSTRA = 0 };
	enum MapState{ FREE = 0, TAKEN = 1, PATH = 2, PATH_START =3, PATH_FINISH = 4};
	enum GraphParameters { DIRECTED = true, UNDIRECTED = false, XY_COORDINATES = true, NO_COORDINATES = false };


	class AdjacencyListGraph
	{
	public:		

		// Constructors

		AdjacencyListGraph();
		AdjacencyListGraph(std::string filename, bool is_directed, bool is_euclidean);
		AdjacencyListGraph(uint vertices, bool is_directed, bool is_euclidian);
		AdjacencyListGraph(uint vertices, double density, bool is_directed, bool is_euclidean);	
		AdjacencyListGraph(uint vertices, double density, bool is_directed, bool is_euclidean, bool fast_generation);
		void SetParameters(uint vertices, double density, bool is_directed, bool is_euclidean);
		void SetParameters(uint vertices, bool is_directed, bool is_euclidean);
		~AdjacencyListGraph();

		// Bulding graph

		void AddEdge(uint source, uint destination, uint weight);
		void SetMaxWeight(uint max_weight);
		void WriteToFile(uint vertices, double density, bool is_directed, bool is_euclidean, std::string filename);
		void WriteToFile(std::string filename);
		void ReadFromFile(std::string filename, bool is_directed, bool is_euclidean);

		

		// Display 

		void DisplayEdges();
		void DisplayEdgesWithWeights();
		void DisplayMap();
		void DisplayMapWithId();
		void DisplayInfo();
		void DrawPath(DTS::Vector<uint> &vec);
		void ClearMap();

		// Traversal & Search

		void BFT(uint start);
		DTS::List<uint> BFT(uint start,uint finish);
		DTS::Vector<uint> AStarPathSearch(uint source, uint destination, Heuristic h);
		uint AStarDistanceSearch(uint source, uint destination, Heuristic h);

		// Minimum Spanning Trees

		void FHPrimMST();
		void PQPrimMST();

		// Maximum Flow
		uint FordFulkerson(uint source, uint sink);

	private:

		//Graph structure & information

		uint vertices_;
		uint edges_;
		uint edges_max_num_;
		uint edge_max_weight_;
		Vertex *adj_tab_;	
		
		// Construcor flags

		bool is_directed_;
		bool is_euclidean_;		

		uint FindVertex(uint x, uint y);

		// Map Generation

		int **map_;
		uint map_size_;

		void ClearGraph();
		uint CalculateMapSize();
		void GenerateCoordinates();
		uint GetNumberOfDigits(unsigned n);

		uint GetX(uint vertex);
		uint GetY(uint vertex);
		void SetX(uint vertex, uint value);
		void SetY(uint vertex, uint value);
	
		//Graph generation

		uint GetDistance(uint source, uint destination);
		void GenerateEdges(double density);
		void GenerateEdges(double density, uint max_weight);
		void GenerateEdgesFast(double density);
		void MakeConnected();

		void MarkAllNotVisited();
		

		//Ford Fulkerson
		bool FordFulkersonBFS(uint source, uint sink,int path[]);
		void CopyResidualWeights();	

		//A* 

		Heuristic heuristic_;

		void SetHeuristic(Heuristic h);		
		uint GetManhattanHeuristic(uint source, uint destination);
		uint GetEuclideanHeuristic(uint source, uint destination);
		uint GetHeuristicValue(uint source, uint destination);	
		void MarkAsPathVertex(uint node_id);
	};
}
