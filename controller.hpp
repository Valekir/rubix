#pragma once

#include <queue>
#include <iostream>
#include <regex>
#include <algorithm>
#include <cstdlib>

#include "cube.hpp"
#include "utils.hpp"
#include "view.hpp"

class Controller {
  private:
    Cube current_cube;
    View console;
    std::queue <char> command_sequence;
    void move(char command);
    int parse_cube_commands(std::string& str);
    int parse_console_commands(std::string& str);
  public:
    Controller() {current_cube = Cube(); console = View(); }
    void new_cube(int dim);
    void parse_input(std::istream& in);
    void close_app();
    void scramble();
    void hello();
};
