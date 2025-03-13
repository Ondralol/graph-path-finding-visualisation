/**
* @file graph.cpp
* @author Ondrej
* @brief Implementation of Graph class
**/

#include "graph.hpp"

#include <iostream>
#include <sstream>
#include <queue>
#include <chrono>
#include <random>
#include <stack>
#include <algorithm>

/** Parses input file and creates graph, throws exception if maze file not found */
Graph::Graph(SearchAlgorithmType algoType, const std::string filePath)
{
	m_algoType = algoType;
	std::ifstream inputFile(filePath);
  
	/* Checks for file validiy */
	if (!inputFile)
		throw std::invalid_argument("Maze file not found");

	
	/* Parses the txt file to create the graph   */
	/* 0 - Wall, 1 - Empty, 2 - Tree */
	std::string line;
  std::istringstream parseLine(line);
	while (std::getline(inputFile, line))
  {
    /* Break when end of maze */
		if (line[0] == 's')
			break;

  	std::vector<int> graph_row;
		for (char x: line)
		{
			if (std::tolower(x) == 'x')
				graph_row.push_back(0);
			else if (std::tolower(x) == ' ')
				graph_row.push_back(1);
			else
				graph_row.push_back(2);
		}
		m_graph.push_back(graph_row);
	}
	
	/* Save start end end positions */

  std::string dummy;
	parseLine = std::istringstream(line);

	parseLine >> dummy >> m_startPos.first >> dummy >> m_startPos.second;
	std::getline(inputFile, line);
	parseLine = std::istringstream(line);
	parseLine >> dummy >> m_endPos.first >> dummy >> m_endPos.second;
	
}

/** Finds all adjacent vertices/positions */
std::vector<Position> Graph::Adjacent(Position v)
{
	std::vector<Position> positions;

	int x, y;
	
	/* Left */
	x = v.first - 1;
	y = v.second;
	if (x >= 0)
	{
		//std::cout << x << " " << y << std::endl;
		if (m_graph[y][x] == true)
			positions.push_back(Position(x,y));
	}

	/* Right */
  x = v.first + 1;
	y = v.second;
  if (x < (int)m_graph[v.second].size())
  {
    if (m_graph[y][x] == true)
      positions.push_back(Position(x,y));
  }

	/* Up */
	x = v.first;
  y = v.second - 1;
  if (y >= 0)
  {
    if (m_graph[y][x] == true)
      positions.push_back(Position(x,y));
  }

	/* Down */
	x = v.first;
  y = v.second + 1;
  if (y < (int)m_graph.size())
  {
    if (m_graph[y][x] == true)
      positions.push_back(Position(x,y));
  }
	
	for (auto x: positions)
	{	
		if (x.first < 0 || x.second < 0 || x.first > 511 || x.second > 511)
			std::cout << x.first << " " << x.second << std::endl; 
	}
	return positions;
}

/** Implementation of DFS algorithm, saves the visited and opened vertices as well as path */
void Graph::BFS(void)
{
	std::map<Position, bool> visited;
	std::map<Position, Position> predecessor;
	std::queue<Position> queue;

	queue.push(m_startPos);

	Position v;
	
	/* Optional */
	m_visitedInOrder.push_back(m_startPos);
	visited[m_startPos] = true;
	predecessor[m_startPos] = Position(-1, -1);

	/* Stops the loop when end position is found */
	bool breakFlag = false;

	while (!queue.empty() && !breakFlag)
	{
		v = queue.front();
		queue.pop();
		
		for (Position w: this -> Adjacent(v))
		{
			if (visited.find(w) == visited.end())
			{
				visited[w] = true;
				queue.push(w);
				m_visitedInOrder.push_back(w);
				predecessor[w] = v;
				if (w == m_endPos)
				{
					breakFlag = true;
					break;
				}
				m_opened[v].push_back(w);
			}	
		}
	}
	
	/* If end was not found, then return */
	if (predecessor.find(m_endPos) == predecessor.end())
		return;


	/* Reconstruct path */
	Position pos = m_endPos;

	std::vector<Position> reversePath;
	m_path.push_back(pos);
	while (predecessor[pos] != Position(-1, -1))
	{
		pos = predecessor[pos];
		m_path.push_back(pos);
	}

	std::reverse(m_path.begin(), m_path.end());
}


