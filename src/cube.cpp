#include "cube.hpp"

std::ostream& operator<<(std::ostream& out, Piece p) {
    out << "Type: " << p.get_type();
    out << "\tColors: ";
    for (auto i : p.get_color()) {
        out << (int) i << ' ';
    }
    out << "\tPos: ";
    for (auto i : p.get_pos()) {
        out << i;
    }
    return out;
}

/// @brief Конструктор по умолчанию, создает пустой объект, у которого все грани покращены в черный цвет
Piece::Piece() {
    for (int i = 0; i < 3; i++) {
        color.push_back(Colors::K);
        angles.push_back(0);
    }
    type = '0';
}

/// @brief Создает объект класса Piece
/// @param _color Массив, задающий цвета элемента (3 цвета, если у элемента должно быть меньше цветов - вместо них будет черный)
/// @param _angles Массив, задающий направление элемента (3 координаты)
/// @param _type Тип элемента (угол, ребро, центр)
Piece::Piece(vector <Colors> _color, vector <int> _angles, char _type) {
    for (int i = 0; i < 3; i++) {
        color.push_back(_color[i]);
        angles.push_back(_angles[i]);
    }
    type = _type;
}

vector <int> Piece::get_pos() { return angles; }

vector <Colors> Piece::get_color() { return color; }

char Piece::get_type() { return type; }

/// @brief Поворачивает элемент в направлении dir
/// @param dir Направление поворота
void Piece::rotate_piece(char dir) {
    angles = rotate_vector(angles, dir);   

    if (tolower(dir) == 'x') {
        swap(color[1], color[2]);
    } else if (tolower(dir) == 'y') {
        swap(color[0], color[2]);
    } else if (tolower(dir) == 'z') {
        swap(color[0], color[1]);
    } 
}

//_______________________________________________Cube_____________________________________________________

Cube::Cube() {
    dimension = 0;
    vector <int> angles{};
    vector <Colors> colors{};
}

/// @brief Конструктор кубика Рубика. Задает изначальное состояние кубика
/// @param n Размер кубика 
Cube::Cube(int n) {
    dimension = n;
    vector <int> angles;
    vector <Colors> colors;
    for (int layer=0; layer < n; layer++) {
        vector <Piece> temp;
        Piece new_piece;
        for (int i = 0; i < n*n; i++) {
            colors = find_colors(layer, i, n);
            angles = find_angles(layer, i, n);
            if (i == 0 || i == n-1 || i == n*n-n|| i == n*n-1) {
                if (layer == 0 || layer == n-1)
                    new_piece = Piece(colors, angles, 'C');
                else
                    new_piece = Piece(colors, angles, 'E');
            } else if (i < n-1 || i > n*n-n || i % n == 0 || i % n == n-1) {
                if (layer == 0 || layer == n-1)
                    new_piece = Piece(colors, angles, 'E');
                else
                    new_piece = Piece(colors, angles, 'M');
            } else {
                if (layer == 0 || layer == n-1)
                    new_piece = Piece(colors, angles, 'M');
                else
                    new_piece = Piece(colors, angles, '0');
            }
            temp.push_back(new_piece);
        }
        parts.push_back(temp);
    }
}

/// @brief Конструктор кубика для чтения кубика из сохранения
/// @param n Размер кубика
/// @param new_angles Набор векторов, задающих направление в пространстве для всех элементов
/// @param new_colors Набор сочетаний цветов для всех элементов
/// Создает кубик не в изначальном состоянии (в котором все грани собраны), а в заданном набором параметров new_angles и new_colors
Cube::Cube(int n, vector<vector<int>> new_angles, vector<vector<Colors>> new_colors) {
    dimension = n;
    vector <int> angles;
    vector <Colors> colors;
    char type;
    for (int layer = 0; layer < n; layer++) {
        vector<Piece> temp;
        for (int i = 0; i < n*n; i++) {
            angles = new_angles[n*n*layer + i];
            colors = new_colors[n*n*layer + i];
            if (i == 0 || i == n-1 || i == n*n-n|| i == n*n-1) {
                if (layer == 0 || layer == n-1)
                    type = 'C';
                else
                    type = 'E';
            } else if (i < n-1 || i > n*n-n || i % n == 0 || i % n == n-1) {
                if (layer == 0 || layer == n-1)
                    type = 'E';
                else
                    type = 'M';
            } else {
                if (layer == 0 || layer == n-1)
                    type = 'M';
                else
                    type = '0';
            }
            temp.push_back(Piece(colors, angles, type));
        }
        parts.push_back(temp);
    }
}

