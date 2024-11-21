#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

#include "cube.hpp"
#include "utils.hpp"

/// @brief Класс, ответственный за отрисовку кубика в окне консоли
/// @details В приватных полях хранит текущий режим развертки - style и направление взгляда - main_direction
class View {
 private:
    // Количество отображаемых сторон
    int style;
    // Направление взгляда на кубик
    std::vector <int> main_direction;
    // Выводит развертку для трех сторон
    void print_three_face(Cube);
    // Выводит развертку для пяти сторон
    void print_five_face(Cube);
    // Выводит развертку для шести сторон
    void print_six_face(Cube);
 public:
    View();
    // изменяет количество сторон, отображаемых в развертке
    void set_style(int);
    // Очищает окно консоли
    void clear();
    // Выводит в консоль справку по приложению
    void print_cube(Cube);
    // Очищает строку ввода команд
    void clear_line();
	// Выводит описание языка вращений кубика
	void help();
};
