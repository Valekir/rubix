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

// Таблица обратных ходов
const std::map<char, char> INVERSE_MOVE = {
    {'R', 'r'}, {'r', 'R'},
    {'L', 'l'}, {'l', 'L'},
    {'F', 'f'}, {'f', 'F'},
    {'B', 'b'}, {'b', 'B'},
    {'U', 'u'}, {'u', 'U'},
    {'D', 'd'}, {'d', 'D'}
};

const std::array<char, 12> ALL_MOVES = {'R','L','F','B','U','D','r','l','f','b','u','d'};


std::string solve_cube(const SCube& cube);

std::pair<int, std::string> ida_search(Node node, int threshold, char last_move);
