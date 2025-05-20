#include "solver.hpp"
#include "converter.hpp"
#include <unordered_set>
#include <optional>
#include <algorithm>
#include <climits>

using std::unordered_set, std::array, std::map, std::pair, std::string, std::find, std::queue;

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

static unordered_set<size_t> visited_states;

struct BFSNode {
    SCube cube;
    string path;
    char last_move;
};

int heuristic(const SCube& cube) {
    static constexpr std::array<std::array<int, 3>, 6> MAIN_DIRECTIONS = {{
        {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1}
    }};
    
    static std::array<Colors, 6> center_colors;
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 6; ++i) {
            center_colors[i] = cube.getCenterColor(MAIN_DIRECTIONS[i]);
        }
        initialized = true;
    }

    int h = 0;
    const auto& parts = cube.getParts();
    const int n = cube.size();

    for (int layer = 0; layer < n; layer++) {
        for (int i = 0; i < n*n; i++) {
            const auto& piece = parts[layer][i];
            const auto& colors = piece.getColor();
            const auto& pos = piece.getPosition();
            
            if (piece.getType() == 'C') {
                for (int j = 0; j < 3; ++j) {
                    if (pos[j] != 0) {
                        const int dir_idx = (pos[j] > 0) ? j*2 : j*2+1;
                        if (std::find(colors.begin(), colors.end(), center_colors[dir_idx]) == colors.end()) {
                            h += 2;
                        }
                    }
                }
            }
            else if (piece.getType() == 'E') {
                for (int j = 0; j < 3; ++j) {
                    if (pos[j] != 0) {
                        const int dir_idx = (pos[j] > 0) ? j*2 : j*2+1;
                        if (std::find(colors.begin(), colors.end(), center_colors[dir_idx]) == colors.end()) {
                            h += 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    return (h + 3) / 4;
}


//______________________________________________BFS___________________________________________

string bfs_search(const SCube& start_cube) {
    unordered_set<size_t> visited;
    queue<BFSNode> q;

    q.push({start_cube, "", 0});
    visited.insert(start_cube.hash());

    while (!q.empty()) {
        BFSNode current = q.front();
        q.pop();

        if (current.cube.isSolved()) {
            return current.path;
        }

        for (char move : ALL_MOVES) {
            if (current.last_move && INVERSE_MOVE.at(current.last_move) == move) continue;

            SCube next_cube = current.cube;
            next_cube.rotateSide(move);
            size_t h = next_cube.hash();

            if (visited.count(h)) continue;

            visited.insert(h);
            q.push({next_cube, current.path + move, move});
        }
    }

    return "";
}


string solve_cube_bfs(const SCube& cube) {
    std::cout << "Solving with BFS:" << std::endl;

    string result = bfs_search(cube);
    return result.empty() ? "" : result;
}

//_________________________________________DFS________________________________________________


pair<int, string> dfs_search(SCube& cube, string& moves, int depth, int max_depth, char last_move) {
    if (cube.isSolved()) return {-1, moves};
    if (depth >= max_depth) return {INT_MAX, ""};

    size_t hash = cube.hash();
    if (visited_states.count(hash)) return {INT_MAX, ""};
    visited_states.insert(hash);

    int min_result = INT_MAX;
    string best_path;

    for (char move : ALL_MOVES) {
        if (last_move != 0 && INVERSE_MOVE.at(last_move) == move) continue;

        cube.rotateSide(move);
        moves.push_back(move);

        auto result = dfs_search(cube, moves, depth + 1, max_depth, move);

        if (result.first == -1) {
            visited_states.erase(hash);
            return result;
        }

        if (result.first < min_result) {
            min_result = result.first;
            best_path = result.second;
        }

        moves.pop_back();
        cube.rotateSide(INVERSE_MOVE.at(move));
    }

    visited_states.erase(hash);
    return {min_result, best_path};
}

string solve_cube_dfs(const SCube& cube) {
    const int max_depth_limit = heuristic(cube);

    std::cout << "\nSolving with DFS:" << std::endl;

    for (int depth = 1; depth <= max_depth_limit; ++depth) {
        SCube work_cube = cube;
        string moves;
        visited_states.clear();
        auto result = dfs_search(work_cube, moves, 0, depth, 0);
        if (result.first == -1)
            return result.second;
    }
    return "";
}


//_________________________________________IDA*_______________________________________________


pair<int, string> ida_search(SCube& cube, string& moves, int g, int threshold, char last_move) {
    const size_t current_hash = cube.hash();
    if (visited_states.count(current_hash)) {
        return {INT_MAX, ""}; // Состояние уже посещено
    }

    const int current_h = heuristic(cube);
    const int current_cost = g + current_h;


    visited_states.insert(current_hash);
    if (cube.isSolved()) return {-1, moves};
    if (current_cost > threshold) return {current_cost, ""};

    int min_thresh = INT_MAX;
    string solution;

    for (char move : ALL_MOVES) {
        if (last_move != 0 && INVERSE_MOVE.at(last_move) == move) continue;
        else if (moves.length() >= 2) {
            if (moves[moves.length() - 2] == move && moves[moves.length() - 1] == move) continue;
        }

        cube.rotateSide(move);
        moves.push_back(move);
        auto result = ida_search(cube, moves, g + 1, threshold, move);
        moves.pop_back();
        cube.rotateSide(INVERSE_MOVE.at(move));

        if (result.first == -1) return result;
        if (result.first < min_thresh) min_thresh = result.first;
    }

    return {min_thresh, solution};
}


string solve_cube_IDAstar(const SCube& cube) {
    std::cout << "\nSolving with IDA*:" << std::endl;
    const int max_depth_limit = heuristic(cube);

    for (int depth = 0; max_depth_limit <= 20; ++depth) {
        SCube work_cube = cube;
        std::string moves;
        visited_states.clear();
        auto result = ida_search(work_cube, moves, 0, depth, 0);

        if (result.first == -1) {
            return result.second;
        }
    }
    
    return "";
}