/** Implementation of DFS algorithm, saves the visited and opened vertices as well as path */
void Graph::DFS(void)
{
	std::map<Position, bool> visited;
  std::map<Position, Position> predecessor;
  std::stack<Position> stack;

  stack.push(m_startPos);

  Position v;

  /* Optional */
  m_visitedInOrder.push_back(m_startPos);
  visited[m_startPos] = true;
  predecessor[m_startPos] = Position(-1, -1);

  /* Stops the loop when end position is found */
  bool breakFlag = false;

  while (!stack.empty() && !breakFlag)
  {
    v = stack.top();
    stack.pop();
		
		visited[v] = true;
		m_visitedInOrder.push_back(v);

    for (Position w: Adjacent(v))
    {
      if (visited.find(w) == visited.end())
      {
        stack.push(w);
        predecessor[w] = v;
        if (w == m_endPos)
        {
          breakFlag = true;
          break;
        }

        m_opened[v].push_back(w);
      }
    }
  }

  /* If end was not found, then return */
  if (predecessor.find(m_endPos) == predecessor.end())
    return;


  /* Reconstruct path */
  Position pos = m_endPos;

  std::vector<Position> reversePath;
  m_path.push_back(pos);
  while (predecessor[pos] != Position(-1, -1))
  {
    pos = predecessor[pos];
    m_path.push_back(pos);
  }

  std::reverse(m_path.begin(), m_path.end());

}

/** Comparator for priorityQueue - using for RandomSearch */
struct PriorityQueueComparatorInt
{
	bool operator()(const std::pair<Position, int>& a, const std::pair<Position, int>& b)
	{
		return a.second > b.second;
	}
};

/** Generates random number in range 1, 10000*/
int randomNum(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
	int min = 1;
	int max = 10000;

	std::uniform_int_distribution<> dist(min, max);

	return dist(gen);
}

/** Implementation of random search algorithm, saves the visited and opened vertices as well as path */
void Graph::RandomSearch(void)
{
	std::map<Position, bool> visited;
  std::map<Position, Position> predecessor;
  std::priority_queue<std::pair<Position, int>, std::vector<std::pair<Position, int>>, PriorityQueueComparatorInt> queue;

  queue.push({m_startPos, randomNum()});

  Position v;
	std::cout << "test" << std::endl;

  /* Optional */
  m_visitedInOrder.push_back(m_startPos);
  visited[m_startPos] = true;
  predecessor[m_startPos] = Position(-1, -1);

  /* Stops the loop when end position is found */
  bool breakFlag = false;

  while (!queue.empty() && !breakFlag)
  {
    v = queue.top().first;
    queue.pop();
    for (Position w: Adjacent(v))
    {
      if (visited.find(w) == visited.end())
      {
        visited[w] = true;
        queue.push({w, randomNum()});
        m_visitedInOrder.push_back(w);
        predecessor[w] = v;
        if (w == m_endPos)
        {
          breakFlag = true;
          break;
        }

        m_opened[v].push_back(w);
      }
    }
  }

  /* If end was not found, then return */
  if (predecessor.find(m_endPos) == predecessor.end())
    return;


  /* Reconstruct path */
  Position pos = m_endPos;

  std::vector<Position> reversePath;
  m_path.push_back(pos);
  while (predecessor[pos] != Position(-1, -1))
  {
    pos = predecessor[pos];
    m_path.push_back(pos);
  }

  std::reverse(m_path.begin(), m_path.end());

}

/** A* and Greedy algorithm heuristic - using L1 Norm/Metric, option to change to L2 */
double heuristic(const Position & pos1, const Position & pos2)
{
  /* L1 Norm */
	return std::abs(pos1.first - pos2.first) + std::abs(pos1.second - pos2.second);

	/* L2 Norm*/
	//return std::sqrt(std::pow(pos1.first - pos2.first, 2) + std::pow(pos1.second - pos2.second, 2));
}

/** Comparator for priorityQueue - using for A* and Greedy Search */
struct PriorityQueueComparatorDouble
{
  bool operator()(const std::pair<Position, double>& a, const std::pair<Position, double>& b)
  {
    return a.second > b.second;
  }
};


