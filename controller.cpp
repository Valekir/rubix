#include "controller.hpp"

using std::regex, std::sregex_iterator, std::smatch, std::string;

/// @brief Создает новый кубик
/// @param dim Размер кубика (dim*dim*dim)
void Controller::new_cube(int dim) {
    current_cube = Cube(dim);
}

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

// Команды: clear, exit, help, style n, new n, 
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
        std::transform(match.begin(), match.end(), match.begin(), 
            [](unsigned char c) { return tolower(c); });

        if (match.find("moves") != string::npos) {
            console.clear();
            console.help();
            console.clear();
            if (flags["show_help"])
                hello_game();
            return 2;
        }
        if (match.find("exit") != string::npos) {
            return 1;
        } 
        if (match.find("scramble") != string::npos) {
            scramble();
            console.clear_line();
            return 2;
        } 
        if (match.find("hide") != string::npos && flags["show_help"] == true) {
            console.clear();
            flags["show_help"] = false;
            return 2;
        }
        if (match.find("help") != string::npos && flags["show_help"] == false) {
            console.clear();
            flags["show_help"] = true;
            hello_game();
            return 2;
        }
    }
    return 0;
}

// Команды: start, exit, settings
int Controller::parse_menu_commands(std::string& str) {
    regex pattern("(start)|(exit)|(settings)|(load)", std::regex_constants::icase);
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
        std::transform(match.begin(), match.end(), match.begin(), [](unsigned char c) { return tolower(c); });
        std::cout << match << std::endl;
        if (match.find("exit") != string::npos) {
            console.clear();
            exit(0);
        } 
        if (match.find("start") != string::npos) {
            console.clear();
            return 1;
        }
        if (match.find("settings") != string::npos) {
            settings();
            console.clear();
            hello_menu();
            std::cout << std::endl;
        }
    }
    console.clear_line();
    return 0;
}

// Настройки: размер кубика, цвета, отображение справки, таймер, 
// настройка сложности (количество ходов в scramble)
/// @brief Ищет в входной строке команды для изменения настроек
/// @param str Строка для поиска команд
/// @return 
int Controller::parse_settings(std::string& str) {
    regex pattern1("(((show_help)|(timer)|(difficulty)|(size)|(color_(top|bottom|right|left|front|back)))[ \t]*=[ \t]*[a-z0-9]+)|(exit)", std::regex_constants::icase);
    auto begin = sregex_iterator(str.begin(), str.end(), pattern1);
    auto end = sregex_iterator();

    if (begin == end)
        return 0;

    std::unordered_map<std::string, std::string> updates;
    std::string line;
    updates = load_config("game.config");

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string line = s.str();
        size_t pos = line.find('=');

        std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) { return tolower(c); });

        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
            updates[key] = value;
        } 

        update_config("game.config", updates);

        if (line.find("exit") != string::npos) {
            return 1;
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
        console.clear();
    }

    int parse_console = 2;
    int parse_cube = 1;
    while (parse_console != 1) {
        if (parse_console == 2 || parse_cube == 1) {
            console.print_cube(current_cube, help_indent * flags["show_help"]);
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
            console.clear_line();
        }
    }
}

/// @brief Запускает меню
void Controller::menu() {
    hello_menu();
    string input = "";

    while (!parse_menu_commands(input)) {
        std::getline(std::cin, input);
    }

    int size = 0;
    std::cout << "Choose cube size from 2 to 5: " << std::endl;
    std::cin >> size;
    while (size < 2 || size > 5) {
        std::cout << "Invalid size! Try again: " << std::endl;
        std::cin >> size;
    }
    current_cube = Cube(size);
}


/// @brief Открывает настроки игры
void Controller::settings() {
    std::string line = "";
    while (!parse_settings(line)) {
        print_settings();
        std::getline(std::cin, line);
    }
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

/// @brief При запуске приложения выводит сообщение, описывающее возможные команды для управления меню
void Controller::hello_menu() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "To start new game use: \"start\"" << std::endl;
    std::cout << "To exit app use: \"exit\"" << std::endl;
    std::cout << "To change settings use: \"settings\"" << std::endl;
}

void Controller::print_settings() {
    std::cout << "\033[2J\033[1;1H";
    std::unordered_map<std::string, std::string> config;
    config = load_config("game.config");

    std::map<std::string, std::string> sorted_map(config.begin(), config.end());

    std::cout << "Current settings:" << std::endl;
    for (const auto& pair : sorted_map) {
        std::cout << pair.first << " = " << pair.second << '\n';
    }
}

void Controller::load_settings() {
    std::unordered_map<std::string, std::string> config;
    config = load_config("game.config");
    if (config["show_help"] == "true") {
        flags["show_help"] = true;
    } else if (config["show_help"] == "false") {
        flags["show_help"] = false;
    }

    if (config["timer"] == "true") {
        flags["timer"] = true;
    } else if (config["timer"] == "false") {
        flags["timer"] = false;
    }
    
    current_cube = Cube(std::stoi(config["size"]));
    
}