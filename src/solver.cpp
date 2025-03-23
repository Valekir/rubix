#include "solver.hpp"
#include <unordered_set>
#include <algorithm>

using std::unordered_set, std::array, std::map, std::pair, std::string, std::find;

static unordered_set<size_t> visited_states;


int heuristic(const SCube& cube) {
    static const vector<array<int, 3>> MAIN_DIRECTIONS = {
        {0,1,0}, {0,-1,0}, {1,0,0}, 
        {-1,0,0}, {0,0,1}, {0,0,-1}
    };
    
    static map<array<int, 3>, Colors> center_colors;
    if (center_colors.empty()) {
        for (const auto& dir : MAIN_DIRECTIONS) {
            center_colors[dir] = cube.getCenterColor(dir);
        }
    }

    int h = 0;
    const auto& parts = cube.getParts();
    const int n = cube.size();

    for (int layer = 0; layer < n; layer++) {
        for (int i = 0; i < n*n; i++) {
            SPiece piece = parts[layer][i];
            
            if (piece.getType() == 'C') {
                const auto& colors = piece.getColor();
                const auto& pos = piece.getPosition();
                
                for (int i = 0; i < 3; ++i) {
                    if (pos[i] != 0) {
                        array<int, 3> dir = {0};
                        dir[i] = pos[i];
                        if (find(colors.begin(), colors.end(), center_colors[dir]) == colors.end()) {
                            h += 2;
                        }
                    }
                }
            }
            else if (piece.getType() == 'E') {
                const auto& colors = piece.getColor();
                const auto& pos = piece.getPosition();
                bool correct = true;
                
                for (int i = 0; i < 3; ++i) {
                    if (pos[i] != 0) {
                        array<int, 3> dir = {0};
                        dir[i] = pos[i];
                        if (find(colors.begin(), colors.end(), center_colors[dir]) == colors.end()) {
                            correct = false;
                            break;
                        }
                    }
                }
                
                if (!correct) h += 1;
            }
        }
    }

    return (h + 3) / 4;
}


pair<int, string> ida_search(Node node, int threshold, char last_move) {
    if (node.cube.isSolved()) {
        return {-1, node.moves};
    }

    const size_t current_hash = node.cube.hash();
    if (visited_states.count(current_hash)) {
        return {INT_MAX, ""};
    }
    visited_states.insert(current_hash);

    if (node.cost > threshold) {
        return {node.cost, ""};
    }

    int min_thresh = INT_MAX;
    string solution;

    for (char move : ALL_MOVES) {
        if (last_move != 0 && INVERSE_MOVE.at(last_move) == move) {
            continue;
        }

        SCube new_cube = node.cube;
        new_cube.rotateSide(move);
        
        Node child(new_cube, node.moves + move, node.moves.size() + 1);
        auto result = ida_search(std::move(child), threshold, move);

        if (result.first == -1) return result;
        if (result.first < min_thresh) min_thresh = result.first;
    }

    visited_states.erase(current_hash);
    return {min_thresh, ""};
}


string solve_cube(const SCube& cube) {
    int threshold = heuristic(cube);
    visited_states.clear();

    for (int depth = 0; depth <= 40; ++depth) {
        Node start_node(cube, "", 0);
        auto result = ida_search(start_node, threshold, 0);
        
        if (result.first == -1) {
            return result.second;
        }
        
        if (threshold >= 40) break;
        threshold = result.first;
    }
    
    return "Solution not found";
}
