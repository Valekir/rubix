#include "controller.hpp"
#include "menu.hpp"
#include <csignal>

#include "solver.hpp"
#include "cube.hpp"

#include <iostream>
#include "utils.hpp"


//_________________________________________________________ostream____________________________________________________
std::ostream& operator<<(std::ostream& out, SCube cube) {
    auto parts = cube.getParts();
    int dim = cube.size();

    for (int i=0; i<dim; i++) {
        for (int j=0; j<dim*dim; j++) {
            auto piece = parts[i][j];

            auto type = piece.getType();
            auto color = piece.getColor();
            auto pos = piece.getPosition();

            std::cout << type << ' ';
            for (int k=0; k<3; k++) {
                out << colortochar(color[k]) << ' ';
            }
            for (int k=0; k<3; k++) {
                out << pos[k] << ' ';
            }
            out << std::endl;
        }
        out << std::endl;
    }
    
    return out;
}

std::ostream& operator<<(std::ostream& out, Piece p) {
    auto colors = p.get_color();
    auto pos = p.get_pos();
    auto type = p.get_type();
    
    out << type << ' ';
    for (auto i : colors) {
        out << colortochar(i) << ' ';
    }
    for (auto i : pos) {
        out << i << ' ';
    }
    
    return out;
}

std::ostream& operator<<(std::ostream& out, Cube c) {
    auto parts = c.get_parts();
    
    for (auto layer : parts) {
        for (auto i : layer) {
            out << i << std::endl;
        }
        out << std::endl;
    }
    
    return out;
}


//__________________________________________________main_________________________________________________________


int main() {
    Cube defaultCube(3);
    SCube cube(defaultCube);
    Timer timer;
//    std::cout << cube.isSolved() << std::endl;    
//    std::cout << "default cube:\n" << defaultCube << std::endl;
//    std::cout << "converted cube:\n" << cube << std::endl;

    cube.rotateSide('U');
    cube.rotateSide('R');
    cube.rotateSide('d');
    cube.rotateSide('l');
    cube.rotateSide('u');
    cube.rotateSide('F');
//    std::cout << heuristic(cube) << std::endl;
    
    timer.start();
    std::cout << solve_cube(cube) << std::endl;
    timer.stop();

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
