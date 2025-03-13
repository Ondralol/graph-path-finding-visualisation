/**
* @file conversion.hpp
* @author Ondrej
* @brief Consists of string to number function and string to algoritm type function
*
**/

#pragma once

#include "graph.hpp"

#include <algorithm>

/* Converts string to number, returns "true" if string contains at least one digit and "false" if it doesn't contain any number */
bool strToNum(const std::string str, size_t & value);

/* Converts string to pathfinding algorithm type */
bool strToAlgoType(std::string str, SearchAlgorithmType & algoType);

