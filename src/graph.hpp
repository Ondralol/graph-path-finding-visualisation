/**
* @file graph.hpp
* @author Ondrej
*	@brief Class that manages graph and path finding using different algorithms
**/

#pragma once

#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>


enum class SearchAlgorithmType
{
    BFS,
    DFS,
    RandomSearch,
    GreedySearch,
    AStar
};

/** Represents position in graph */
using Position = std::pair<int, int>;

class GraphVisualisation;

class Graph
{
public:
    /** Constructor */
    Graph(SearchAlgorithmType algoType, const std::string filePath);

    /** Shows graph in ascii */
    void showGraphASCII(void);

    /** Show path info - opened vertices and path length */
    void pathInfo(void);

    /** Resets graph - clears all vectors and maps that need to be cleared */
    void reset(void);

    /** Finds all adjacent position where can you move in the graph */
    std::vector<Position> Adjacent(Position v);

    /** Implementation of BFS algorithm */
    void BFS(void);

    /** Implementation of DFS algorithm */
    void DFS(void);

    /** Implementation of RandomSearch algorithm */
    void RandomSearch(void);

    /** Implementation of GreedySearch algorithm */
    void GreedySearch(void);

    /** Implementation of AStar algorithm */
    void AStar(void);

    /** Sets up things */
    void setUp(int state);

    /** Class used for visualisation */
    friend class GraphVisualisation;

private:
    Position m_startPos;
    Position m_endPos;
    SearchAlgorithmType m_algoType;

    /** Using this to distinguish between wall, clear path and tree*/
    /** 0 - Wall, 1 - Empty, 2 - Tree */
    std::vector<std::vector<int>> m_graph;

    /** Representing the graph twice using Adjacency Matrix */
    std::map<Position, std::vector<Position>> m_adjList;

    /** For each step stores Position */
    std::vector<Position> m_visitedInOrder;

    /** For each Position stores positions which the current position opened */
    std::map<Position, std::vector<Position>> m_opened;

    /** Stores path that the algorithm found */
    std::vector<Position> m_path;
};
