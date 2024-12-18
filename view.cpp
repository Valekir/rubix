#include "view.hpp"

using std::cout, std::endl, std::vector, std::ifstream, std::string;



//___________________________________________________View______________________________________________________________

View::View() { 
    style = 6; 
    main_direction = {0, 1, 0}; 
}

/// @brief Изменяет количество сторон, отображаемых в развертке
/// @param n Новое количество сторон в развертке
void View::set_style(int n) {
    if (n < style) {
        clear();
    }

    if (n == 1 || n == 3 || n == 5 || n == 6)
        style = n;
    else
        cout << "Invalid style!" << endl;
}

/// @brief Полностью очищает окно консоли
void View::clear() {
    cout << "\033[2J\033[1;1H";
}

/// @brief Очищает одну строку консоли
void View::clear_line() {
    cout << "\033[1A\033[2K\r";
}

/// @brief Выводит описание языка вращений кубика
void View::help() {
    std::cout << "Cube rotations:" << std::endl << std::endl;
    std::cout << "F (front): the face facing the solver.\nB (back): the back face." << std::endl;
    std::cout << "R (right): the right face.\nL (left): the left face." << std::endl;
    std::cout << "U (up): the upper face.\nD (down): the face opposite to the upper face." << std::endl;
    std::cout << "M (middle): the middle layer parallel to the R & L faces." << std::endl;
    std::cout << "E (equator): the middle layer parallel to the U & D faces." << std::endl;
    std::cout << "S (side): the middle layer parallel to the F & B faces." << std::endl << std::endl;
    std::cout << "The letter means turning a single turn, (90 degrees), clockwise, the corresponding face." << std::endl << std::endl;
    std::cout << "Letter followed by an apostrophe mark (') means turning the corresponding face a single turn, (90 degrees), counter-clockwise." << std::endl;
    std::cout << "ex. R' U'" << std::endl << std::endl;
    std::cout << "Letter followed by \"2\" means turning the corresponding face 2 single turns (180 degrees)" << std::endl;
    std::cout << "ex. R2 = R R" << std::endl << std::endl;
    std::cout << "Command can be written with or without delimitters" << std::endl;
    std::cout << "ex. RURU = R U R U = R_U_R_U" << std::endl << std::endl;
    std::cout << "Cube Rotations" << std::endl;
    std::cout << "These are not actual turns, and is meant to instruct a rotation of whole the cube." << std::endl;
    std::cout << "There are 3 possible axes to rotate the cube on: The X, Y & Z axes." << std::endl;
    std::cout << "x : rotating the cube on the X axis. (R & L faces remain intact)" << std::endl;
    std::cout << "y : rotating the cube on the Y axis. (U & D faces remain intact)" << std::endl;
    std::cout << "z : rotating the cube on the Z axis. (F & B faces remain intact)" << std::endl << std::endl;
    std::cout << "any characters EXCEPT [FBRLUDMxyz'2] will be ignored" << std::endl;
    std::cout << "\nTo exit this menu write anything in console: ";
    getchar();
}

// void View::print_three_face(Cube cube) {
//     vector <vector <Colors>> faceU, faceR, faceF;
//     vector <int> direction = main_direction;
//     faceU = cube.face_to_print(direction);          // верхняя сторона

//     direction = rotate_vector(direction, 'Z');
//     faceR = cube.face_to_print(direction);          // правая сторона

//     direction = rotate_vector(main_direction, 'x');
//     faceF = cube.face_to_print(direction);          // передняя сторона

//     for (int i = 0; i < cube.size(); i++) {
//         print_vector(faceU[i]);
//         print_vector(faceR[i]);
//         cout << endl;
//     }

//     for (auto row : faceF) {
//         print_vector(row);
//         cout << endl;
//     }
// }

// void View::print_five_face(Cube cube) {
//     vector <vector <Colors>> faceU, faceR, faceF, faceL, faceB;
//     vector <int> direction = main_direction;
//     string indent = form_indent(cube.size());

//     direction = rotate_vector(main_direction, 'X');
//     faceB = cube.face_to_print(direction);                  // задняя сторона

//     direction = rotate_vector(main_direction, 'z');
//     faceL = cube.face_to_print(direction);                  // левая сторона

//     faceU = cube.face_to_print(main_direction);             // верхняя сторона

//     direction = rotate_vector(main_direction, 'Z');
//     faceR = cube.face_to_print(direction);                  // правая сторона

//     direction = rotate_vector(main_direction, 'x');
//     faceF = cube.face_to_print(direction);                  // передняя сторона

//     for (int i = 0; i < cube.size(); i++) {
//         cout << indent;
//         print_vector(faceB[i]);
//         cout << endl;
//     }

//     for (int i = 0; i < cube.size(); i++) {
//         print_vector(faceL[i]);
//         print_vector(faceU[i]);
//         print_vector(faceR[i]);
//         cout << endl;
//     }

//        for (int i = 0; i < cube.size(); i++) {
//         cout << indent;
//         print_vector(faceF[i]);
//         cout << endl;
//     }
// }

