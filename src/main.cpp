#include <csignal>
#include <iostream>

#include "controller.hpp"
#include "menu.hpp"
#include "utils.hpp"

#include "solver.hpp"
#include "cube.hpp"
#include "benchmark.hpp"


// int game();
// void solver();


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
        // solver();
    }

    if (mode == 3) {
        Benchmark benchmark;
        benchmark.select_test();
        benchmark.run();
    }

    return 0;
}

// void scramble_cube(SCube& cube) {
//     int n = 0;

//     cube = SCube(Cube(3));

//     std::cout << "Enter number of moves to scramble between 1 and 20: ";
//     while (n <= 0 || n > 20) {
//         std::cin >> n;
//         if (n <= 0 || n >= 20) {
//             std::cout << "number of moves must be between 1 and 20" << std::endl;

//         }
//     }
//     cube.scramble(n);
// }

// void solver() {
//     SCube cube(Cube(3));
//     Timer timer;
    
//     scramble_cube(cube);

//     while (1) {
//         int search_mode = -1;
//         string result = "";

//         std::cout << "Enter 4 to re-scramble cube" << std::endl;
//         std::cout << "Choose search algorithm:\n(1): BFS\n(2): DFS\n(3): IDA*" << std::endl;
//         std::cout << "Enter 0 to exit program" << std::endl;
        
//         while (search_mode < 0 || search_mode > 3) {
//             std::cin >> search_mode;

//             if (search_mode == 4) {
//                 scramble_cube(cube);
//             }
            
//             if (search_mode < 0 || search_mode > 3) {
//                 std::cout << "Search mode must be 1 (BFS) or 2 (DFS) or 3 (IDA*)" << std::endl;
//             }
//         }
    
//         timer.start();
    
//         switch (search_mode) {
//             case 0: 
//             {
//                 return;
//             }
//             case 1:
//             {
//                 result = solve_cube_bfs(cube);
//                 break;
//             }
//             case 2:
//             {
//                 result = solve_cube_dfs(cube);
//                 break;
//             }
//             case 3:
//             {
//                 result = solve_cube_IDAstar(cube);
//                 break;
//             }
//         }

//         if (result != "") {
//             std::cout << "\n\033[32mSolution: " << result  << "\033[0m" << std::endl;
//         } else {
//             std::cout << "\n\033[31mSolution not found\033[0m" << std::endl;
//         }
    
//         timer.stop(result != "");
//         std::cout << std::endl;
//     }
// }
