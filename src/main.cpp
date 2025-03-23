#include "controller.hpp"
#include "menu.hpp"
#include <csignal>

#include "solver.hpp"
#include "cube.hpp"
#include <iostream>

int main() {
    Cube cube(3);

    cube.rotate_side('U');
    cube.rotate_side('R');
    cube.rotate_side('d');
    cube.rotate_side('l');
    cube.rotate_side('u');
    cube.rotate_side('F');

    std::cout << solve_cube(cube) << std::endl;

    return 0;
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
