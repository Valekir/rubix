#include "utils.hpp"

using std::cout, std::cerr, std::endl, std::cos, std::sin, std::string;


void Timer::start() {
    if (!running) {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }
}

void Timer::stop() {
    if (running) {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Time: " << duration.count() << " ms.\n";
    }
}

// Устанавливает 8-битный цвет заливки с кодом n
void set_background_color(int n) {
    cout << "\033[48;5;" << (int) n << "m";
}

// Формирует строку отступа длиной n, состоящую из пробелов
string form_indent(int n) {
    string res;
    for (int i = 0; i < 2*n; i++) {
        res += " ";
    }
    return res;
}

// Функция определяет направление по осям xyz, в котором будет направлен элемент
vector <int> find_angles(int layer, int i, int dim) {
    vector <int> angles;

    if (i % dim == dim-1)               // правая сторона
        angles.push_back(1);
    else if (i % dim == 0)              // левая сторона
        angles.push_back(-1);
    else
        angles.push_back(0);

    if (layer == 0)                     // верхняя сторона
        angles.push_back(1);
    else if (layer == dim-1)            // нижняя сторона
        angles.push_back(-1);
    else
        angles.push_back(0);

    if (i > dim*dim - dim - 1)          // передняя сторона
        angles.push_back(1);
    else if (i < dim)                   // задняя сторона
        angles.push_back(-1);
    else
        angles.push_back(0);
    return angles;
}

// 1 - белый 2 - зеленый 3 - красный 4 - оранжевый 5 - синий 6 - 0 - нет цвета
// Поворачивает вектор по осям xyz, в соответствии с командой. 
vector <Colors> find_colors(int layer, int i, int dim) {
    vector <Colors> colors;

    if (i % dim == dim-1)
        colors.push_back(Colors::R);
    else if (i % dim == 0)
        colors.push_back(Colors::O);
    else
        colors.push_back(Colors::K);

    if (layer == 0)
        colors.push_back(Colors::W);
    else if (layer == dim-1)
        colors.push_back(Colors::Y);
    else
        colors.push_back(Colors::K);

    if (i > dim*dim - dim - 1)
        colors.push_back(Colors::G);
    else if (i < dim)
        colors.push_back(Colors::B);
    else
        colors.push_back(Colors::K);

    return colors;
}

// Поворачивает единичный вектор по осям xyz, в соответствии с командой. 
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
