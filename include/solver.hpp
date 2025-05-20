#pragma once
#include "converter.hpp"
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <utility>
#include <climits>
#include <optional>

using std::unordered_set, std::array, std::map, std::pair, std::string, std::find;


int heuristic(const SCube& cube);


string bfs_search(const SCube& start_cube);
string solve_cube_bfs(const SCube& cube);


pair<int, string> dfs_search(SCube& cube, string& moves, int depth, int max_depth, char last_move);
string solve_cube_dfs(const SCube& input_cube);


pair<int, string> ida_search(SCube& cube, string& moves, int g, int threshold, char last_move);
string solve_cube_IDAstar(const SCube& cube);