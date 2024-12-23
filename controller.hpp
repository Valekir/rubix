#pragma once

#include <queue>
#include <unordered_map>
#include <iostream>
#include <regex>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <ncurses.h>

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
		{"scrambled", false}
	};
	
	int difficulty = 20;
	Timer timer;
    
	// Поворот грани кубика / кубика полностью
	void move(char command);
    // Ищет в строке команды для вращения кубика 
    int parse_cube_commands(std::string& str);
    // Ищет в строке команды для взаимодействия с консолью
    int parse_console_commands(std::string& str);
	// Ищет в строке команды для задания настроек
	int parse_settings(std::string& str);
	
	void load_saved_cube(std::string filename);

  public:
    Controller() {current_cube = Cube(3); console = nullptr; }
	~Controller() { if (console != nullptr) delete console; }
	// Ищет в строке команды управления меню
	int parse_menu_commands(std::string& in);
	// Создает новый кубик
	void new_cube(int dim);
    // Запутывает кубик
    void scramble();

	void resize(WINDOW*, int, int);
    // Запускает игру
	void game(bool from_save=false, std::string filename="");

    // При старте игры выводит сообщение, описывающее возможные команды для взаимодействия с кубиком
    void hello_game();
    // При запуске приложения выводит сообщение, описывающее возможные команды для управления меню
	void hello_menu();
	// При открытии настроек выводит сообщение со всеми текущими настройками
	void print_settings();
	void clear() {console->clear();}
	// Сохраняет текущую игру
	void save();
	void load_settings();
};