/// @brief Формирует массив чисел для вывода одной сторонs кубика
/// @param direction Сторона кубика, которую нужно вывести
vector <vector <Colors>> Cube::face_to_print(vector <int> direction) {
    vector <Colors> colors, temp;
    vector <vector <Colors>> result;
    int n = dimension;
    if (direction[1] == 1 || direction[1] == -1) {
        int slice = 0;
        direction[1] == 1 ? slice = 0 : slice = n - 1;
        for (auto i : parts[slice]) {
            colors = i.get_color();
            temp.push_back(colors[1]);
            if (temp.size() == n) {
                result.push_back(temp);
                temp.clear();
            }
        }
    } else if (direction[0] == 1 || direction[0] == -1) {
        int start = 0;
        direction[0] == 1 ? start = n - 1 : start = 0;
        for (auto slice : parts) {
            for (int i = start; i < n*n; i += n) {
                colors = slice[i].get_color();
                temp.push_back(colors[0]);
                if (temp.size() == n) {
                    result.push_back(temp);
                    temp.clear();
                }
            }
        }
        transpose(result, dimension);
        if (direction[0] == -1)
            reflect_hor(result, dimension);
    } else if (direction[2] == 1 || direction[2] == -1) {
        int start = 0, end = 0;
        if (direction[2] == -1) {
            start = 0;
            end = n;
        } else if (direction[2] == 1) {
            start = n*n - n;
            end = n*n;
        }
        for (auto slice : parts) {
            for (int i = start; i < end; i++) {
                colors = slice[i].get_color();
                temp.push_back(colors[2]);
                if (temp.size() == n) {
                    result.push_back(temp);
                    temp.clear();
                }
            }
        }
        if (direction[2] == -1)
            reflect_vert(result, dimension);
    }
    return result;
}

/// @brief Поворачивает сторону кубика
/// @param side Сторона, которую нужно повернуть
void Cube::rotate_side(char side) {
    int n = dimension;
    char t_side = tolower(side);
    
    if (t_side == 'f' || t_side == 'b') {
        rotate_z_axis(side);
    } else if (t_side == 's') {
        char t_dir = side == 's' ? 'F': 'f';
        for (int i = 1; i <= n-2; i++) {
            rotate_z_axis(t_dir, i);
        }
    } else if (t_side == 'r' || t_side == 'l') {
        rotate_x_axis(side);
    } else if (t_side == 'm') {
        char t_dir = side == 'm' ? 'r': 'R';
        for (int i = 1; i <= n-2; i++) {
            rotate_x_axis(t_dir, i);
        }
    } else if (t_side == 'u' || t_side == 'd') {
        rotate_y_axis(side);
    } else if (t_side == 'e') {
        char t_dir = side == 'e' ? 'u': 'U';
        for (int i = 1; i <= n-2; i++) {
            rotate_y_axis(t_dir, i);
        }
    }
}

/// @brief Поворачивает кубик в пространстве
/// @param dir Направление поворота
void Cube::change_direction(char dir) {
    if (tolower(dir) == 'x') {
        char t_dir = dir == 'x' ? 'R' : 'r';
        for (int i = 0; i < dimension; i++) {
            rotate_x_axis(t_dir, i);
        }
    } else if(tolower(dir) == 'y') {
        char t_dir = dir == 'y' ? 'U' : 'u';
        for (int i = 0; i < dimension; i++) {
            rotate_y_axis(t_dir, i);
        } 
    } else if(tolower(dir) == 'z') {
        char t_dir = dir == 'z' ? 'F' : 'f';
        for (int i = 0; i < dimension; i++) {
            rotate_z_axis(t_dir, i);
        } 
    }
}

