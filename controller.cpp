#include "controller.hpp"

using std::regex, std::sregex_iterator, std::smatch, std::string;


/// @brief Поворачивает грань кубика, или весь кубик
/// @param command Команда для поворота кубика
void Controller::move(char command) {
    char temp = tolower(command);
    if (temp == 'x' || temp == 'y' || temp == 'z')
        current_cube.change_direction(command);
    else
        current_cube.rotate_side(command);
}

/// @brief Случайно перемешивает кубик
void Controller::scramble() {
    current_cube = Cube(current_cube.size());
    vector <char> moves = {'U', 'D', 'F', 'B', 'R', 'L', 'u', 'd', 'f', 'b', 'r', 'l'};
    std::srand(std::time(0));
    for (int i = 0; i < difficulty; i++) {
        current_cube.rotate_side(moves[std::rand() % 12]);
    }
}

/// @brief Проверяет строку на наличие команд, изменящих кубик
/// @param str Строка для поиска
/// @return Если команды найдены, возвращает 1, иначе 0
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

// Команды: help, exit, scramble, hide, moves 
/// @brief Проверяет строку на наличие команд, связанных с окном консоли
/// @param str Строка для поиска
/// @return 0 - если нет новой информации для вывода на экран. 1 - если нужно выйти в меню. 2 - если нужно заново отрисовать кубик
int Controller::parse_console_commands(string& str) {
    regex pattern("(help)|(exit)|(scramble)|(hide)|(moves)", std::regex_constants::icase);
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
        std::transform(match.begin(), match.end(), match.begin(), [](unsigned char c) { return tolower(c); });

        if (match.find("moves") != string::npos) {
            console->clear();
            console->help();
            console->clear();
            if (flags["show_help"])
                hello_game();
            return 2;
        }
        if (match.find("exit") != string::npos) {
            return 1;
        } 
        if (match.find("scramble") != string::npos) {
            scramble();
            console->clear_line();
            return 2;
        } 
        if (match.find("hide") != string::npos && flags["show_help"] == true) {
            std::unordered_map<std::string, std::string> updates;
            flags["show_help"] = "false";
            update_config("game.config", updates)

            console->clear();
            flags["show_help"] = false;
            return 2;
        }
        if (match.find("help") != string::npos && flags["show_help"] == false) {
            std::unordered_map<std::string, std::string> updates;
            flags["show_help"] = "true";
            update_config("game.config", updates)

            console->clear();
            flags["show_help"] = true;
            hello_game();
            return 2;
        }
    }
    return 0;
}

/// @brief Запускает игру
void Controller::game() {
    load_settings();
    if (flags["show_help"]) {
        hello_game();
    } else {
        console->clear();
    }

    int parse_console = 2;
    int parse_cube = 1;
    while (parse_console != 1) {
        if (parse_console == 2 || parse_cube == 1) {
            console->print_cube(current_cube, help_indent * flags["show_help"]);
        }        
        string input;
        
        std::getline(std::cin, input);
        parse_console = parse_console_commands(input);
        parse_cube = parse_cube_commands(input);

        if (parse_cube == 1 && parse_console == 0) {
            while (! command_sequence.empty()) {
                move(command_sequence.front());
                command_sequence.pop();
            }
        }

        if (parse_console != 2 || parse_cube == 1) {
            console->clear_line();
        }
    }
    command_sequence = std::queue<char>();  
}

/// @brief Выводит приветственное сообщение при старте игры
void Controller::hello_game() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "To rotate sides enter commands: ex. RUR'U'" << std::endl;
    std::cout << "To read full information about moves notation use: \"moves\"" << std::endl;
    std::cout << "To scramble cube use: \"scramble\"" << std::endl;
    std::cout << "To exit to menu use: \"exit\"" << std::endl;
    std::cout << "To hide this text use \"hide\"" << std::endl;
    std::cout << "To see this text again use \"help\"" << std::endl;
}

/// @brief  Применяет настройки игры
void Controller::load_settings() {
    std::unordered_map<std::string, std::string> config;
    config = load_config("game.config");

    if (config["window"] == "scalable") {
        console = new ScalableWindow();
    } else if (config["window"] == "default") {
        console = new View();
    }

    bool help = false;
    if (config["show_help"] == "true") {
        help = !help;
    }
    flags["show_help"] = help;
    console->set_help(help);

    if (config["timer"] == "true") {
        flags["timer"] = true;
    } else if (config["timer"] == "false") {
        flags["timer"] = false;
    } 

    current_cube = Cube(std::stoi(config["size"]));

    std::map<Colors, int> cube_color{
      {Colors::W, stoi(config["color_top"])},  {Colors::G, stoi(config["color_front"])},
      {Colors::R, stoi(config["color_right"])}, {Colors::O, stoi(config["color_left"])},
      {Colors::B, stoi(config["color_back"])},  {Colors::Y, stoi(config["color_bottom"])},
      {Colors::K, 0}
    };

    console->set_colors(cube_color);
    difficulty = std::stoi(config["difficulty"]);
}

void Controller::rescale() {
    console->clear();
    if (flags["show_help"]) {hello_game();}
    console->print_cube(current_cube, 7);
}
