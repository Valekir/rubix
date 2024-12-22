#pragma once
#include <vector>
#include <iostream>
#include <unistd.h>
using std::vector;

#include "utils.hpp"

/// @brief Класс, который хранит в себе цвета, направление по осям и тип (угол, ребро, центр) элемента
/// @details В приватных полях хранит цвет элемента (от 1 до 3 значений, в зависимости от типа),
/// направление элемента по осям (x, y, z), тип элемента (угол, ребро, центр)
class Piece{
 protected:
	vector <Colors> color;
	vector <int> angles;
	char type;
 public:
	vector <int> get_pos();
	vector <Colors> get_color();
	char get_type();
	void rotate_piece(char dir);
	
	Piece();
	Piece(vector <Colors> _color, vector <int> _angles, char _type);
};

/// @brief Класс, который является симуляцией кубика Рубика. Хранит в себе набор элементов (углы, ребра, центры)
/// @details В приватных полях хранит двумерный массив элементов. Первое измерение массива - слой кубика.
/// Слои пронумерованы сверху вниз. Второе измерение массива - элементы на одном слое. Элементы пронумерованы
/// слева направо, сверху вниз. 
/// В изначальном состоянии кубик собран
class Cube {
 private:
    vector<vector <Piece>> parts;
    int dimension;
    void rotate_x_axis(char face, int start_offset=0);
    void rotate_y_axis(char face, int start_offset=0);
    void rotate_z_axis(char face, int start_offset=0); 
 public:
    Cube(int, vector<vector<int>>, vector<vector<Colors>>);
    explicit Cube(int);
    Cube();
    vector<vector <Piece>> get_parts() { return parts; }
    /// @brief Возвращает размер кубика
    int size() { return dimension; }
    vector <vector <Colors>> face_to_print(vector <int> direction);
    void rotate_side(char side);
    void change_direction(char dir);
};
