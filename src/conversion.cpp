/**
* @file conversion.cpp
* @author Ondrej
* @brief Implementation of conversion functions
*
**/

#include "conversion.hpp"
#include <iostream>
#include <sstream>

/* Converts string to number, returns "true" if string contains at least one digit and "false" if it doesn't contain any number */
bool strToNum(const std::string str, size_t &value)
{
    std::istringstream parse(str);
    if (parse >> value)
        return true;

    return false;
}

/* Converts string to pathfinding algorithm type */
bool strToAlgoType(std::string str, SearchAlgorithmType &algoType)
{
    /* To lowercase */
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });

    if (str == "bfs")
        algoType = SearchAlgorithmType::BFS;

    else if (str == "dfs")
        algoType = SearchAlgorithmType::DFS;

    else if (str == "random")
        algoType = SearchAlgorithmType::RandomSearch;

    else if (str == "greedy")
        algoType = SearchAlgorithmType::GreedySearch;

    else if (str == "astar")
        algoType = SearchAlgorithmType::AStar;

    else
        return false;

    return true;
}