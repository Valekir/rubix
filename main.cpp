#include "controller.hpp"
#include "menu.hpp"
#include <csignal>


void signal_handler(int sign) {
    endwin();
    system("clear");
    exit(0);
}

int main() {
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
