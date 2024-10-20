#include <iostream>
#include <vector>
using std::vector, std::cin, std::cout, std::endl;

#include "cube.h"
#include "view.h"

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

    for (auto item : s) {
        window.clear();
        window.set_style(item);
        window.print_cube(cube);
        if (item != s[s.size()-1]) {
            cout << "\nTo continue enter anything: ";
            cin >> c;
        }
    }
    return 0;
}

