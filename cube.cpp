#include "cube.h"

Piece::Piece() {
        for (int i = 0; i < 3; i++) {
            color.push_back(Colors::K);
            angles.push_back(0);
        }
        type = '0';
}

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

// Формирует массив чисел для вывода одной сторону кубика, направленную в сторону direction
vector <vector <Colors>> Cube::face_to_print(vector <int> direction) {
    int n = 0;
    vector <int> coords;
    vector <Colors> colors, temp;
    vector <vector <Colors>> result;
    if (direction[1] != 0)
        n = 1;
    else if (direction[2] != 0)
        n = 2;
    for (auto slice : parts) {
        for (auto elem : slice) {
            colors = elem.get_color();
            coords = elem.get_pos();
            if (coords[n] == direction[n])
                temp.push_back(colors[n]);
            if (temp.size() == dimension) {
                result.push_back(temp);
                temp.clear();
            }
        }
    }
    return result;
}
