#include "solver.hpp"
#include <unordered_set>
#include <algorithm>
#include <future>

using std::unordered_set, std::array, std::map, std::pair, std::string, std::find;

static unordered_set<size_t> visited_states;


// int heuristic(const SCube& cube) {
//     static const vector<array<int, 3>> MAIN_DIRECTIONS = {
//         {0,1,0}, {0,-1,0}, {1,0,0}, 
//         {-1,0,0}, {0,0,1}, {0,0,-1}
//     };
    
//     static map<array<int, 3>, Colors> center_colors;
//     if (center_colors.empty()) {
//         for (const auto& dir : MAIN_DIRECTIONS) {
//             center_colors[dir] = cube.getCenterColor(dir);
//         }
//     }

//     int h = 0;
//     auto parts = cube.getParts();
//     int n = cube.size();

//     for (int layer = 0; layer < n; layer++) {
//         for (int i = 0; i < n*n; i++) {
//             SPiece piece = parts[layer][i];
            
//             if (piece.getType() == 'C') {
//                 auto colors = piece.getColor();
//                 auto pos = piece.getPosition();
                
//                 for (int i = 0; i < 3; ++i) {
//                     if (pos[i] != 0) {
//                         array<int, 3> dir = {0};
//                         dir[i] = (pos[i] > 0) ? 1 : -1;
//                         Colors center_color = center_colors[dir];
//                         bool found = false;
//                         for (Colors c : colors) {
//                             if (c == center_color) {
//                                 found = true;
//                                 break;
//                             }
//                         }
//                         if (!found) {
//                             h += 2;
//                         }
//                     }
//                 }
//             }
//             else if (piece.getType() == 'E') {
//                 auto colors = piece.getColor();
//                 auto pos = piece.getPosition();
                
//                 bool correct = true;
//                 for (int i = 0; i < 3; ++i) {
//                     if (pos[i] != 0) {
//                         array<int, 3> dir = {0};
//                         dir[i] = pos[i];
//                         Colors center_color = center_colors[dir];
//                         bool found = false;
//                         for (Colors c : colors) {
//                             if (c == center_color) {
//                                 found = true;
//                                 break;
//                             }
//                         }
//                         if (!found) {
//                             correct = false;
//                             break;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     return (h + 3) / 4;
// }

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


// pair<int, string> ida_search(SCube& cube, const string& moves, int g, int threshold, char last_move) {
//     int current_h = heuristic(cube);
//     int current_cost = g + current_h;

//     if (current_cost > threshold) {
//         return {current_cost, ""};
//     }

//     if (cube.isSolved()) {
//         return {-1, moves};
//     }

//     int min_thresh = INT_MAX;
//     string solution;

//     for (char move : ALL_MOVES) {
//     if (last_move != 0 && (INVERSE_MOVE.at(last_move) == move || OPPOSITE_MOVE.at(last_move) == move)) {            continue;
//         }

//         cube.rotateSide(move);
//         auto result = ida_search(cube, moves + move, g + 1, threshold, move);
//         cube.rotateSide(INVERSE_MOVE.at(move));

//         if (result.first == -1) return result;
//         if (result.first < min_thresh) {
//             min_thresh = result.first;
//             solution = result.second;
//         }
//     }

//     return {min_thresh, solution};
// }

pair<int, string> ida_search(SCube& cube, string& moves, int g, int threshold, char last_move, char last_axis) {
    const int current_h = heuristic(cube);
    const int current_cost = g + current_h;

    if (current_cost > threshold) return {current_cost, ""};
    if (cube.isSolved()) return {-1, moves};

    int min_thresh = INT_MAX;
    string solution;

    for (char move : ALL_MOVES) {
        const char curr_axis = (move == 'R' || move == 'L' || move == 'r' || move == 'l') ? 'X' :
                              (move == 'U' || move == 'D' || move == 'u' || move == 'd') ? 'Y' : 'Z';
        
        // Пропускаем обратные и противоположные ходы, а также последовательные ходы на одной оси
        if ((last_move != 0 && INVERSE_MOVE.at(last_move) == move) ||
            (last_axis != 0 && curr_axis == last_axis)) {
            continue;
        }

        cube.rotateSide(move);
        moves.push_back(move);
        auto result = ida_search(cube, moves, g + 1, threshold, move, curr_axis);
        moves.pop_back();
        cube.rotateSide(INVERSE_MOVE.at(move));

        if (result.first == -1) return result;
        if (result.first < min_thresh) min_thresh = result.first;
    }

    return {min_thresh, solution};
}

string solve_cube(const SCube& cube) {
    int threshold = heuristic(cube);

    for (int depth = 0; depth <= 40; ++depth) {
        SCube work_cube = cube;
        std::string moves;
        auto result = ida_search(work_cube, moves, 0, depth, 0, 0);

        if (result.first == -1) {
            return result.second;
        }

        if (threshold >= 40) break;
        threshold = result.first;
    }
    
    return "Solution not found";
}

std::string solve_cube_parallel(const SCube& cube) {
    const int initial_threshold = heuristic(cube);
    std::vector<std::future<std::pair<int, std::string>>> futures;
    
    for (int depth = initial_threshold; depth <= 40; depth += 5) {
        futures.emplace_back(std::async(std::launch::async, [&cube, depth]{
            SCube work_cube = cube;
            std::string moves;
            auto result = ida_search(work_cube, moves, 0, depth, 0, 0);
            return result;
        }));
    }

    for (auto& fut : futures) {
        auto result = fut.get();
        if (result.first == -1) return result.second;
    }
    
    return "Solution not found";
}