/** Implementation of Greedy algorithm using L2 Norm, saves the visited and opened vertices as well as path */
void Graph::GreedySearch(void)
{
	std::map<Position, bool> visited;
  std::map<Position, size_t> distance;
  std::map<Position, Position> predecessor;
  std::priority_queue<std::pair<Position, double>, std::vector<std::pair<Position, double>>, PriorityQueueComparatorDouble> queue;

  queue.push({m_startPos, 0});

  Position v;

  /* Optional */
  m_visitedInOrder.push_back(m_startPos);
  visited[m_startPos] = true;
  predecessor[m_startPos] = Position(-1, -1);
  distance[m_startPos] = 0;

  /* Stops the loop when end position is found */
  bool breakFlag = false;

  while (!queue.empty() && !breakFlag)
  {
    v = queue.top().first;
    queue.pop();
    for (Position w: Adjacent(v))
    {
      if (visited.find(w) == visited.end())
      {
        visited[w] = true;
        m_visitedInOrder.push_back(w);
        predecessor[w] = v;
        distance[w] = distance[v] + 1;
        queue.push({w, heuristic(w, m_endPos)});
        if (w == m_endPos)
        { 
          breakFlag = true;
          break;
        }
        
        m_opened[v].push_back(w);
      }
    }
  }

  /* If end was not found, then return */
  if (predecessor.find(m_endPos) == predecessor.end())
    return;


  /* Reconstruct path */
  Position pos = m_endPos;

  std::vector<Position> reversePath;
  m_path.push_back(pos);
  while (predecessor[pos] != Position(-1, -1))
  {
    pos = predecessor[pos];
    m_path.push_back(pos);
  }

  std::reverse(m_path.begin(), m_path.end());

}


/** Implementation of A* algorithm using L2 norm, saves the visited and opened vertices as well as path */
void Graph::AStar(void)
{
	std::map<Position, bool> visited;
	std::map<Position, size_t> distance;
  std::map<Position, Position> predecessor;
  std::priority_queue<std::pair<Position, double>, std::vector<std::pair<Position, double>>, PriorityQueueComparatorDouble> queue;

  queue.push({m_startPos, 0});

  Position v;

  /* Optional */
  m_visitedInOrder.push_back(m_startPos);
  visited[m_startPos] = true;
  predecessor[m_startPos] = Position(-1, -1);
	distance[m_startPos] = 0;

  /* Stops the loop when end position is found */
  bool breakFlag = false;

  while (!queue.empty() && !breakFlag)
  {
    v = queue.top().first;
    queue.pop();
    for (Position w: Adjacent(v))
    {
      if (visited.find(w) == visited.end())
      {
        visited[w] = true;
        m_visitedInOrder.push_back(w);
        predecessor[w] = v;
				distance[w] = distance[v] + 1;
        queue.push({w, distance[w] + 1.0 * heuristic(w, m_endPos)});
				if (w == m_endPos)
        {
          breakFlag = true;
          break;
        }

        m_opened[v].push_back(w);
      }
    }
  }

  /* If end was not found, then return */
  if (predecessor.find(m_endPos) == predecessor.end())
    return;


  /* Reconstruct path */
  Position pos = m_endPos;

  std::vector<Position> reversePath;
  m_path.push_back(pos);
  while (predecessor[pos] != Position(-1, -1))
  {
    pos = predecessor[pos];
    m_path.push_back(pos);
  }

  std::reverse(m_path.begin(), m_path.end());

}

/** Set up things */
void Graph::setUp(int state)
{
	if (m_startPos == m_endPos)
		return;
	
	if (state != - 1)
	{
		m_algoType = static_cast<SearchAlgorithmType>(state);
	}

	switch(m_algoType)
	{
		case SearchAlgorithmType::BFS:
			this -> BFS();
			break;
		case SearchAlgorithmType::DFS:
			this -> DFS();
			break;
		case SearchAlgorithmType::RandomSearch:
			this -> RandomSearch();
			break;
		case SearchAlgorithmType::GreedySearch:
			this -> GreedySearch();
			break;
	  case SearchAlgorithmType::AStar:
			this -> AStar();
			break;	
	}
	
}

/** Displays graph in STDOUT */
void Graph::showGraphASCII()
{
	for (auto row: m_graph)
	{
		for(auto x: row)
		{
			if (x == false)
				std::cout << 'x';
			else
				std::cout << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << "Start: (" << m_startPos.first << ", " << m_startPos.second << ")" << std::endl;
	std::cout << "End: (" << m_endPos.first << ", " << m_endPos.second << ")" << std::endl;

}

/** Clears containers used to store graph paths etc. */
void Graph::reset(void)
{
	m_visitedInOrder.clear();
	m_opened.clear();
	m_path.clear();
}	

/** Displays path length and how many vertices were opened/visited */
void Graph::pathInfo(void)
{
	std::cout << "Opened vertices: " << m_visitedInOrder.size() << std::endl;
  std::cout << "Path length: " << m_path.size() << std::endl;
}


