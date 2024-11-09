#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#ifndef M_PI_2
float M_PI_2 = 1.57079632679489661923;
#endif

using std::vector, std::swap;

// Первый набор цветов. 
// W - белый G - зеленый R - красный O - оранжевый 
// B - синий Y - желтый K - key - черный.
enum class Colors {
    W = 15, G = 40, R = 196, O = 208, B = 12, Y = 11, K = 0
};

// Выводит набор цветов в окно консоли
void print_vector(vector <Colors> vec);

// Устанавливает 8-битный цвет заливки с кодом n
void set_background_color(Colors n);

// Формирует строку отступа длиной n, состоящую из пробелов
std::string form_indent(int n);

// Функция определяет направление по осям xyz, в котором будет направлен элемент,
// в зависимости от его расположения на кубике в изначальном положении. Оси направлены
// направо, наверх и на зрителя. 1 значит направление по оси, -1 направление против оси,
// 0 - на эту ось элемент не отображается, например центр будет иметь только одну координату, так
// как у него только одна "сторона" направлена наружу
vector <int> find_angles(int layer, int a, int n);

// 1 - белый 2 - зеленый 3 - красный 4 - оранжевый 5 - синий 6 - 0 - нет цвета
// Функция определяет, в какие цвета будет покрашен элемент в изначальном положении.
// Для углов находит 3 цвета, для ребер 2 цвета, для центров 1 цвет. Каждый цвет записывается
// в ячейку вектора, соответствующую координатной оси. Например код 103 обозначает бело-красное ребро,
// а код 600 желтый центр
vector <Colors> find_colors(int layer, int a, int n);

// Поворачивает единичный вектор по осям xyz, в соответствии с командой. Заглавная буква
// означает поворот против часовой стрелки, строчная буква - поворот по часовой стрелке.
// ось x направлена вправо, ось y вверх, ось z на зрителя
vector <int> rotate_vector(vector<int> vec, char dir);

// Поворачивает элементы квадратной матрицы против часовой стрелки
template<typename T>
void rotate_clockwise(std::vector<T>& matrix, int n) {
    vector<T> result(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j * n + i] = matrix[i * n + j];
        }
    }
    matrix = result;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            swap(matrix[i * n + j], matrix[i * n + (n - 1 - j)]);
        }
    }
}

// Поворачивает элементы квадратной матрицы по часовой стрелке
template <typename T>
void rotate_counter(vector<T>& matrix, int n) {
    vector<T> result(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[j * n + i] = matrix[i * n + j];
        }
    }
    matrix = result;
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            swap(matrix[i * n + j], matrix[(n - 1 - i) * n + j]);
        }
    }
}


// Транспонирует квадратную матрицу
template <typename T>
void transpose(vector<T>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
}

// Отражает квадратную матрицу по вертикали
template <typename T>
void reflect_vert(vector<vector <T>>& matrix, int n) {
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            swap(matrix[i][j], matrix[n - 1 - i][j]);
        }
    }
}

template <typename T>
void reflect_hor(vector<vector<T>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            swap(matrix[i][j], matrix[i][n - 1 - j]);
        }
    }
}