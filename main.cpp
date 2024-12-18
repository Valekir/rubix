#include "controller.hpp"
#include "menu.hpp"

int main() {
    Controller _main;
    initscr();               // Инициализация ncurses
    cbreak();                // Включаем режим распознавания символов
    noecho();                // Отключаем отображение нажатых клавиш
    keypad(stdscr, TRUE);    // Включаем работу с функцией клавиш

    while (true) {
        int choice = menu_control();
        switch (choice) {
            case 0:
                endwin();               // Завершаем работу с ncurses
                _main.game();                
                refresh();
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
    endwin();
    return 0;
}
