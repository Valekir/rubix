#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

using std::cout, std::endl, std::vector, std::ifstream, std::string;

#include "cube.h"
#include "utils.h"

// Класс, ответственный за отрисовку кубика в окне консоли
class View {
 private:
   int style;
   vector <int> main_direction;
   
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
   void help();
   // Выводит развертку кубика, в зависимости от выбранного режима развертки
   void print_cube(Cube);
};
