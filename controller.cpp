#include "controller.hpp"

using std::regex, std::sregex_iterator, std::smatch, std::string;

// Создает новый куб размера dim*dim*dim
void Controller::new_cube(int dim) {
    current_cube = Cube(dim);
}

void Controller::parse_input(std::istream& in) {
    string input;
    std::getline(in, input);
    if (! parse_console_commands(input)) {
        if (parse_cube_commands(input)) {
            while (! command_sequence.empty()) {
                move(command_sequence.front());
                command_sequence.pop();
            }
            console.clear();
            console.print_cube(current_cube);
        }
    }    
}

// Проверяет входной поток на наличие команд, изменящих кубик
int Controller::parse_cube_commands(string& str) {
    regex pattern("([FBRLUDMSExyz]('2|2'|'|2)?)");
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
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
    regex pattern1("([cC][lL][eE][aA][rR])|([eE][nN][dD])|([hH][eE][lL][pP])|([sS][tT][yY][lL][eE] [1356])|[nN][eE][wW] (([1-5]))|([sS][cC][rR][aA][mM][bB][lL][eE])|([sS][hH][oO][wW])");
    auto begin = sregex_iterator(str.begin(), str.end(), pattern1);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
        std::transform(match.begin(), match.end(), match.begin(), 
            [](unsigned char c) { return tolower(c); });

        if (match.find("help") != string::npos) {
            console.help();
        } 
        if (match.find("clear") != string::npos) {
            console.clear();
        } 
        if (match.find("end") != string::npos) {
            close_app();
        } 
        if (match.find("scramble") != string::npos) {
            console.clear();
            scramble();
            console.print_cube(current_cube);
        } 
        if (match.find("show") != string::npos) {
            console.clear();
            console.print_cube(current_cube);
        } 
        if (match.find("new") != string::npos) {
            current_cube = Cube((int) (match[4] - '0'));
        } 
        if (match.find("style") != string::npos) {
            console.set_style((int) (match[6] - '0'));
        }
    }
    return 1;
}

// Изменяет состояние кубика - вращает грани или сам кубик в пространстве
void Controller::move(char command) {
    char temp = tolower(command);
    std::cout << command << std::endl;

    if (temp == 'x' || temp == 'y' || temp == 'z')
        current_cube.change_direction(command);
    else
        current_cube.rotate_side(command);
}

// Завершает работу программы
void Controller::close_app() {
    console.clear();
    exit(0);
}

// Случайно перемешивает кубик
void Controller::scramble() {
    current_cube = Cube(current_cube.size());
    vector <char> moves = {'U', 'D', 'F', 'B', 'R', 'L', 'u', 'd', 'f', 'b', 'r', 'l'};
    std::srand(std::time(0));
    for (int i = 0; i < 20; i++) {
        current_cube.rotate_side(moves[std::rand() % 12]);
    }
}
