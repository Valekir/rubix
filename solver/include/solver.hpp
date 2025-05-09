#pragma once
#include "converter.hpp"
#include <map>
#include <string>
#include <vector>
#include <unordered_set>
#include <utility>
#include <climits>


int heuristic(const SCube& cube);


// Структура узла поиска
struct Node {
    SCube cube;
    std::string moves;
    int cost;
    
    Node(SCube c, std::string m, int g) 
        : cube(c), moves(m), cost(g + heuristic(c)) {}
};


// std::pair<int, std::string> ida_search(Node node, int threshold, char last_move);
std::pair<int, std::string> ida_search(SCube& cube, std::string& moves, int g, int threshold, char last_move, char last_axis);

std::string solve_cube(const SCube& cube);
