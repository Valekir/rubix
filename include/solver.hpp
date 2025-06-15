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


class Solver {
  private:
    int search_mode = 0;
    SCube cube = Cube(3);
    Timer timer;

    int heuristic(const SCube& cube);
    int heuristic_misplaced(const SCube& cube);

    string bfs_search(const SCube& start_cube);
    pair<int, string> dfs_search(SCube& cube, string& moves, int depth, int max_depth, char last_move);
    pair<int, string> ida_search(SCube& cube, string& moves, int g, int threshold, char last_move);

    void scramble_cube(SCube& cube);
  public:
    string solve_cube_bfs(const SCube& cube);
    string solve_cube_dfs(const SCube& input_cube);
    string solve_cube_IDAstar(const SCube& cube);

    void solve_cube();
    void select_mode();
};

const std::map<char, char> INVERSE_MOVE = {
    {'R', 'r'}, {'r', 'R'},
    {'L', 'l'}, {'l', 'L'},
    {'F', 'f'}, {'f', 'F'},
    {'B', 'b'}, {'b', 'B'},
    {'U', 'u'}, {'u', 'U'},
    {'D', 'd'}, {'d', 'D'}
};

const std::map<char, char> OPPOSITE_MOVE = {
    {'R', 'L'}, {'L', 'R'},
    {'r', 'l'}, {'l', 'r'},
    {'F', 'B'}, {'B', 'F'},
    {'f', 'b'}, {'b', 'f'},
    {'U', 'D'}, {'D', 'U'},
    {'u', 'd'}, {'d', 'u'},
};

const std::array<char, 12> ALL_MOVES = {'R','F','U','L','B','D','r','f','u','l','b','d'};

static constexpr std::array<std::array<int, 3>, 6> MAIN_DIRECTIONS = {{
    {0, 1, 0},
    {0, -1, 0},
    {1, 0, 0},
    {-1, 0, 0},
    {0, 0, 1},
    {0, 0, -1}
}};