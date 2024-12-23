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
    system("clear");
    // cout << "\033[1J\033[H";
}

/// @brief Очищает одну строку консоли
void View::clear_line() {
    cout << "\033[1A\033[2K\r";
}

/// @brief Выводит описание языка вращений кубика
void View::help() {
    clear();
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
    clear();
}

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

vector<vector<vector<Colors>>> View::find_faces(Cube cube) {
	vector <vector <vector <Colors>>> faces;
    vector <int> direction = main_direction;
    string indent = form_indent(cube.size());

    direction = rotate_vector(main_direction, 'X');
    faces.push_back(cube.face_to_print(direction));				// задняя сторона

    direction = rotate_vector(main_direction, 'z');
    faces.push_back(cube.face_to_print(direction));				// левая сторона

    faces.push_back(cube.face_to_print(main_direction));		// верхняя сторона

    direction = rotate_vector(main_direction, 'Z');
    faces.push_back(cube.face_to_print(direction));				// правая сторона

    direction = rotate_vector(main_direction, 'x');
    faces.push_back(cube.face_to_print(direction));				// передняя сторона

    direction = rotate_vector(main_direction, 'x');
    direction = rotate_vector(direction, 'x');
    faces.push_back(cube.face_to_print(direction));				// нижняя сторона

	return faces;
}

void View::print_cube(Cube cube, int n) {
    int x = 0, y = 1, size = cube.size();
    if (n > 0) { y += n; }
	vector <vector<vector<Colors>>> faces = find_faces(cube);
    
    int cell_size = 1;
    int width = (size * (cell_size + 1)) * 2;
    int height = size * (cell_size + 1);

    print_face(faces[0], 2 + width, y, cell_size);
    print_face(faces[1], 1, y + height, cell_size);
    print_face(faces[2], 2 + width, y + height, cell_size);
    print_face(faces[3], 3 + width * 2, y + height, cell_size);
    print_face(faces[5], 4 + width * 3, y + height, cell_size);
    print_face(faces[4], 2 + width, y + height * 2, cell_size);
    cout << endl;
}

void View::set_colors(std::map<Colors, int> new_colors) {
    cube_colors = new_colors;
}

void View::set_help(bool help) {
    show_help = help;
}

int View::find_scale(int n) {
    return 1;
}

//_____________________________________________ScalableWindow________________________________________________


/// @brief Находит максимальный размер ячеек для вывода в зависимости от размера окна
/// @return Размер ячейки
int ScalableWindow::find_scale(int size) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int x = w.ws_col;
    int y = w.ws_row - 7 * show_help;
    bool flag = true;

    int new_scale_factor = 1;
    if (((4 * size) * new_scale_factor * 2 + 25) >= x || ((4 * size) * new_scale_factor + 7) >= y) {
        flag = false;
    }
    while ((((4 * size) * new_scale_factor * 2 + 25) < x) && (((4 * size) * new_scale_factor + 7) < y)) {
        new_scale_factor++;
    }
    new_scale_factor -= 1 * flag;
    scale_factor = new_scale_factor;
	return scale_factor;
}


/// @brief Выводит на экран развертку кубика
/// @param cube Кубик, который нужно отрисовать
void ScalableWindow::print_cube(Cube cube, int n) {
int x = 0, y = 1, size = cube.size();
    if (n > 0) { y += n; }
	vector <vector<vector<Colors>>> faces = find_faces(cube);
    
    int cell_size = find_scale(cube.size());
    int width = (size * (cell_size + 1)) * 2;
    int height = size * (cell_size + 1);

    print_face(faces[0], 2 + width, y, cell_size);
    print_face(faces[1], 1, y + height, cell_size);
    print_face(faces[2], 2 + width, y + height, cell_size);
    print_face(faces[3], 3 + width * 2, y + height, cell_size);
    print_face(faces[5], 4 + width * 3, y + height, cell_size);
    print_face(faces[4], 2 + width, y + height * 2, cell_size);
    cout << endl;
}

