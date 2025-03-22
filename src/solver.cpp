#include "solver.hpp"

using std::string, std::pair;




// Вспомогательные функции
std::string vector_to_key(std::vector<int> vec) {
    return std::to_string(vec[0]) + "," + std::to_string(vec[1]) + "," + std::to_string(vec[2]);
}

int heuristic(Cube cube) {
    std::vector<std::vector<int>> main_directions = {
        {0,1,0}, {0,-1,0}, {1,0,0}, 
        {-1,0,0}, {0,0,1}, {0,0,-1}
    };

    // Собираем цвета центров
    std::map<std::string, Colors> center_colors;
    for (auto dir : main_directions) {
        center_colors[vector_to_key(dir)] = cube.get_center_color(dir);
    }

    int h = 0;
    auto parts = cube.get_parts();

    // Анализ угловых элементов
    for (auto layer : parts) {
        for (auto piece : layer) {
            if (piece.get_type() != 'C') continue;

            auto colors = piece.get_color();
            auto pos = piece.get_pos();
            
            // Определение связанных граней
            std::vector<std::string> faces;
            for (int i = 0; i < 3; ++i) {
                if (pos[i] != 0) {
                    std::vector<int> dir(3, 0);
                    dir[i] = pos[i];
                    faces.push_back(vector_to_key(dir));
                }
            }

            // Проверка соответствия цветов
            for (auto f : faces) {
                if (std::find(colors.begin(), colors.end(), center_colors[f]) == colors.end()) {
                    h += 2;
                }
            }
        }
    }

    // Анализ рёберных элементов
    for (auto layer : parts) {
        for (auto piece : layer) {
            if (piece.get_type() != 'E') continue;

            auto colors = piece.get_color();
            auto pos = piece.get_pos();
            
            std::vector<std::string> faces;
            for (int i = 0; i < 3; ++i) {
                if (pos[i] != 0) {
                    std::vector<int> dir(3, 0);
                    dir[i] = pos[i];
                    faces.push_back(vector_to_key(dir));
                }
            }

            bool is_correct = true;
            for (auto f : faces) {
                if (std::find(colors.begin(), colors.end(), center_colors[f]) == colors.end()) {
                    is_correct = false;
                    break;
                }
            }
            if (!is_correct) h += 1;
        }
    }

    return (h + 3) / 4; // Округление вверх
}



// Рекурсивный поиск с ограничением глубины
pair<int, string> ida_search(Node node, int threshold, char last_move) {
    // Проверка на решенное состояние
    if (node.cube.is_solved()) {
        return { -1, node.moves };
    }
    
    // Проверка превышения порога стоимости
    if (node.cost > threshold) {
        return { node.cost, "" };
    }
    
    int min_threshold = INT_MAX;
    string solution;

    // Перебор всех возможных ходов
    for (char move : ALL_MOVES) {
        // Пропуск обратного предыдущему ходу
        if (last_move != 0 && INVERSE_MOVE.at(last_move) == move) {
            continue;
        }

        // Применение хода к кубику
        Cube new_cube = node.cube;
        new_cube.rotate_side(move);
        
        // Создание нового узла
        Node child(new_cube, node.moves + move, node.moves.size() + 1);
        
        // Рекурсивный поиск
        auto result = ida_search(child, threshold, move);
        
        // Проверка найденного решения
        if (result.first == -1) {
            return result;
        }
        
        // Обновление минимального порога
        if (result.first < min_threshold) {
            min_threshold = result.first;
        }
    }
    
    return { min_threshold, "" };
}



// Основная функция решения
string solve_cube(Cube cube) {
    int threshold = heuristic(cube);
    
    while (true) {
        Node start_node(cube, "", 0);
        auto result = ida_search(start_node, threshold, 0);
        
        if (result.first == -1) {
            return result.second;
        }
        if (threshold >= 40) { // Ограничение максимальной глубины
            return "Solution not found within limit";
        }
        threshold = result.first;
    }
}
