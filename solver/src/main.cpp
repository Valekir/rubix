#include "controller.hpp"
#include "menu.hpp"
#include <csignal>

#include "solver.hpp"
#include "cube.hpp"

#include <iostream>
#include "utils.hpp"


int game();
void test();

int main(int argc, char** argv) {
    int mode = 0;

    std::cout << "Choose mode: \n(1): game\n(2): solver" << std::endl;
    while (mode != 1 && mode != 2) {
        std::cin >> mode;
        if (mode != 1 && mode != 2) {
            std::cout << "Mode has to be 1 (game) or 2 (solver)" << std::endl;
        }
    }

    if (mode == 1) {
        game();
    }

    if (mode == 2) {
        test();
    }

    return 0;
}


void test() {
    Cube defaultCube(3);
    SCube cube(defaultCube);
    Timer timer;
    
    int n = 0;
    std::cout << "Enter number of moves to scramble between 1 and 20: ";
    while (n <= 0 || n > 20) {
        std::cin >> n;
        if (n <= 0 || n >= 20) {
            std::cout << "number of moves must be between 1 and 20" << std::endl;
        }
    }

    
    cube.scramble(n);

    while (1) {
    int search_mode = -1;
        string result = "";
    
        std::cout << "Choose search algorithm:\n(1): BFS\n(2): DFS\n(3): IDA*\nOr enter 0 to exit programm" << std::endl;
        while (search_mode < 0 || search_mode > 3) {
            std::cin >> search_mode;
            if (search_mode < 0 || search_mode > 3) {
                std::cout << "Search mode must be 1 (BFS) or 2 (DFS) or 3 (IDA*)" << std::endl;
            }
        }
    
        timer.start();
    
        switch (search_mode) {
            case 0: 
            {
                return;
            }
            case 1:
            {
                result = solve_cube_bfs(cube);
                break;
            }
            case 2:
            {
                result = solve_cube_dfs(cube);
                break;
            }
            case 3:
            {
                result = solve_cube_IDAstar(cube);
                break;
            }
        }

        if (result != "") {
            std::cout << "\n\033[32mSolution: " << result  << "\033[0m" << std::endl;
        } else {
            std::cout << "\n\033[31mSolution not found\033[0m" << std::endl;
        }
    
        timer.stop(result != "");
        std::cout << std::endl;
    }
}



void signal_handler(int sign) {
    endwin();
    system("clear");
    exit(0);
}

int game() {
    Controller _main;

    initscr();               // Инициализация ncurses
    cbreak();                // Включаем режим распознавания символов
    noecho();                // Отключаем отображение нажатых клавиш
    keypad(stdscr, TRUE);    // Включаем работу с функцией клавиш

    signal(SIGINT, signal_handler);

    while (true) {
        int choice = menu_control();
        
        switch (choice) {
            case 0: {    //game
                endwin();               // Завершаем работу с ncurses
                _main.clear();
                _main.load_settings();
                _main.game(false, "");    
                refresh();
                break;
                }
            case 1: {    //saves
                std::string savefile;
                savefile = save_menu();
                if (savefile == "")
                    break;
                
                endwin();               // Завершаем работу с ncurses
                _main.clear();
                _main.load_settings();
                _main.game(true, savefile);
                break;
                }
            case 2: {    //settings
                settings_menu();
                break;
                }
            case 3: {    //exit
                endwin();
                _main.clear();
                return 0;
                break;
                }
        }
    }
    endwin();               // Завершаем работу с ncurses
    return 0;
}
