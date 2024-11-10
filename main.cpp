#include "controller.hpp"


int main() {
    Controller _main;
    _main.hello();
    while (1) {
        _main.parse_input(std::cin);
    }    
}