/// @brief Поворачивает сторону, направленную по оси х
/// @param face Поворачиваемая сторона
/// @param start_offset Отступ от начала массива 
void Cube::rotate_x_axis(char face, int start_offset) {
    int n = dimension, offset = 0, k = 0;

    if (face == 'r' || face == 'R') {
        offset = n - 1 - start_offset;
    } else {
        offset = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(parts[i][n*j + offset], parts[j][n*i + offset]);
        }
    }

    if (face == 'r' || face == 'L') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][n*j + offset], parts[i][(n - 1 - j) * n + offset]);
            }
        }
    } else if (face == 'R' || face == 'l') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][(n-i-1)*n + offset], parts[n - 1 - j][(n-i-1)*n + offset]);
            }
        }
    }

    char dir = (face == 'R' || face == 'l') ? 'X' : 'x';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            parts[i][n*j + offset].rotate_piece(dir);
        }
    }
}

/// @brief Поворачивает сторону, направленную по оси y
/// @param face Поворачиваемая сторона
/// @param start_offset Отступ от начала массива 
void Cube::rotate_y_axis(char face, int start_offset) {
    int n = dimension;
    int slice = 0;
    int so = start_offset;

    if (tolower(face) == 'u') {
        slice = 0;
    } else if (tolower(face) == 'd') {
        slice = n - 1;
    } 

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(parts[so+slice][j*n + i], parts[so+slice][i*n + j]);
        }
    }

    int k = n/2;            // ОЧЕНЬ ВАЖНО. без этого ломаются большие кубы
    if (n > 3) k -= 1;
    if (face == 'U' || face == 'd') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                swap(parts[so+slice][i*n + j], parts[so+slice][i*n + (n - 1 - j)]);
            }
        }
    } else if (face == 'u' || face == 'D') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                swap(parts[so+slice][i], parts[so+slice][(n - 1 - j) * n + i]);
            }
        }
    }

    char dir = (face == 'U' || face == 'd') ? 'Y' : 'y';
    for (int i = 0; i < n*n; i++) {
        parts[so+slice][i].rotate_piece(dir);
    }
}

/// @brief Поворачивает сторону, направленную по оси z
/// @param face Поворачиваемая сторона
/// @param start_offset Отступ от начала массива 
void Cube::rotate_z_axis(char face, int start_offset) {
    int n = dimension, offset = 0;

    if (tolower(face) == 'f') {
        offset = n*n - n - n*start_offset;
    } else if (tolower(face) == 'b') {
        offset = 0 + n*start_offset;
    }

    for (int i = 0; i < n; i++) {
         for (int j = i + 1; j < n; j++) {
             swap(parts[i][j + offset], parts[j][i + offset]);
         }
    }

    if (face == 'f' || face == 'B')  {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][i + offset], parts[n - j - 1][i + offset]);
            }
        }
    } else if (face == 'F' || face == 'b') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][offset + j], parts[i][offset + n - 1 - j]);
            }
        }
    }

    char dir = (face == 'F' || face == 'b') ? 'z' : 'Z';
    for (int i = 0; i < n; i++) {
        for (int j = offset; j < offset + n; j++) {
            parts[i][j].rotate_piece(dir);
        }
    }
}

/// @brief Проверка того, собран ли кубик
/// @return True, если все грани правильно собраны, иначе false 
/// Проверяет по очереди все грани на то, что все цвета одинаковые
bool Cube::is_solved() {
    vector <vector <Colors>> face;
    vector <int> main_direction = {0, 1, 0};
    vector <int> direction = {0, 1, 0};

    direction = rotate_vector(main_direction, 'X');
    face = face_to_print(direction);				// задняя сторона
    if (!is_equal(face)) return false;

    direction = rotate_vector(main_direction, 'z');
    face = face_to_print(direction);				// задняя сторона
    if (!is_equal(face)) return false;

    face = face_to_print(main_direction);		// верхняя сторона
    if (!is_equal(face)) return false;

    direction = rotate_vector(main_direction, 'Z');
    face = face_to_print(direction);				// задняя сторона
    if (!is_equal(face)) return false;

    direction = rotate_vector(main_direction, 'x');
    face = face_to_print(direction);				// задняя сторона
    if (!is_equal(face)) return false;

    direction = rotate_vector(main_direction, 'x');
    direction = rotate_vector(direction, 'x');
    face = face_to_print(direction);				// задняя сторона
    if (!is_equal(face)) return false;
    return true;
}
