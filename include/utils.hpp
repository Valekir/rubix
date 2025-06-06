#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef M_PI_2
float M_PI_2 = 1.57079632679489661923;
#endif

using std::vector, std::swap;

// Проверяет, существует ли файл
bool file_exists(std::string filename);

// Удаляет сохранение
void delete_save(std::string filename);

// Удаляет сроку str из файла filename
void remove_string(std::string filename, std::string str);

// Возвращает массив с именами всех сохранений
std::vector<std::string> load_saves();

// Функция для обновления конфиг файла
void update_config(const std::string& filename, const std::unordered_map<std::string, std::string>& updates);

// Читает настройки из конфиг файла
std::unordered_map<std::string, std::string> load_config(const std::string& filename);

/// @brief Заполняет участок консоли цветом color
/// @param x Начальная координата x
/// @param y Начальная координата y
/// @param n Размер квадрата
void fill_area(int x, int y, int n, int color);


/// @brief набор цветов. 
/// @details W - белый G - зеленый R - красный O - оранжевый 
/// B - синий Y - желтый K - key - черный.
enum class Colors {
    W, G, R, O, B, Y, K
};

/// @brief Приводит объект типа Colors к типу char
char colortochar (Colors col);

Colors chartocolor (char col);


/// @brief Выводит вектор (vec*scale) цветов в окно консоли
/// @param vec Исходный вектор 
/// @param color Набор цветов
/// @param scale Коэффициент "растягивания" вектора
void print_vector(vector <Colors> vec, std::map<Colors, int> color, int scale);

/// @brief Устанавливает 8-битный цвет заливки 
/// @param n Код цвета
void set_background_color(int n);

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

//____________________________________________________Timer___________________________________________________

/// @brief Класс секундомера
class Timer {
 private:
	// Время начала
    std::chrono::high_resolution_clock::time_point start_time;
	// Время окончания
    std::chrono::high_resolution_clock::time_point end_time;
	bool running;

 public:
    // Состояние секундомера
    Timer() : running(false) {}
    // Метод для старта секундомера
    void start();
    // Метод для остановки секундомера и вывода времени
    void stop(bool print=true);
    bool is_running() { return running; }
};

template <typename T>
bool is_equal(vector<vector<T>> vec) {
    int n = vec.size();
    T first = vec[0][0];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (vec[i][j] != first)
                return false;
        }
    }
    return true;
}
