#pragma once

#include <queue>
#include <unordered_map>
#include <iostream>
#include <regex>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>

#include "cube.hpp"
#include "utils.hpp"
#include "view.hpp"

/// @brief Класс, ответственный за управление программой
/// @details Хранит текущий кубик, а также объект класса View, который управляет выводом в консоль модели кубика.
class Controller {
  private:
    Cube current_cube;
    View* console;
    std::queue <char> command_sequence;

	// Флаги для управления состоянием игры
	std::unordered_map <std::string, bool> flags {
		{"show_help", true}, 
		{"timer", false},
		{"scalable", false},
	};
	
	// Отступ от верха консоли, если отображается справка по игре
	int help_indent = 7;
	int difficulty = 20;
	Timer timer;
    
	// Поворот грани кубика / кубика полностью
	void move(char command);
    // Ищет в строке команды для вращения кубика 
    int parse_cube_commands(std::string& str);
    // Ищет в строке команды для взаимодействия с консолью
    int parse_console_commands(std::string& str);
	// Загружает настройи игры
	void load_settings();
	// Обновляет окно консоли при изменении размера окна
	void rescale();

  public:
    Controller() {current_cube = Cube(3); }
	~Controller() { delete console; }
    // Запутывает кубик
    void scramble();
    // Запускает игру
    void game();
    // При старте игры выводит сообщение, описывающее возможные команды для взаимодействия с кубиком
    void hello_game();
	void clear() {console->clear();}
};
