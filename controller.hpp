#pragma once

#include <queue>
#include <iostream>
#include <regex>
#include <algorithm>
#include <cstdlib>

#include "cube.hpp"
#include "utils.hpp"
#include "view.hpp"

#pragma GCC system_header
using std::queue, std::istream, std::cin, std::regex, std::sregex_iterator, std::regex_iterator, 
std::smatch, std::transform, std::srand, std::rand, std::time;

class Controller {
  private:
    Cube current_cube;
    View console;
    queue <char> command_sequence;
    void move(char command);
    int parse_cube_commands(string& str);
    int parse_console_commands(string& str);
  public:
    Controller() {current_cube = Cube(); console = View(); }
    void new_cube(int dim);
    void parse_input(istream& in);
    void close_app();
    void scramble();
};
