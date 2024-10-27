#pragma once

#include <queue>
#include <iostream>
#include <regex>
#include <algorithm>

#include "utils.hpp"
#include "cube.hpp"
#include "utils.hpp"
#include "view.hpp"

using std::queue, std::istream, std::regex, std::sregex_iterator, std::regex_iterator, std::smatch, std::transform;

class Controller {
  private:
    Cube current_cube;
    View console;
    queue <char> command_sequence;
    void move(char command);
  public:
    Controller() {current_cube = Cube(); console = View(); }
    void new_cube(int dim);
    int parse_cube_commands(string& str);
    int parse_console_commands(string& str);
    void parse_input(istream& in);
    void close_app();
    void scramble();
};
