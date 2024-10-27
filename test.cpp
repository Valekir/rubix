#include <vector>
#include <iostream>
#include <cmath>
#ifndef M_PI_2
float M_PI_2 = 1.57079632679489661923;
#endif
using namespace std;

enum class Colors {
    W = 15, G = 40, R = 196, O = 208, B = 12, Y = 11, K = 0
};

vector <int> rotate_vector(vector<int> vec, char dir) {
    int x = vec[0], y = vec[1], z = vec[2];
    double angle = 0;
    if (tolower(dir) == dir)
        angle = M_PI_2;
    else
        angle = -M_PI_2;
    int c = cos(angle);
    int s = sin(angle);
    if (dir == 'x' || dir == 'X') {
        y = vec[1]*c - vec[2]*s;
        z = vec[1]*s + vec[2]*c;
    } else if (dir == 'y' || dir == 'Y') {
        x = vec[0]*c - vec[2]*s;
        z = vec[0]*s + vec[2]*c;
    } else if (dir == 'z' || dir == 'Z') {
        x = vec[0]*c - vec[1]*s;
        y = vec[0]*s + vec[1]*c;
    } else {
        cerr << "Something went wrong!\n";
        return vec;
    }
    vec = {x, y, z};
    return vec;
}

class Piece{
 protected:
   vector <Colors> color;
   vector <int> angles;
   char type;
   int ID;
 public:
   Piece();
   Piece(vector <Colors> _color, vector <int> _angles, char _type, int id);
   vector <int> get_pos();
   vector <Colors> get_color();
   char get_type();
   void rotate_piece(char dir);
   int get_id() {return ID;}
};

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
    out << "\tID: " << p.get_id() << endl;
    return out;
}



int main() {
    Piece p;
    vector <int> angles = {1, 1, 1};
    vector <Colors> colors = {Colors::R, Colors::W, Colors::G};
    p =  Piece(colors, angles, 'C', 1);
    char c = 'a';
    cout << p << endl;
    while (1) {
        cin >> c;
        p.rotate_piece(c);
        cout << p;
    }

}
