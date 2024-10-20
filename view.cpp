#include "view.h"

View::View() { 
    style = 6; 
    main_direction = {0, 1, 0}; 
}

void View::set_style(int n) {
    if (n == 1 || n == 3 || n == 5 || n == 6)
        style = n;
}

void View::clear() {
    cout << "\033[2J\033[1;1H";
}

void View::help() {
    ifstream file("command_list");
    assert(file);
    string line;

    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void View::print_three_face(Cube cube) {
    vector <vector <Colors>> faceU, faceR, faceF;
    vector <int> direction = main_direction;
    faceU = cube.face_to_print(direction);          // верхняя сторона

    direction = rotate_vector(direction, 'Z');
    faceR = cube.face_to_print(direction);          // правая сторона

    direction = rotate_vector(main_direction, 'x');
    faceF = cube.face_to_print(direction);          // передняя сторона

    for (int i = 0; i < cube.size(); i++) {
        print_vector(faceU[i]);
        print_vector(faceR[i]);
        cout << endl;
    }

    for (auto row : faceF) {
        print_vector(row);
        cout << endl;
    }
}

void View::print_five_face(Cube cube) {
    vector <vector <Colors>> faceU, faceR, faceF, faceL, faceB;
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

    for (int i = 0; i < cube.size(); i++) {
        cout << indent;
        print_vector(faceB[i]);
        cout << endl;
    }

    for (int i = 0; i < cube.size(); i++) {
        print_vector(faceL[i]);
        print_vector(faceU[i]);
        print_vector(faceR[i]);
        cout << endl;
    }

       for (int i = 0; i < cube.size(); i++) {
        cout << indent;
        print_vector(faceF[i]);
        cout << endl;
    }
}

void View::print_six_face(Cube cube) {
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

    for (int i = 0; i < cube.size(); i++) {
        cout << indent;
        print_vector(faceB[i]);
        cout << endl;
    }

    for (int i = 0; i < cube.size(); i++) {
        print_vector(faceL[i]);
        print_vector(faceU[i]);
        print_vector(faceR[i]);
        print_vector(faceD[i]);
        cout << endl;
    }

       for (int i = 0; i < cube.size(); i++) {
        cout << indent;
        print_vector(faceF[i]);
        cout << endl;
    }
}

void View::print_cube(Cube cube) {
    switch (style) {
    case 1: {
        vector <vector <Colors>> faceU = cube.face_to_print(main_direction);
        for (auto row : faceU) {
            print_vector(row);
            cout << endl;
        }
        break;
    }case 3: {
        print_three_face(cube);
        break;
    }case 5: {
        print_five_face(cube);
        break;
    }case 6: {
        print_six_face(cube);
        break;
    }default:
        break;
    }
    cout << "\033[39m" << endl;
}

