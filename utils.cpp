#include "utils.h"

// Выводит вектор в окно консоли
void print_vector(vector <Colors> vec) {
    SetConsoleOutputCP(CP_UTF8);
    // char c = 254;                       // символ квадрата в Extended ASCII. Работает в Windows, не работает в Linux.
    for (auto item : vec) {
        set_color(item);
        cout << u8"\u23F9";            // символ квадрата в Unicode. Не работает в Windows, работает в Linux. 
        // cout << c;             
    }
}

// Устанавливает 8-битный цвет текста с кодом n
void set_color(Colors n) {
    string res;
    cout << "\033[38;5;" << (int) n << "m";
}

// Формирует строку отступа длиной n, состоящую из пробелов
string form_indent(int n) {
    string res;
    for (int i = 0; i < n; i++) {
        res += " ";
    }
    return res;
}

// Функция определяет направление по осям xyz, в котором будет направлен элемент,
// в зависимости от его расположения на кубике в изначальном положении. Оси направлены
// направо, наверх и на зрителя. 1 значит направление по оси, -1 направление против оси,
// 0 - на эту ось элемент не отображается, например центр будет иметь только одну координату, так
// как у него только одна "сторона" направлена наружу
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
// Функция определяет, в какие цвета будет покрашен элемент в изначальном положении.
// Для углов находит 3 цвета, для ребер 2 цвета, для центров 1 цвет. Каждый цвет записывается
// в ячейку вектора, соответствующую координатной оси. Например код 103 обозначает бело-красное ребро,
// а код 600 желтый центр
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

// Поворачивает единичный вектор по осям xyz, в соответствии с командой. Заглавная буква
// означает поворот против часовой стрелки, строчная буква - поворот по часовой стрелке.
// ось x направлена вправо, ось y вверх, ось z на зрителя
vector <int> rotate_vector(vector<int> vec, char dir) {
    int x = vec[0], y = vec[1], z = vec[2];
    double angle = 0;
    if (tolower(dir) == dir)
        angle = M_PI_2;
    else
        angle = -M_PI_2;

    if (dir == 'x' || dir == 'X') {
        y = vec[1]*cos(angle) - vec[2]*sin(angle);
        z = vec[1]*sin(angle) + vec[2]*cos(angle);
    } else if (dir == 'y' || dir == 'Y') {
        x = vec[0]*cos(angle) - vec[2]*sin(angle);
        z = vec[0]*sin(angle) + vec[2]*cos(angle);
    } else if (dir == 'z' || dir == 'Z') {
        x = vec[0]*cos(angle) - vec[1]*sin(angle);
        y = vec[0]*sin(angle) + vec[1]*cos(angle);
    } else {
        cerr << "Something went wrong!\n";
        return vec;
    }
    vec = {x, y, z};
    return vec;
}


