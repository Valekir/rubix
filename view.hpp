#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cassert>

#include "cube.hpp"
#include "utils.hpp"

// W = 15, G = 40, R = 196, O = 208, B = 12, Y = 11, K = 0

/// @brief Класс, ответственный за отрисовку кубика в окне консоли
/// @details В приватных полях хранит текущий режим развертки - style и направление взгляда - main_direction
class View {
 protected:
    // Количество отображаемых сторон
    int style;
    // Направление взгляда на кубик
    std::vector <int> main_direction;
    // Набор цветов кубика
    std::map <Colors, int> cube_colors = {
      {Colors::W, 15},  {Colors::G, 40},
      {Colors::R, 196}, {Colors::O, 208},
      {Colors::B, 12},  {Colors::Y, 11},
      {Colors::K, 0}
    };
    // Выводит развертку для трех сторон
    void print_three_face(Cube);
    // Выводит развертку для пяти сторон
    void print_five_face(Cube);
    // Выводит развертку для шести сторон
    void print_six_face(Cube);
    // Выводит вектор цветов
    virtual void print_vector(vector <Colors> vec);
	// Выводит одну сторону кубика начиная из координат (x, y)
	virtual void print_face(vector <vector<Colors>> face, int x, int y, int size);
 public:
    View();
    // изменяет количество сторон, отображаемых в развертке
    void set_style(int);
    // Очищает окно консоли
    void clear();
    // Рисует развертку кубика в консоли, начиная с строки n
    virtual void print_cube(Cube, int);
    // Очищает строку ввода команд
    void clear_line();
	// Выводит описание языка вращений кубика
	void help();
   // Устанавливает новые цвета для кубика
   void set_colors(std::map<Colors, int>);
};


class ScalableWindow : public View {
 private:
    int scale_factor = 2;
    // Находит максимально возможное разрешение картинки в зависимости от размера консоли
    int find_scale();
    // Выводит вектор цветов, растягивая его в зависимости от размера окна
    void print_vector(vector <Colors> vec) override;
 public:
    // Выводит кубик с размерами, зависящими от размера окна
	void print_cube(Cube, int) override;
};

