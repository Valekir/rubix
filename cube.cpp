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
    out << "\tID: " << p.get_id() << std::endl;
    return out;
}

Piece::Piece() {
        for (int i = 0; i < 3; i++) {
            color.push_back(Colors::K);
            angles.push_back(0);
        }
        type = '0';
}

Piece::Piece(vector <Colors> _color, vector <int> _angles, char _type, int id) {
    for (int i = 0; i < 3; i++) {
        color.push_back(_color[i]);
        angles.push_back(_angles[i]);
    }
    type = _type;
    ID = id;
}

vector <int> Piece::get_pos() { return angles; }

vector <Colors> Piece::get_color() { return color; }

char Piece::get_type() { return type; }

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

Cube::Cube() {
    dimension = 0;
    vector <int> angles{};
    vector <Colors> colors{};
}

// Конструктор кубика Рубика. Задает изначальное состояние кубика, размером NxNxN
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
                    new_piece = Piece(colors, angles, 'C', layer*9+i);
                else
                    new_piece = Piece(colors, angles, 'E', layer*9+i);
            } else if (i < n-1 || i > n*n-n || i % n == 0 || i % n == n-1) {
                if (layer == 0 || layer == n-1)
                    new_piece = Piece(colors, angles, 'E', layer*9+i);
                else
                    new_piece = Piece(colors, angles, 'M', layer*9+i);
            } else {
                if (layer == 0 || layer == n-1)
                    new_piece = Piece(colors, angles, 'M', layer*9+i);
                else
                    new_piece = Piece(colors, angles, '0', layer*9+i);
            }
            temp.push_back(new_piece);
        }
    parts.push_back(temp);
    }
}

// Формирует массив чисел для вывода одной сторону кубика, направленную в сторону direction
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

void Cube::rotate_side(char side) {
    int n = dimension;
    switch (side) {
    case 'R':
        for (int i = 1; i < n + 1; i++) {
            for (int j = i + 1; j < n + 1; j++) {
                swap(parts[i - 1][j*n - 1], parts[j - 1][i*n - 1]);
            }
        }

        for (int i = 1; i <= n/2; i++) {
            for (int j = 1; j <= n; j++) {
                swap(parts[i - 1][j*n - 1], parts[n - i][j*n - 1]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 1; j <= n; j++) {
                parts[i][j*n - 1].rotate_piece('x');
            }
        }
        break;
    case 'r':
        for (int i = 1; i < n + 1; i++) {
            for (int j = i + 1; j < n + 1; j++) {
                swap(parts[i - 1][j*n - 1], parts[j - 1][i*n - 1]);
            }
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n/2; j++) {
                swap(parts[i - 1][j*n - 1], parts[i - 1][n*n - j]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 1; j <= n; j++) {
                parts[i][j*n - 1].rotate_piece('X');
            }
        }
        break;
    case 'L':

       for (int i = 1; i < n + 1; i++) {
            for (int j = i; j < n; j++) {
                swap(parts[i - 1][j * n], parts[j][(i - 1) * n]);
            }
        }

        for (int i = 1; i <= n/2; i++) {
            for (int j = 1; j <= n; j++) {
                swap(parts[j-1][i-1], parts[j-1][(n-i)*n]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 1; j <= n; j++) {
                parts[i][(j - 1) * n].rotate_piece('x');
            }
        }
        break;
    case 'l':
       for (int i = 1; i < n + 1; i++) {
            for (int j = i; j < n; j++) {
                swap(parts[i - 1][j * n], parts[j][(i - 1) * n]);
            }
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n/2; j++) {
                swap(parts[j - 1][(i - 1) * n], parts[n-j][(i - 1) * n]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 1; j <= n; j++) {
                parts[i][(j - 1) * n].rotate_piece('X');
            }
        }
        break;
    case 'M':
        break;
    case 'm':
        break;
    case 'U':
        rotate_clockwise(parts[0], n);
        for (int i = 0; i < n*n; i++) {
            parts[0][i].rotate_piece('y');
        }
        break;
    case 'u':
        rotate_counter(parts[0], n);
        for (int i = 0; i < n*n; i++) {
            parts[0][i].rotate_piece('Y');
        }
        break;
    case 'D':
        rotate_counter(parts[n-1], n);
        for (int i = 0; i < n*n; i++) {
            parts[n-1][i].rotate_piece('y');
        }
        break;
    case 'd':
        rotate_clockwise(parts[n-1], n);
        for (int i = 0; i < n*n; i++) {
            parts[n-1][i].rotate_piece('Y');
        }
        break;
    case 'E':
        for (int i = 1; i < n - 1; i++) {
            rotate_clockwise(parts[i], n);
            for (int j = 0; j < n*n; j++) {
                parts[i][j].rotate_piece('y');
            }
        }
        break;
    case 'e':
        for (int i = 1; i < n - 1; i++) {
            rotate_counter(parts[i], n);
            for (int j = 0; j < n*n; j++) {
                parts[i][j].rotate_piece('Y');
            }
        }
        break;
    case 'F':
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(parts[i][n*n - n + j], parts[j][n*n - n + i]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][n*n - j - 1], parts[i][n*n - n + j]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = n*n - n; j < n*n; j++) {
                parts[i][j].rotate_piece('z');
            }
        }
        break;
    case 'f':
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(parts[i][n*n - n + j], parts[j][n*n - n + i]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][n*n - n + i], parts[n - j - 1][n*n - n + i]);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = n*n - n; j < n*n; j++) {
                parts[i][j].rotate_piece('Z');
            }
        }
        break;
    case 'B':
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(parts[i][j], parts[j][i]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][i], parts[n-j-1][i]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                parts[i][j].rotate_piece('Z');
            }
        }
        break;
    case 'b':
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                swap(parts[i][j], parts[j][i]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][j], parts[i][n-1-j]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                parts[i][j].rotate_piece('Z');
            }
        }
        break;
    case 'S':
        break;
    case 's':
        break;
    default:
        break;
    }
}

void Cube::change_direction(char dir) {
    switch (dir) {
    case 'x':
        rotate_side('R');
        rotate_side('M');
        rotate_side('L');
        break;
    case 'X':
        rotate_side('r');
        rotate_side('m');
        rotate_side('l');
        break;
    case 'y':
        rotate_side('U');
        rotate_side('E');
        rotate_side('D');
        break;
    case 'Y':
        rotate_side('u');
        rotate_side('e');
        rotate_side('d');
        break;
    case 'z':
        rotate_side('F');
        rotate_side('S');
        rotate_side('B');
        break;
    case 'Z':
        rotate_side('f');
        rotate_side('s');
        rotate_side('b');
        break;
    default:
        break;
    }
}

void Cube::rotate_x_axis(char dir) {
    int n = dimension;
    int offsetX = 0, offsetY = 0, offsetZ = 0;
    if (tolower(dir) == 'r') {
        offsetX = 2;
    } else if (tolower(dir) == 'l') {
        offsetY = -2;
    } else if (tolower(dir) == 'm') {
        offsetZ = 0;
    }

    // Транспонирование
    for (int i = 1; i <= n; i++) {
        for (int j = i; j < n; j++) {
            printf("(%d;%d) (%d;%d)\n", i - 1, j*n + offsetX, j, i*n - 1 + offsetY);
        }
    }
}
