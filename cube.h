#pragma once
#include <vector>
using std::vector;

#include "utils.h"

// Класс, который хранит в себе цвета, направление по осям и тип (угол, ребро, центр) элемента
class Piece{
 protected:
    vector <Colors> color;
    vector <int> angles;
    char type;
 public:
    Piece();
    Piece(vector <Colors> _color, vector <int> _angles, char _type);
    vector <int> get_pos();
    vector <Colors> get_color();
    char get_type();
};
// Класс, который является симуляцией кубика Рубика. Хранит в себе набор элементов (углы, ребра, центры)
class Cube {
 private:
    vector<vector <Piece>> parts;
    int dimension;
 public:
    // Конструктор кубика Рубика. Задает изначальное состояние кубика, размером NxNxN
    explicit Cube(int);
    vector<vector <Piece>> get_parts() { return parts; }
    // Формирует массив чисел для вывода одной сторону кубика, направленную в сторону direction
    int size() { return dimension; }
    vector <vector <Colors>> face_to_print(vector <int>);
};
