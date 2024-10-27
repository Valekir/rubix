#include <iostream>
#include <vector>
using std::vector, std::cin, std::cout, std::endl;

#include "cube.hpp"
#include "view.hpp"

// 8-битные цвета
// "\033[36;5;#m" # - номер цвета
// cout << "\033[38;5;196m" << c << endl;               // красный
// cout << "\033[38;5;40m" << c << endl;                // зеленый
// cout << "\033[38;5;11m" << c << endl;                // желтый
// cout << "\033[38;5;12m" << c << endl;                // синий
// cout << "\033[38;5;15m" << c << endl;                // белый
// cout << "\033[38;5;208m" << c << endl;               // оранжевый
// cout << "\033[38;5;0m" << c << endl;                 // черный
// cout << "\033[39m";                                  // очистка   


int main() {
    vector <int> arr = {0, 0, 0};
    int n = 0;

    cout << "New cube's size: ";
    cin >> n;
    Cube cube(n);
    cout << "Cube created!" << endl;

    View window;
    vector <int> s = {1, 3, 5, 6};
    char c;
    window.print_cube(cube);
    
    while (1) {
        cin >> c;
        cout << "\033[2J\033[1;1H";

        cube.rotate_side(c);
        window.print_cube(cube);
    }

    return 0;
}

