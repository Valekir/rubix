#include "controller.hpp"
#include "menu.hpp"

void start_ncurses() {
    initscr();               // Инициализация ncurses
    cbreak();                // Включаем режим распознавания символов
    noecho();                // Отключаем отображение нажатых клавиш
    keypad(stdscr, TRUE);    // Включаем работу с функцией клавиш

}

int main() {
    Controller _main;
    start_ncurses();

    while (true) {
        int choice = menu_control();

        switch (choice) {
            case 0:
                endwin();               // Завершаем работу с ncurses
                _main.game();                
                start_ncurses();
                break;
            case 1:
                save_menu();
                break;
            case 2:
                settings_menu();
                break;
            case 3:
                _main.clear();
                endwin();
                return 0;
                break;
        }
    }

    endwin();               // Завершаем работу с ncurses
    return 0;
}