// void View::print_six_face(Cube cube) {
//     vector <vector <Colors>> faceU, faceR, faceF, faceL, faceB, faceD;
//     vector <int> direction = main_direction;
//     string indent = form_indent(cube.size());

//     direction = rotate_vector(main_direction, 'X');
//     faceB = cube.face_to_print(direction);                  // задняя сторона

//     direction = rotate_vector(main_direction, 'z');
//     faceL = cube.face_to_print(direction);                  // левая сторона

//     faceU = cube.face_to_print(main_direction);             // верхняя сторона

//     direction = rotate_vector(main_direction, 'Z');
//     faceR = cube.face_to_print(direction);                  // правая сторона

//     direction = rotate_vector(main_direction, 'x');
//     faceF = cube.face_to_print(direction);                  // передняя сторона

//     direction = rotate_vector(main_direction, 'x');
//     direction = rotate_vector(direction, 'x');
//     faceD = cube.face_to_print(direction);                  // нижняя сторона

//     for (int i = 0; i < cube.size(); i++) {
//         cout << indent;
//         print_vector(faceB[i]);
//         cout << endl;
//     }

//     for (int i = 0; i < cube.size(); i++) {
//         print_vector(faceL[i]);
//         print_vector(faceU[i]);
//         print_vector(faceR[i]);
//         print_vector(faceD[i]);
//         cout << endl;
//     }

//        for (int i = 0; i < cube.size(); i++) {
//         cout << indent;
//         print_vector(faceF[i]);
//         cout << endl;
//     }
// }

/// @brief Выводит на экран развертку кубика
/// @param cube Кубик, который нужно отрисовать
// void View::print_cube(Cube cube, int n) {
//     cout << "\033[" << n << ";0H";
//     switch (style) {
//     case 1: {
//         vector <vector <Colors>> faceU = cube.face_to_print(main_direction);
//         for (auto row : faceU) {
//             print_vector(row);
//             cout << endl;
//         }
//         break;
//     }case 3: {
//         print_three_face(cube);
//         break;
//     }case 5: {
//         print_five_face(cube);
//         break;
//     }case 6: {
//         print_six_face(cube);
//         break;
//     }default:
//         break;
//     }
//     cout << "\033[39m" << endl;
// }

void View::print_face(vector <vector<Colors>> face, int x, int y, int cell_size) {
    int n = face.size();
    int color = 0;
    cell_size += 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            color = cube_colors[face[i][j]];
            fill_area(x + 2 * j * cell_size, y + i * cell_size, cell_size-1, color);
        }
    }
}

void View::print_cube(Cube cube, int n) {
    int x = 0, y = 1, size = cube.size();
    if (n > 0) { y += n; }

    vector <vector <Colors>> faceU, faceR, faceF, faceL, faceB, faceD;
    vector <int> direction = main_direction;
    string indent = form_indent(cube.size());

    direction = rotate_vector(main_direction, 'X');
    faceB = cube.face_to_print(direction);                  // задняя сторона

    direction = rotate_vector(main_direction, 'z');
    faceL = cube.face_to_print(direction);                  // левая сторона

    faceU = cube.face_to_print(main_direction);             // верхняя сторона

    direction = rotate_vector(main_direction, 'Z');
    faceR = cube.face_to_print(direction);                  // правая сторона

    direction = rotate_vector(main_direction, 'x');
    faceF = cube.face_to_print(direction);                  // передняя сторона

    direction = rotate_vector(main_direction, 'x');
    direction = rotate_vector(direction, 'x');
    faceD = cube.face_to_print(direction);                  // нижняя сторона

    int s = 1;
    int width = (size * s + 3) * 2;
    int height = size * s + 3;

    print_face(faceB, 1 + width, y, s);
    print_face(faceL, 1, y + height, s);
    print_face(faceU, 1 + width, y + height, s);
    print_face(faceR, 1 + width * 2, y + height, s);
    print_face(faceD, 1 + width * 3, y + height, s);
    print_face(faceF, 1 + width, y + height * 2, s);

    cout << endl;
}

void View::set_colors(std::map<Colors, int> new_colors) {
    cube_colors = new_colors;
}

// Выводит вектор в окно консоли
void View::print_vector(vector <Colors> vec) {
    for (auto item : vec) {
        set_background_color(cube_colors[item]);
        cout << "  ";
    }
    cout << "\033[0m";
}


//_____________________________________________ScalableWindow________________________________________________

void ScalableWindow::print_vector(vector <Colors> vec) {
    for (int i = 1; i <= scale_factor * 2; i++) {
        for (Colors item : vec) {
            for (int j = 1; j <= scale_factor; j++) {
                set_background_color(cube_colors[item]);
                cout << "  ";
            }
            cout << "\033[0m" << ' ';
        }
        cout << "\033[0m" << endl;
    }
}

/// @brief Выводит на экран развертку кубика
/// @param cube Кубик, который нужно отрисовать
void ScalableWindow::print_cube(Cube cube, int n) {

}

