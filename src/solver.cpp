#include "solver.hpp"
#include "converter.hpp"
#include <unordered_set>
#include <optional>
#include <algorithm>
#include <climits>

using std::unordered_set, std::array, std::map, std::pair, std::string, std::find, std::queue;



// Эвристическая функция, которая рассчитывает количество неправильных стикеров на каждой грани
// плохо подходит для IDA*, потому что сильно переоценивает глубину решения
int Solver::heuristic(const SCube& cube) {
    if (cube.isSolved()) {
        return 0; // Если кубик решен, эвристика равна 0
    }
    
    int h = 0;

    for (const auto& dir : MAIN_DIRECTIONS) {
        // 1. Получаем центральный цвет текущей грани
        const Colors center_color = cube.getCenterColor(dir);
        
        // 2. Получаем все элементы на текущей грани
        const auto face_elements = cube.getFaceElements(dir);

        // 3. Определяем, какая координата (и, соответственно, какой индекс цвета) 
        int color_index_on_piece = -1;
        for (int i = 0; i < 3; ++i) {
            if (dir[i] != 0) {
                color_index_on_piece = i;
                break;
            }
        }

        if (color_index_on_piece == -1) {
            continue; 
        }

        // 4. Проверяем каждый элемент на грани
        for (const auto& piece : face_elements) {
            if (piece.getColor()[color_index_on_piece] != center_color) {
                h++;
            }
        }
    }

    return (h + 3) / 4; 
}


const SCube SOLVED_CUBE = SCube(Cube(3));

// Эвристическая функция, которая рассчитывает количество неправильно расположенных элементов
// плохо подходит для DFS, потому что сильно недооценивает глубину решения
int Solver::heuristic_misplaced(const SCube& cube) {
    if (cube.isSolved()) {
        return 0; 
    }

    int misplaced_pieces_count = 0;
    const auto& current_parts = cube.getParts();
    const auto& solved_parts = SOLVED_CUBE.getParts();

    const int dimension = cube.size();

    for (int layer = 0; layer < dimension; ++layer) {
        for (int i = 0; i < dimension * dimension; ++i) { 
            const SPiece& current_piece = current_parts[layer][i];
            const SPiece& solved_piece = solved_parts[layer][i];

            if (current_piece.getPosition() != solved_piece.getPosition()
                || current_piece.getColor() != solved_piece.getColor()
                ) {
                misplaced_pieces_count++;
            }
        }
    }

    return misplaced_pieces_count / 8; 
}


//______________________________________________BFS___________________________________________

struct BFSNode {
    SCube cube;
    string path;
    char last_move;
};


string Solver::bfs_search(const SCube& start_cube) {
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


string Solver::solve_cube_bfs(const SCube& cube) {
    // std::cout << "Solving with BFS:" << std::endl;

    string result = bfs_search(cube);
    return result.empty() ? "" : result;
}

//_________________________________________DFS________________________________________________

static unordered_set<size_t> visited_states_DFS;


pair<int, string> Solver::dfs_search(SCube& cube, string& moves, int depth, int max_depth, char last_move) {
    if (cube.isSolved()) return {-1, moves};
    if (depth >= max_depth) return {INT_MAX, ""};

    size_t hash = cube.hash();
    if (visited_states_DFS.count(hash)) return {INT_MAX, ""};
    visited_states_DFS.insert(hash);

    int min_result = INT_MAX;
    string best_path;

    for (char move : ALL_MOVES) {
        if (last_move != 0 && INVERSE_MOVE.at(last_move) == move) continue;

        cube.rotateSide(move);
        moves.push_back(move);

        auto result = dfs_search(cube, moves, depth + 1, max_depth, move);

        if (result.first == -1) {
            visited_states_DFS.erase(hash);
            return result;
        }

        if (result.first < min_result) {
            min_result = result.first;
            best_path = result.second;
        }

        moves.pop_back();
        cube.rotateSide(INVERSE_MOVE.at(move));
    }

    visited_states_DFS.erase(hash);
    return {min_result, best_path};
}

string Solver::solve_cube_dfs(const SCube& cube) {
    visited_states_DFS.clear();
    const int max_depth_limit = heuristic(cube);

    // std::cout << "\nSolving with DFS:" << std::endl;

    for (int depth = 1; depth <= max_depth_limit; ++depth) {
        SCube work_cube = cube;
        string moves;
        visited_states_DFS.clear();
        auto result = dfs_search(work_cube, moves, 0, depth, 0);
        if (result.first == -1)
            return result.second;
    }
    return "";
}


//_________________________________________IDA*_______________________________________________


static unordered_set<size_t> visited_states_IDAstar;


pair<int, string> Solver::ida_search(SCube& cube, string& moves, int g, int threshold, char last_move) {
    const size_t current_hash = cube.hash();
    if (visited_states_IDAstar.count(current_hash)) {
        return {INT_MAX, ""}; // Состояние уже посещено
    }

    const int current_h = heuristic(cube);
    const int current_cost = g + current_h;


    visited_states_IDAstar.insert(current_hash);
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


string Solver::solve_cube_IDAstar(const SCube& cube) {
    visited_states_IDAstar.clear();
    // std::cout << "\nSolving with IDA*:" << std::endl;

    for (int depth = 0; depth <= 20; ++depth) {
        SCube work_cube = cube;
        std::string moves;
        visited_states_IDAstar.clear();
        auto result = ida_search(work_cube, moves, 0, depth, 0);

        if (result.first == -1) {
            return result.second;
        }
    }
    
    return "";
}

//__________________________________Running__________________________________________________

void Solver::scramble_cube(SCube& cube) {
    int n = 0;

    cube = SCube(Cube(3));

    std::cout << "Enter number of moves to scramble between 1 and 20: ";
    while (n <= 0 || n > 20) {
        std::cin >> n;
        if (n <= 0 || n >= 20) {
            std::cout << "number of moves must be between 1 and 20" << std::endl;

        }
    }
    cube.scramble(n);
}

void Solver::select_mode() {
    search_mode = -1;
    string result = "";

    std::cout << "Enter 4 to re-scramble cube" << std::endl;
    std::cout << "Choose search algorithm:\n(1): BFS\n(2): DFS\n(3): IDA*" << std::endl;
    std::cout << "Enter 0 to exit program" << std::endl;
    
    while (search_mode < 0 || search_mode > 3) {
        std::cin >> search_mode;

        if (search_mode == 4) {
            scramble_cube(cube);
        }
        
        if (search_mode < 0 || search_mode > 3) {
            std::cout << "Search mode must be 1 (BFS) or 2 (DFS) or 3 (IDA*)" << std::endl;
        }
    }
}

void Solver::solve_cube() {
    scramble_cube(cube);

    while (1) {
        string result = "";
        select_mode();
        timer.start();
    
        switch (search_mode) {
            case 1:
            {
                result = solve_cube_bfs(cube);
                break;
            }
            case 2:
            {
                result = solve_cube_dfs(cube);
                break;
            }
            case 3:
            {
                result = solve_cube_IDAstar(cube);
                break;
            }
            default:
            {
                return;
            }
        }

        if (result != "") {
            std::cout << "\n\033[32mSolution: " << result  << "\033[0m" << std::endl;
        } else {
            std::cout << "\n\033[31mSolution not found\033[0m" << std::endl;
        }
    
        timer.stop(result != "");
        std::cout << std::endl;
    }
}
