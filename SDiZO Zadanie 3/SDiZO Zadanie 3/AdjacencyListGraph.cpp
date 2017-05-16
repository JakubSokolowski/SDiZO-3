#include "stdafx.h"
#include "AdjacencyListGraph.h"


using namespace SDZ;

//Base graph constructor. Creates not directed graph with 10 vertices without X,Y coordinates
AdjacencyListGraph::AdjacencyListGraph()
	: vertices_(10)
	, edges_(0)
	, is_directed_(false)
	, is_euclidean_(false)
	, edges_max_num_(10*9/2)
	, adj_tab_(new Vertex[10])
{}

SDZ::AdjacencyListGraph::AdjacencyListGraph(std::string filepath, bool is_directed, bool is_euclidean)
{
	//Assign the graph parameters needed for proper graph construction
	is_euclidean_ = is_euclidean;
	is_directed_ = is_directed;

	std::fstream file;
	file.open(filepath, std::ios_base::in);
	if (!file)
		throw std::runtime_error("Could not open the file");

	uint edges, source, destination, weight, x, y;

	file >> edges;
	file >> vertices_;

	if (is_directed)
		edges_max_num_ = vertices_ * (vertices_ - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;

	adj_tab_ = new Vertex[vertices_];
	for (uint i = 0; i < vertices_; i++)
	{
		adj_tab_[i].vertex_id_ = i;
	}
	if (is_euclidean)
	{
		for (uint it = 0; it < vertices_; it++)
		{
			if (file >> x >> y)
			{
				SetX(it, x);
				SetY(it, y);
			}
		}
	}
	
	while (file >> source >> destination >> weight)
	{
		if(!adj_tab_[source].IsConnected(destination))
			AddEdge(source, destination, weight);
	}
	is_directed_ = is_directed;
	file.close();	
}

//Creates graph with given amount of vertices. If is_euclidean_ flag is set, generate X,Y coordinates for vertices
SDZ::AdjacencyListGraph::AdjacencyListGraph(uint vertices, bool is_directed, bool is_euclidean)
{	
	vertices_ = vertices;
	edges_ = 0;
	is_directed_ = is_directed;
	is_euclidean_ = is_euclidean;
	if (is_directed)
		edges_max_num_ = vertices * (vertices - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;
	
	adj_tab_ = new Vertex[vertices_];

	//Assign the ids 0 ... vertices-1
	for (uint it = 0; it < vertices_; ++it)
	{
		adj_tab_[it].vertex_id_ = it;
	}	
	//If vertices need to have x,y coordinates
	if (is_euclidean_)
	{
		map_size_ = CalculateMapSize();
		GenerateCoordinates();
	}
	MarkAllNotVisited();
}

//Creates graph with given amount of vertices and density. If is_euclidean_ flag is set, generate X,Y coordinates for vertices
SDZ::AdjacencyListGraph::AdjacencyListGraph(uint vertices, double density, bool is_directed, bool is_euclidean)
{
	vertices_ = vertices;
	edges_ = 0;
	is_directed_ = is_directed;
	is_euclidean_ = is_euclidean;
	if (is_directed)
		edges_max_num_ = vertices * (vertices - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;
	adj_tab_ = new Vertex[vertices_];

	//Assign the ids 0 ... vertices-1
	for (uint it = 0; it < vertices_; ++it)
	{
		adj_tab_[it].vertex_id_ = it;
	}
	if (is_euclidean_)
	{
		map_size_ = CalculateMapSize();
		GenerateCoordinates();
		GenerateEdgesFast(density);
	}
	else
	{
		GenerateEdgesFast(density);
	}
	MarkAllNotVisited();
}

//Creates graph with given amount of vertices and density. If is_euclidean_ flag is set, generate X,Y coordinates for vertices,
SDZ::AdjacencyListGraph::AdjacencyListGraph(uint vertices, double density, bool is_directed, bool is_euclidean, bool fast_generation)
{
	vertices_ = vertices;
	edges_ = 0;
	is_directed_ = is_directed;
	is_euclidean_ = is_euclidean;
	if (is_directed)
		edges_max_num_ = vertices * (vertices - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;
	adj_tab_ = new Vertex[vertices_];

	//Assign the ids 0 ... vertices-1
	for (uint it = 0; it < vertices_; ++it)
	{
		adj_tab_[it].vertex_id_ = it;
	}
	if (is_euclidean_)
	{
		map_size_ = CalculateMapSize();
		GenerateCoordinates();
		if(fast_generation)
			GenerateEdgesFast(density);
		else
			GenerateEdges(density);		
	}
	else
	{
		GenerateEdgesFast(density);
	}
	MarkAllNotVisited();
}

void SDZ::AdjacencyListGraph::ClearGraph()
{
	delete [] adj_tab_;
	edges_ = 0;
	vertices_ = 0;
	edges_max_num_ = 0;
	edge_max_weight_ = 0;

	delete[] map_;
	map_size_ = 0;
}

//Empties graph and generates new one with given parameters
void SDZ::AdjacencyListGraph::SetParameters(uint vertices, double density, bool is_directed, bool is_euclidean)
{
	ClearGraph();
	vertices_ = vertices;
	is_directed_ = is_directed;
	is_euclidean_ = is_euclidean;

	adj_tab_ = new Vertex[vertices_];

	if (is_directed)
		edges_max_num_ = vertices * (vertices - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;

	//Assign the ids 0 ... vertices-1
	for (uint it = 0; it < vertices_; ++it)
	{
		adj_tab_[it].vertex_id_ = it;
	}

	if (is_euclidean_)
	{
		map_size_ = CalculateMapSize();
		GenerateCoordinates();
		GenerateEdgesFast(density);
	}
	else
	{
		GenerateEdgesFast(density);
	}
	MarkAllNotVisited();
}

//Empties graph and generates new one with given parameters
void SDZ::AdjacencyListGraph::SetParameters(uint vertices, bool is_directed, bool is_euclidean)
{
	ClearGraph();
	vertices_ = vertices;
	edges_ = 0;
	is_directed_ = is_directed;
	is_euclidean_ = is_euclidean;
	if (is_directed)
		edges_max_num_ = vertices * (vertices - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;

	adj_tab_ = new Vertex[vertices_];

	//Assign the ids 0 ... vertices-1
	for (uint it = 0; it < vertices_; ++it)
	{
		adj_tab_[it].vertex_id_ = it;
	}
	//If vertices need to have x,y coordinates
	if (is_euclidean_)
	{
		map_size_ = CalculateMapSize();
		GenerateCoordinates();
	}
	MarkAllNotVisited();
}

//Destructor
AdjacencyListGraph::~AdjacencyListGraph()
{
	if(adj_tab_)
		delete[] adj_tab_;
	if(map_)
		delete[] map_;
}

//Connects source and destination vertex with edge of given weight;
void SDZ::AdjacencyListGraph::AddEdge(uint source, uint destination, uint weight)
{
	
	adj_tab_[source].AddEdge(destination, weight);
	if (!is_directed_)
		adj_tab_[destination].AddEdge(source, weight);
	++edges_;
}

//Sets the maximum weight of edge
void SDZ::AdjacencyListGraph::SetMaxWeight(uint max_weight)
{
	edge_max_weight_ = max_weight;
}

//Creates new graph and writes it to file
void SDZ::AdjacencyListGraph::WriteToFile(uint vertices, double density, bool is_directed, bool is_euclidean,std::string filename)
{
	SetParameters(vertices, density, is_directed, is_euclidean);
	std::ofstream file(filename);

	//If graph has coordinates, write them
	file << edges_ << " " << vertices_ << "\n";
	if (is_euclidean)
		for (uint j = 0; j< vertices_; j++)
			file << GetX(j) << " " << GetY(j) << "\n";

	//Write edges
	for (uint it = 0; it < vertices_; it++)
	{	
		for (auto i = adj_tab_[it].list_.begin(); i != adj_tab_[it].list_.end(); i++)		
			file << i->source_id << " " << i->destination_id << " " << i->weight_ << "\n";
	}
	file.close();
}

//Writes the existing graph to file
void SDZ::AdjacencyListGraph::WriteToFile(std::string filename)
{
	std::ofstream file(filename);

	//If graph has coordinates, write them
	file << edges_ << " " << vertices_ << "\n";
	if (is_euclidean_)
		for (uint j = 0; j< vertices_; j++)
			file << GetX(j) << " " << GetY(j) << "\n";

	//Write edges
	for (uint it = 0; it < vertices_; it++)
	{
		for (auto i = adj_tab_[it].list_.begin(); i != adj_tab_[it].list_.end(); i++)
			file << i->source_id << " " << i->destination_id << " " << i->weight_ << "\n";
	}
	file.close();
}

//Empties the graph, creates new one using information fro file
void SDZ::AdjacencyListGraph::ReadFromFile(std::string filepath, bool is_directed, bool is_euclidean)
{
	//Assign the graph parameters needed for proper graph construction
	ClearGraph();
	is_euclidean_ = is_euclidean;
	is_directed_ = is_directed;

	std::fstream file;
	file.open(filepath, std::ios_base::in);
	if (!file)
		throw std::runtime_error("Could not open the file");

	uint edges, source, destination, weight, x, y;

	file >> edges;
	file >> vertices_;

	if (is_directed)
		edges_max_num_ = vertices_ * (vertices_ - 1);
	else
		edges_max_num_ = vertices_ * (vertices_ - 1) / 2;

	adj_tab_ = new Vertex[vertices_];
	for (uint i = 0; i < vertices_; i++)
	{
		adj_tab_[i].vertex_id_ = i;
	}
	if (is_euclidean)
	{
		for (uint it = 0; it < vertices_; it++)
		{
			if (file >> x >> y)
			{
				SetX(it, x);
				SetY(it, y);
			}
		}
	}

	while (file >> source >> destination >> weight)
	{
		if (!adj_tab_[source].IsConnected(destination))
			AddEdge(source, destination, weight);
	}
	is_directed_ = is_directed;
	file.close();
}

//Displays all the edges in graph
void SDZ::AdjacencyListGraph::DisplayEdges()
{
	for (uint it = 0; it < vertices_; it++)
	{
		adj_tab_[it].DisplayList();
	}
}

//Displays all the edges with weights in graph
void SDZ::AdjacencyListGraph::DisplayEdgesWithWeights()
{
	for (uint it = 0; it < vertices_; it++)
	{
		adj_tab_[it].DisplayListWithWeights();
	}
	std::cout << std::endl;
}

//Breadth first traversal of graph. Displays the traversed path
void SDZ::AdjacencyListGraph::BFT(uint start_id)
{
	//Create Queue for ids 
	DTS::Queue<uint> queue = DTS::Queue<uint>();
	//Mark all vertices not visited
	MarkAllNotVisited();
	//Current id 
	uint current = start_id;
	uint counter = 0;

	//Mark the current node as visited and enqueue it
	adj_tab_[current].visited_ = true;
	queue.PushBack(current);
	
	std::cout << "\nBreadth first traversal from node: " << current << std::endl;

	while (!queue.IsEmpty())
	{
		//Deque a vertex from queue and print it's id
		current = queue.GetFront();
		std::cout << current << " ";
		++counter;
		queue.PopFront();

		//Iterate through all the adacent vertices of the dequeed vertex
		for (auto it = adj_tab_[current].list_.begin(); it!= adj_tab_[current].list_.end() ; it++)
		{
			//If adjacent has not been visited
			if (!adj_tab_[it->destination_id].visited_)
			{
				//Mark it visited
					adj_tab_[it->destination_id].visited_ = true;
				//And enqueue it
				queue.PushBack(it->destination_id);			
			}
		}
	}
	std::cout << " Traversed " << counter << " nodes. " << std::endl;
}

//Breadth first traversal of graph from star vertex to end vertex. 
//Returns list of nodes traversed
DTS::List<uint> SDZ::AdjacencyListGraph::BFT(uint start_id, uint finish_id)
{
	
	//Create Queue for ids 
	DTS::Queue<uint> queue = DTS::Queue<uint>();
	//Create list that holds traversed path
	DTS::List<uint> came_from = DTS::List<uint>();
	//Mark all vertices not visited
	MarkAllNotVisited();
	//Current id 
	uint current = start_id;
	uint counter = 0;

	//Mark the current node as visited and enqueue it
	adj_tab_[current].visited_ = true;
	queue.PushBack(current);

	while (!queue.IsEmpty())
	{
		//Deque a vertex from queue and print it's id
		current = queue.GetFront();
		came_from.PushBack(current);
		++counter;
		queue.PopFront();

		//If the goal is reached, stop early
		if (current == finish_id)
			break;

		//Iterate through all the adacent vertices of the dequeed vertex
		for (auto it = adj_tab_[current].list_.begin(); it != adj_tab_[current].list_.end(); it++)
		{
			//If adjacent has not been visited
			if (!adj_tab_[it->destination_id].visited_)
			{
				//Mark it visited
				adj_tab_[it->destination_id].visited_ = true;
				//And enqueue it
				queue.PushBack(it->destination_id);
			}
		}
	}
	return came_from;
}


//Marks all nodes as not visited
void SDZ::AdjacencyListGraph::MarkAllNotVisited()
{
	for (uint it = 0; it < vertices_; it++)
		adj_tab_[it].visited_ = false;
}

//Generates a map and randomly puts vertices on it. Coordinates XY are unique for every vertex
void SDZ::AdjacencyListGraph::GenerateCoordinates()
{	
	uint map_size_ = CalculateMapSize();

	//Create map 
	map_ = new int*[map_size_];
	for (uint i = 0; i < map_size_; i++)
	{
		//Mark all spaces in map as free
		map_[i] = new int[map_size_];
		for (uint j = 0; j < map_size_; j++)
			map_[i][j] = FREE;
	}

	//Uniform distribution random place generation
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, map_size_-1);

	for (uint it = 0; it < vertices_; it++)
	{		
		bool avalible_space = false;

		while (!avalible_space)
		{
			uint x = uni(rng);
			uint y = uni(rng);
			if (map_[x][y] == 0)
			{
				adj_tab_[it].SetCoordinates(x, y);
				map_[x][y] = TAKEN;
				avalible_space = true;				
			}
		}		
	}	
}

uint SDZ::AdjacencyListGraph::GetX(uint vertex)
{
	return adj_tab_[vertex].x_;
}

uint SDZ::AdjacencyListGraph::GetY(uint vertex)
{
	return adj_tab_[vertex].y_;
}

void SDZ::AdjacencyListGraph::SetX(uint vertex, uint value)
{
	adj_tab_[vertex].x_ = value;
}

void SDZ::AdjacencyListGraph::SetY(uint vertex, uint value)
{
	adj_tab_[vertex].y_ = value;
}

//Displays map of vertices. 
void SDZ::AdjacencyListGraph::DisplayMap()
{
	//Needed to change color
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::endl;	

	int counter = 0;
	if (is_euclidean_)
	{
		for (uint x = 0; x < map_size_; x++)
		{
			for (uint y = 0; y < map_size_; y++)
			{
				if (map_[x][y] == TAKEN)
				{
					// 12 - Red, 16 - Black, 15 - White, 13 - Green
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << std::setw(2) << (char)254u;
					counter++;
					SetConsoleTextAttribute(hConsole, 23);
				}	
				if (map_[x][y] == FREE)
				{
					SetConsoleTextAttribute(hConsole, 23);
					std::cout << std::setw(1) << (char)219u << (char)219u;
				}
				if (map_[x][y] == PATH)
				{
					SetConsoleTextAttribute(hConsole, 13);
					std::cout << std::setw(2) << (char)254u;
					SetConsoleTextAttribute(hConsole, 23);
				}	
				if (map_[x][y] == PATH_START)
				{
					SetConsoleTextAttribute(hConsole, 9);
					std::cout << std::setw(2) << (char)254u;
					SetConsoleTextAttribute(hConsole, 23);
				}
				if (map_[x][y] == PATH_FINISH)
				{
					SetConsoleTextAttribute(hConsole, 9);
					std::cout << std::setw(2) << (char)254u;
					SetConsoleTextAttribute(hConsole, 23);
				}
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
	SetConsoleTextAttribute(hConsole, 15);
}

//Displays map of vertices with ids
void SDZ::AdjacencyListGraph::DisplayMapWithId()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << std::endl;

	uint print_width = GetNumberOfDigits(vertices_) + 1 ;

	int counter = 0;
	if (is_euclidean_)
	{
		for (uint x = 0; x < map_size_; x++)
		{
			for (uint y = 0; y < map_size_; y++)
			{
				if (map_[x][y] == TAKEN)
				{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << std::setw(print_width) << FindVertex(x,y);
					counter++;
					SetConsoleTextAttribute(hConsole, 15);
				}
				if (map_[x][y] == FREE)
				{
					SetConsoleTextAttribute(hConsole, 15);
					std::cout << std::setw(print_width) << (char)254u;
				}
				if(map_[x][y] == PATH)
				{
					SetConsoleTextAttribute(hConsole, 10);
					std::cout << std::setw(print_width) << FindVertex(x,y);
					SetConsoleTextAttribute(hConsole, 15);
				}
				if (map_[x][y] == PATH_START)
				{
					SetConsoleTextAttribute(hConsole, 9);
					std::cout << std::setw(print_width) << FindVertex(x, y);
					SetConsoleTextAttribute(hConsole, 15);
				}
				if (map_[x][y] == PATH_FINISH)
				{
					SetConsoleTextAttribute(hConsole, 9);
					std::cout << std::setw(print_width) << FindVertex(x, y);
					SetConsoleTextAttribute(hConsole, 15);
				}
			}
			std::cout << std::endl;
		}
	}
}

//Displays the basci graph info
void SDZ::AdjacencyListGraph::DisplayInfo()
{
	std::cout << std::endl;
	if (is_directed_)
	{
		std::cout << "Graph is directed";
		if (is_euclidean_)
			std::cout << " and has X,Y coordinates on map"<<std::endl;
		else
		{
			std::cout << " and does not have X,Y coordinates on map" << std::endl;
			std::cout << "Max edge weight: " << edge_max_weight_ << std::endl;
		}
	}
	else
	{
		std::cout << "Graph is undirected";
		if (is_euclidean_)
			std::cout << " and has X,Y coordinates on map" << std::endl;
		else
		{
			std::cout << " and does not have X,Y coordinates on map" << std::endl;
			std::cout << "Max edge weight: " << edge_max_weight_ << std::endl;
		}
	}
	std::cout << "Vertices : " << vertices_ << std::endl;
	std::cout << "Edges    : " << edges_ << "  " << "Max edges: " << edges_max_num_ << std::endl;
	std::cout << std::setprecision(2) << "Density  : " << double(edges_) / edges_max_num_ << "\n\n";
}

//Marks path vertices on a map
void SDZ::AdjacencyListGraph::DrawPath(DTS::Vector<uint> &vec)
{
	ClearMap();
	for (auto it : vec)
	{
		MarkAsPathVertex(it);
	}
	map_[adj_tab_[vec.at(0)].x_][adj_tab_[vec.at(0)].y_] = PATH_START;
	map_[adj_tab_[vec.at(vec.size()-1)].x_][adj_tab_[vec.at(vec.size()-1)].y_] = PATH_FINISH;
}

//Connects randomly vertices with edges, until the density is reached
void SDZ::AdjacencyListGraph::GenerateEdges(double density)
{
	if (edge_max_weight_ <= 0)
		edge_max_weight_ = 10;
	MakeConnected();
	//Calculate the number of edges needed for given density
	uint desired_edges = static_cast<uint>(floor(density * edges_max_num_ + 0.5));
	//Calculte the number of missing edges, by substracting current number from desired
	if (edges_ >= desired_edges)
		return;
	uint missing_edges = desired_edges - edges_;

	
	std::random_device rd;
	std::mt19937 rng(rd());
	//Generate random integers to connect all id's
	std::uniform_int_distribution<uint> rnd_vertex(0, vertices_-1);
	
	std::uniform_int_distribution<uint> rnd_weight(1, edge_max_weight_);

	for (uint it = 0; it < missing_edges;)
	{
		uint source = rnd_vertex(rng);
		uint destination = rnd_vertex(rng);
		if (!adj_tab_[source].IsConnected(destination))
		{
			if (source == destination)
				continue;
			if (is_euclidean_)
				AddEdge(source, destination, GetDistance(source, destination));
			else
				AddEdge(source, destination, rnd_weight(rng));
			++it;
		}
	}
}

//Connects randomly graph with random weight edges, until the density is reached
void SDZ::AdjacencyListGraph::GenerateEdges(double density, uint max_weight)
{
	//Calculate the number of edges needed for given density
	uint desired_edges = static_cast<uint>(density * edges_max_num_);

	// If given density is smaller than the min density required for graph to be alredy_connected, return
	if (edges_ >= desired_edges)
		return;

	//Calculte the number of missing edges, by substracting current number from desired
	uint missing_edges = desired_edges - edges_;

	std::random_device rd;
	std::mt19937 rng(rd());
	//Generate random integers to connect all id's
	std::uniform_int_distribution<uint> rnd_vertex(0, vertices_ - 1);
	std::uniform_int_distribution<uint> rnd_weight(1, max_weight);

	for (uint it = 0; it < missing_edges;)
	{
		//Choose 2 random vertices

		uint source = rnd_vertex(rng);
		uint destination = rnd_vertex(rng);

		//Check if they are alredy_connected
		if (!adj_tab_[source].IsConnected(destination))
		{
			// Cannot connect vertex with itself - loops are not allowed
			if (source == destination)
				continue;
			// Connect the vertices with random weight
			AddEdge(source, destination, rnd_weight(rng));
			++it;
		}
	}
}

//Connects randomly graph with random weight edges, until the density is reached
void SDZ::AdjacencyListGraph::GenerateEdgesFast(double density)
{
	if (edge_max_weight_ <= 0)
		edge_max_weight_ = 10;
	
	MakeConnected();
	//Calculate the number of edges needed for given density
	uint desired_edges = static_cast<uint>(floor(density * edges_max_num_ + 0.5));

	//Calculte the number of missing edges, by substracting current number from desired
	if (edges_ >= desired_edges)
		return;	

	uint threshold = static_cast<uint>(floor(density * 100 + 0.5));
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<uint> uni(0, 100);
	std::uniform_int_distribution<uint> weight(1, edge_max_weight_);

	while (edges_<=desired_edges)
	{
		if (edges_ == desired_edges)
			break;
		for (uint i = 0; i < vertices_; i++)
		{
			if (edges_ == desired_edges)
				break;
			for (uint j = 0; j < vertices_; j++)
			{
				if (i == j)
					continue;
				if (edges_ == desired_edges)
					break;
				//Connect the edges ith probability
				uint value = uni(rng);
				if (value < threshold)
				{
					if (!adj_tab_[i].IsConnected(j))
					{
						if (is_euclidean_)
						{
							AddEdge(i, j, GetDistance(i, j));
						}
						else
						{
							AddEdge(i, j, weight(rng));
						}
					}						
				}
				if (edges_ == desired_edges)
					break;
			}
		}		
	}
}

//Connects all the vertices with each other so the graph is fully alredy_connected
void SDZ::AdjacencyListGraph::MakeConnected()
{
	bool old = is_directed_;
	if (old)
		is_directed_ = false;

	DTS::Vector<bool> alredy_connected(vertices_, false);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> weight(0, edge_max_weight_);
	std::uniform_int_distribution<int> rnd_ver(0, vertices_-1);
	
	uint source = rnd_ver(rng);
	alredy_connected[source] = true;
	for (uint it = 0; it < vertices_-1;)
	{				
		uint destination = rnd_ver(rng);
		if (source == destination)
			continue;

		if (!adj_tab_[source].IsConnected(destination) && !alredy_connected[destination])
		{
			if (is_euclidean_)						
				AddEdge(source, destination, GetDistance(source, destination));							
			else
				AddEdge(source, destination, weight(rng));

			if (old)
				++edges_;
				
			alredy_connected[destination] = true;
			source = destination;
			++it;
		}
	}	
	is_directed_ = old;
}

//Searches the shortest path from start to finish using A* algorithm. 
//Avalible heuristics - Manhattan Distance, Euclidean Distance
//Returns the list containing the shortest path
DTS::Vector<uint> SDZ::AdjacencyListGraph::AStarPathSearch(uint start, uint goal, Heuristic h)
{

	std::unordered_map<uint, uint> came_from, cost_so_far;
	SetHeuristic(h);
	
	//Create Queue for vertices and coresponding priorities 
	DTS::PriorityQueue<uint, uint> frontier;


	frontier.Insert(start, 0);
	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.IsEmpty())
	{
		//Deque a vertex with lowest priority from queue
		auto current = frontier.GetFront();	
		frontier.PopFront();

		//If the goal is reached, stop early
		if (current == goal)		
			break;

		//Iterate through all the successor nodes of vertex
		for (auto it = adj_tab_[current].list_.begin(); it != adj_tab_[current].list_.end(); it++)
		{
			//Set new cost of succesor node to the cost of path so far 
			//plus weight of connection from current to succesor 
			uint new_cost = cost_so_far[current] + it->weight_;

			//If the cost so far for node is empty, or new_cost is lower tha cost so far for that node
			if (!cost_so_far.count(it->destination_id) || new_cost < cost_so_far[it->destination_id])
			{
				//Update the cost of path to the node
				cost_so_far[it->destination_id] = new_cost;
				//Calculate the priority of node
				uint priority = new_cost + GetHeuristicValue(it->destination_id, goal);	
				//Insert the node into queue
				frontier.Insert(it->destination_id, priority);	
				//Add node to path
				came_from[it->destination_id] = current;
			}
		}
	}
	//Copy the path from map to array
	DTS::Vector<uint> rec_path;
	uint curr = goal;
	rec_path.push_back(curr);
	while (curr != start)
	{
		curr = came_from[curr];
		rec_path.push_back(curr);
	}
	//Reverse the path
	std::reverse(rec_path.begin(), rec_path.end());

	return rec_path;
}

//Searches the shortest path from start to finish using A* algorithm. 
//Avalible heuristics - Manhattan Distance, Euclidean Distance
//Returns the distance of path
uint SDZ::AdjacencyListGraph::AStarDistanceSearch(uint start_id, uint finish_id, Heuristic h)
{
	//Stores path 
	std::unordered_map<uint, uint> came_from;
	//Stores cost 
	std::unordered_map<uint, uint> cost_so_far;
	SetHeuristic(h);

	//Create Queue for ids 
	DTS::PriorityQueue<uint, uint> p_queue;

	//Current id 
	uint current = start_id;

	p_queue.Insert(start_id, 0);
	came_from[start_id] = start_id;
	cost_so_far[start_id] = 0;

	while (!p_queue.IsEmpty())
	{
		//Deque a vertex with lowest priority from queue
		current = p_queue.GetFront();
		p_queue.PopFront();

		//If the goal is reached, stop early
		if (current == finish_id)
			break;	

		//Iterate through all the succesor nodes
		for (auto it = adj_tab_[current].list_.begin(); it != adj_tab_[current].list_.end(); it++)
		{
			//Set new cost of succesor node to the cost of path so far 
			//plus weight of connection from current to succesor 
			uint new_cost = cost_so_far[current] + it->weight_;
			
			//If the cost so far for node is empty, or new_cost is lower tha cost so far for that node
			if (!cost_so_far.count(it->destination_id) || new_cost < cost_so_far[it->destination_id])
			{
				//Update the cost of path to the node
				cost_so_far[it->destination_id] = new_cost;
				//Calculate the priority of node
				uint priority = new_cost + GetHeuristicValue(it->destination_id, finish_id);
				//Insert the node into queue
				p_queue.Insert(it->destination_id, priority);
				//Add node to path
				came_from[it->destination_id] = current;
			}
		}
	}
	return cost_so_far[finish_id];
}

//Finds the MST of graph using fibonacci heap based priority queue
void SDZ::AdjacencyListGraph::FHPrimMST()
{
	//Create a heap to store vertices 
	DTS::FibonacciHeap<uint, uint> fib_heap;
	
	//Create Vector for keys, initializa all keys as infinite
	DTS::Vector<uint> keys(vertices_, INF);

	DTS::Vector<int> parent(vertices_, -1);
	DTS::Vector<int> weights(vertices_, 0);

	DTS::Vector<bool> inMST(vertices_, false);

	//Vertex 0 is source
	uint source = 0;
	fib_heap.Insert(source, 0);
	keys[source] = 0;

	while (!fib_heap.IsEmpty())
	{
		//The first vertex in pair is the minimum key vertex,
		//extract it from heap.
		uint u = fib_heap.GetMin();
		fib_heap.PopMin();
		inMST[u] = true;

		for (auto it = adj_tab_[u].list_.begin(); it != adj_tab_[u].list_.end(); it++)
		{
			//Get vertex label and weight of curren adjacent of u
			uint ver = (*it).destination_id;
			uint weight = (*it).weight_;

			if (inMST[ver])
				continue;
			//If v is not in mst and weight of (u,v) is smaller than current key of v
			if (keys[ver] > weight)
			{
				keys[ver] = weight;
				fib_heap.Insert(ver, weight);
				parent[ver] = u;
				weights[ver] = weight;
			}
		}
	}

	std::cout << "Minimum spanning tree : " << std::endl;
	for (uint i = 1; i < vertices_; ++i)
	{
		std::cout << parent[i] << " " << i << ":  " <<weights[i] << std::endl;
	}	
}

//Finds the MST of graph using binary heap based priority queue
void SDZ::AdjacencyListGraph::PQPrimMST()
{

	//Create a heap to store vertices 
	DTS::PriorityQueue<uint, uint> pqueue;

	//Create Vector for keys, initializa all keys as infinite
	DTS::Vector<uint> keys(vertices_, INF);

	DTS::Vector<int> parent(vertices_, -1);
	DTS::Vector<int> weights(vertices_, 0);

	DTS::Vector<bool> inMST(vertices_, false);

	//Vertex 0 is source
	uint source = 0;
	pqueue.Insert(source, 0);
	keys[source] = 0;

	while (!pqueue.IsEmpty())
	{
		//The first vertex in pair is the minimum key vertex,
		//extract it from heap.
		uint u = pqueue.GetFront();
		pqueue.PopFront();
		inMST[u] = true;

		for (auto it = adj_tab_[u].list_.begin(); it != adj_tab_[u].list_.end(); it++)
		{
			//Get vertex label and weight of curren adjacent of u
			uint ver = (*it).destination_id;
			uint weight = (*it).weight_;

			if (inMST[ver])
				continue;
			//If v is not in mst and weight of (u,v) is smaller than current key of v
			if (keys[ver] > weight)
			{
				keys[ver] = weight;
				pqueue.Insert(ver, weight);
				parent[ver] = u;
				weights[ver] = weight;
			}
		}
	}

	std::cout << "Minimum spanning tree : " << std::endl;
	for (uint i = 1; i < vertices_; ++i)
	{
		std::cout << parent[i] << " " << i << ":  " << weights[i] << std::endl;
	}
}

//Returns the id of vertex at given coordinates
uint SDZ::AdjacencyListGraph::FindVertex(uint x, uint y)
{
	for (uint it = 0; it < vertices_; ++it)
	{
		if (adj_tab_[it].x_ == x && adj_tab_[it].y_ == y)
		{
			return adj_tab_[it].vertex_id_;
		}
	}		
	return NULL;
}

//Marks the vertex on map as path. Used in Display 
void SDZ::AdjacencyListGraph::MarkAsPathVertex(uint vertex_id)
{
	map_[adj_tab_[vertex_id].x_][adj_tab_[vertex_id].y_] = 2;
}

//Ereases the path drawn by A*
void SDZ::AdjacencyListGraph::ClearMap()
{
	if (is_euclidean_)
	{
		for (uint x = 0; x < map_size_; x++)
		{
			for (uint y = 0; y < map_size_; y++)
			{
				if (map_[x][y] != FREE)
				{
					map_[x][y] = TAKEN;
				}
			}
		}
	}
}

//Returns size of map suitable for number of vertices in graph
uint SDZ::AdjacencyListGraph::CalculateMapSize()
{
	//Set map size so that only ~25% places are taken
	return uint(ceil(sqrt(vertices_ * 4)));
}

//Returns the max flow from source to sink
uint SDZ::AdjacencyListGraph::FordFulkerson(uint source, uint sink)
{
	CopyResidualWeights();
	uint curr_node, parent_node;
	uint max_flow = 0;
	//Filled by BFS
	int *path = new int[vertices_];

	// Augument the flow while there is a path from source to sink

	while (FordFulkersonBFS(source, sink, path))
	{
		//Find the minimum residual capacity of the edges along the
		//path filled by BFS.

		uint path_flow = INF;

		for (curr_node = sink; curr_node != source; curr_node = path[curr_node])
		{
			parent_node = path[curr_node];
			path_flow = min(path_flow, adj_tab_[parent_node].GetResidualWeight(curr_node));
		}
		for (curr_node = sink; curr_node != source; curr_node = path[curr_node])
		{
			parent_node = path[curr_node];
			//Substract path flow from reverse path
			adj_tab_[parent_node].AddToResidualWeight(curr_node, path_flow * -1);
			//And add to path
			adj_tab_[curr_node].AddToResidualWeight(parent_node, path_flow);
		}
		//Add to max_flow
		max_flow += path_flow;
	}
	return max_flow;
}

//Returns true if there exits an augumenting path in graph
bool SDZ::AdjacencyListGraph::FordFulkersonBFS(uint source, uint destination,int path[])
{
	//Create Queue for ids 
	DTS::Queue<uint> queue = DTS::Queue<uint>();
	bool *visited = new bool[vertices_];

	for (uint i = 0; i<vertices_; i++)
		visited[i] = false;

	visited[source] = true;
	path[source] = -1;
	queue.PushBack(source);

	while (!queue.IsEmpty())
	{
		//Deque a vertex from queue 
		uint curr_ver = queue.GetFront();
		queue.PopFront();	

		//Iterate through all the adacent vertices of the dequeed vertex
		for (auto it = adj_tab_[curr_ver].list_.begin(); it != adj_tab_[curr_ver].list_.end(); it++)
		{
			//If adjacent has not been visited and the connection capacity > 0 (there is possbile flow)
			if (!visited[it->destination_id] && adj_tab_[curr_ver].GetResidualWeight(it->destination_id) > 0)
			{				
				//Enqueue it
				queue.PushBack(it->destination_id);
				//Add to path
				path[it->destination_id] = curr_ver;
				//Mark as visited
				visited[it->destination_id] = true;
			}
		}
	}
	return (visited[destination]==true);
}

//Copies the weights into residual weihts so that ford-fulkerson can be used multiple times on the same graph
void SDZ::AdjacencyListGraph::CopyResidualWeights()
{
	for (uint it = 0; it < vertices_; it++)
	{
		adj_tab_[it].CopyResidual();
	}
}

//Sets the heuristic used in A* search
void SDZ::AdjacencyListGraph::SetHeuristic(Heuristic h)
{
	heuristic_ = h;
}

//Returns straight line distance between 2 vertices , casted to unsigned int
uint SDZ::AdjacencyListGraph::GetDistance(uint source, uint destination)
{
	//Calculte the distance between 2 points and cast it on unsigned int
	int X = abs((int)adj_tab_[source].x_ - (int)adj_tab_[destination].x_);
	int Y = abs((int)adj_tab_[source].y_ - (int)adj_tab_[destination].y_);
	return static_cast<uint>(sqrt(pow(X, 2) + pow(Y, 2)));
}

//Returns value of Manhattan Heuristic 
uint SDZ::AdjacencyListGraph::GetManhattanHeuristic(uint source, uint destination)
{
	int X = abs((int)adj_tab_[source].x_ - (int)adj_tab_[destination].x_);
	int Y = abs((int)adj_tab_[source].y_ - (int)adj_tab_[destination].y_);
	return static_cast<uint>(MANHATTAN * (X + Y));
}

//Returns value of Euclidean Heuristic 
uint SDZ::AdjacencyListGraph::GetEuclideanHeuristic(uint source, uint destination)
{
	int X = abs((int)adj_tab_[source].x_ - (int)adj_tab_[destination].x_);
	int Y = abs((int)adj_tab_[source].y_ - (int)adj_tab_[destination].y_);
	return static_cast<uint>(EUCLIDEAN * sqrt(pow(X, 2) + pow(Y, 2)));
}

//Returns the value of current heuristic function
uint SDZ::AdjacencyListGraph::GetHeuristicValue(uint source, uint destination)
{
	if (heuristic_ == MANHATTAN)
	{
		return GetManhattanHeuristic(source, destination);
	}
	if(heuristic_ == EUCLIDEAN)
	{
		return GetEuclideanHeuristic(source, destination);
	}
	else
	{
		return 0;
	}
}

//Returns number of digits in integer
uint SDZ::AdjacencyListGraph::GetNumberOfDigits(unsigned n)
{
	return n > 0 ? (int)log10((double)n) + 1 : 1;
}


