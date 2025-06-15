#include <csignal>
#include <iostream>

#include "controller.hpp"
#include "menu.hpp"
#include "utils.hpp"

#include "solver.hpp"
#include "cube.hpp"
#include "benchmark.hpp"


int main(int argc, char** argv) {
    Controller controller;
    int mode = 0;

    std::cout << "Choose mode: \n(1): game\n(2): solver\n(3): benchmark" << std::endl;
    while (mode <= 0 || mode > 3) {
        std::cin >> mode;
        if (mode <= 0 || mode > 3) {
            std::cout << "Mode has to be 1 (game) 2 (solver) or 3 (benchmark)" << std::endl;
        }
    }

    if (mode == 1) {
        controller.open_game_menu();
    }

    if (mode == 2) {
        Solver solver;
        solver.solve_cube();
    }

    if (mode == 3) {
        Benchmark benchmark;
        benchmark.select_test();
        benchmark.run();
    }

    return 0;
}
