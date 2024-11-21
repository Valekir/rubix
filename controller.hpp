#pragma once

#include <queue>
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
    View console;
    std::queue <char> command_sequence;
    void move(char command);
    // Ищет в строке команды для вращения кубика 
    int parse_cube_commands(std::string& str);
    // Ищет в строке команды для взаимодействия с консолью
    int parse_console_commands(std::string& str);
  public:
    Controller() {current_cube = Cube(3); console = View(); }
	// Ищет в строке команды управления меню
	int parse_menu_commands(std::string& in);
	// Создает новый кубик
	void new_cube(int dim);
    //  Завершает работу программы
    void close_app();
    // Запутывает кубик
    void scramble();
    // Запускает игру
    void game();
    // Запускает меню
    void menu();

    // При старте игры выводит сообщение, описывающее возможные команды для взаимодействия с кубиком
    void hello_game();
    // При запуске приложения выводит сообщение, описывающее возможные команды для управления меню
	  void hello_menu();

};
