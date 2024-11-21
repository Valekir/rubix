#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#ifndef M_PI_2
float M_PI_2 = 1.57079632679489661923;
#endif

using std::vector, std::swap;

/// @brief набор цветов. 
/// @details W - белый G - зеленый R - красный O - оранжевый 
/// B - синий Y - желтый K - key - черный.
enum class Colors {
    W = 15, G = 40, R = 196, O = 208, B = 12, Y = 11, K = 0
};

/// @brief Выводит набор цветов в окно консоли
/// @param vec Исходный вектор 
void print_vector(vector <Colors> vec);

/// @brief Устанавливает 8-битный цвет заливки 
/// @param n Код цвета
void set_background_color(Colors n);

/// @brief Формирует строку отступа, состоящую из пробелов
/// @param n Длина строки
std::string form_indent(int n);

/// @brief определяет направление по осям xyz, в котором будет направлен элемент,
/// в зависимости от его расположения на кубике в изначальном положении. 
/// @param layer Слой, на котором расположен элемент 
/// @param a Номер элемнта на слое
/// @param n Размер кубика
/// @return Вектор, отображающий ориентацию элемента в пространстве
/// @details направлены направо, наверх и на зрителя. 1 значит направление по оси, -1 направление против оси,
/// 0 - на эту ось элемент не отображается, например центр будет иметь только одну координату, так
/// как у него только одна "сторона" направлена наружу
vector <int> find_angles(int layer, int a, int n);

/// @brief Функция определяет, в какие цвета будет покрашен элемент в изначальном положении.
/// @param layer Слой, на котором расположен элемент 
/// @param a Номер элемнта на слое
/// @param n Размер кубика
/// @return Массив, содержащий цвета, в которые будет покрашен элемент
/// @details Для углов находит 3 цвета, для ребер 2 цвета, для центров 1 цвет. Каждый цвет записывается
/// в ячейку вектора, соответствующую координатной оси. Например код 103 обозначает бело-красное ребро,
/// а код 600 желтый центр
vector <Colors> find_colors(int layer, int a, int n);

/// @brief Поворачивает вектор по осям xyz, в соответствии с командой. 
/// @param vec Изначальный вектор
/// @param dir Направление поворота
/// @return Изначальный вектор, повернутый в направлении dir
/// @details Заглавная буква означает поворот против часовой стрелки, строчная буква - поворот по часовой стрелке.
/// ось x направлена вправо, ось y вверх, ось z на зрителя
vector <int> rotate_vector(vector<int> vec, char dir);

/// @brief Транспонирует квадратную матрицу
/// @param matrix Исходная матрица 
/// @param n Размер матрицы
template <typename T>
void transpose(vector<T>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(matrix[i][j], matrix[j][i]);
        }
    }
}

/// @brief Отражает квадратную матрицу по вертикали
/// @param matrix Исходная матрица
/// @param n Размер матрицы
template <typename T>
void reflect_vert(vector<vector <T>>& matrix, int n) {
    for (int i = 0; i < n / 2; i++) {
        for (int j = 0; j < n; j++) {
            swap(matrix[i][j], matrix[n - 1 - i][j]);
        }
    }
}

/// @brief Отражает квадратную матрицу по горизонтали
/// @param matrix Исходная матрица
/// @param n Размер матрицы
template <typename T>
void reflect_hor(vector<vector<T>>& matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            swap(matrix[i][j], matrix[i][n - 1 - j]);
        }
    }
}
