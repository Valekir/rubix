#include "controller.hpp"

// Создает новый куб размера dim*dim*dim
void Controller::new_cube(int dim) {
    current_cube = Cube(dim);
}

//
void Controller::parse_input(istream& in) {
    string input;
    while (in >> input) {
        if (parse_cube_commands(input)) {}
        else {
            regex pattern();

        } 

    }
}

// Проверяет входной поток на наличие команд, изменящих кубик
int Controller::parse_cube_commands(string& str) {
    regex pattern("([FBRLUDMXYZfbrludxyz]('2|2|')?)");
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    for (regex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();

        // Для того, чтобы парсер не зависел от регистра ввода
        char temp_char = tolower(match[0]);
        if (temp_char == 'x' || temp_char == 'y' || temp_char == 'z')
            match[0] = tolower(match[0]);
        else 
            match[0] = toupper(match[0]);
        
        if (match.length() != 1) {
            if (match[1] == '\'' || match.length() == 3) {
                if (match[0] < 'a')
                    match[0] = match[0] + ('a'-'A');
                else 
                    match[0] = match[0] - ('a'-'A');
            }
            if (match[1] == '2' || match.length() == 3)
                command_sequence.push(match[0]);
        }
        command_sequence.push(match[0]);
    }
    return 1;
}

// Команды: clear, end, help, style n, new n, 
// Проверяет входной поток на наличие команд, связанных с окном консоли
// ([cC][lL][eE][aA][rR])|([eE][nN][dD])|([hH][eE][lL][pP])
// ([sS][tT][yY][lL][eE] [1356]))|([nN][eE][wW] ((1[0-7])|[1-9]))
int Controller::parse_console_commands(string& str) {
    regex pattern1("([cC][lL][eE][aA][rR])|([eE][nN][dD])|([hH][eE][lL][pP]))");
    regex pattern2("([sS][tT][yY][lL][eE] [1356]))|([nN][eE][wW] ((1[0-7])|[1-9]))");
    auto begin = sregex_iterator(str.begin(), str.end(), pattern1);
    auto end = sregex_iterator();
    int res = 1;

    if (begin == end)
        res = 0;

    for (regex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
        transform(match.begin(), match.end(), match.begin(), 
            [](unsigned char c) { return tolower(c); });

        if (match == "help") {
            console.help();
        } else if (match == "clear") {
            console.clear();
        } else if (match == "end") {
            close_app();
        } else if (match.length() == 5 || match.length() == 6) {
            current_cube = Cube(3);
        } else if (match.length() == 7) {
            console.set_style((int) match[6]);
        }
    }
}

// Изменяет состояние кубика - вращает грани или сам кубик в пространстве
void Controller::move(char command) {
    char temp = tolower(command);
    if (temp == 'x' || temp == 'y' || temp == 'z')
        current_cube.change_direction(command);
    else
        current_cube.rotate_side(command);
}

// Завершает работу программы
void Controller::close_app() {

}

// Случайно перемешивает кубик
void Controller::scramble() {

}