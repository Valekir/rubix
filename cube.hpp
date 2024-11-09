#pragma once
#include <vector>
#include <iostream>
using std::vector;

#include "utils.hpp"

// Класс, который хранит в себе цвета, направление по осям и тип (угол, ребро, центр) элемента
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

// Класс, который является симуляцией кубика Рубика. Хранит в себе набор элементов (углы, ребра, центры)
class Cube {
 private:
    vector<vector <Piece>> parts;
    int dimension;
 public:
    // Конструктор кубика Рубика. Задает изначальное состояние кубика, размером NxNxN
    explicit Cube(int);
    Cube();
    vector<vector <Piece>> get_parts() { return parts; }
    // Формирует массив чисел для вывода одной сторону кубика, направленную в сторону direction
    int size() { return dimension; }
    vector <vector <Colors>> face_to_print(vector <int>);
    void rotate_side(char side);
    void change_direction(char dir);
    void rotate_x_axis(char dir);
};
