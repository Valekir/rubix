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
    regex pattern1("(size[ \t]*=[ \t]*[3-5]+)|((show_help|timer)[ \t]*=[ \t]*(false|true))|(color_(top|bottom|right|left|front|back)[ \t]*=[ \t]*(2[0-4][0-9]|25[0-5]|1[0-9]{2}|[1-9][0-9]{0,1}|0))|(window[ \t]*=[ \t]*(default|scalable))|(exit)|(reset)", std::regex_constants::icase);
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

        if (line.find("exit") != string::npos) {
            return 1;
        }

        if (line.find("reset") != string::npos) {
            updates["size"] = "3";
            updates["color_front"] = "40";
            updates["color_back"] = "12";
            updates["color_left"] = "208";
            updates["color_right"] = "196";
            updates["color_top"] = "15";
            updates["color_bottom"] = "11";
            updates["difficulty"] = "5";
            updates["timer"] = "false";
            updates["show_help"] = "true";
            updates["window"] = "scalable";   

        }
        update_config("game.config", updates);
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
}


/// @brief Открывает настройки игры
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
    std::cout << "Color is integer between 0 and 255" << std::endl;
    std::cout << "Size is integer between 2 and 5" << std::endl;
    std::cout << "Flags can be true/false" << std::endl;
    std::cout << "Window can be default/scalable" << std::endl;
    std::cout << "To set default settings use: \"reset\"" << std::endl;
    std::cout << "To exit to menu use \"exit\"" << std::endl;
    std::unordered_map<std::string, std::string> config;
    config = load_config("game.config");

    std::map<std::string, std::string> sorted_map(config.begin(), config.end());

    std::cout << "Current settings:" << std::endl;
    for (const auto& pair : sorted_map) {
        std::cout << pair.first << " = " << pair.second << '\n';
    }
}

/// @brief  Применяет настройки игры
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
    
    // if (config["window"] == "scalable") {
    //     console = ScalableWindow();
    // } else if (config["window"] == "default") {
    //     console = View();
    // }
    
    current_cube = Cube(std::stoi(config["size"]));

    std::map<Colors, int> cube_color{
      {Colors::W, stoi(config["color_top"])},  {Colors::G, stoi(config["color_front"])},
      {Colors::R, stoi(config["color_right"])}, {Colors::O, stoi(config["color_left"])},
      {Colors::B, stoi(config["color_back"])},  {Colors::Y, stoi(config["color_bottom"])},
      {Colors::K, 0}
    };

    console.set_colors(cube_color);
    difficulty = std::stoi(config["difficulty"]);
}

/// @brief Читает настройки из файла filename
/// @return 
std::unordered_map<std::string, std::string> Controller::load_config(const std::string& filename) {
    std::unordered_map<std::string, std::string> config;

    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
                value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());

                config[key] = value;
            }
        }
        file.close();
    } else {
        config["size"] = "3";
        config["color_front"] = "40";
        config["color_back"] = "12";
        config["color_left"] = "208";
        config["color_right"] = "196";
        config["color_top"] = "15";
        config["color_bottom"] = "11";
        config["difficulty"] = "5";
        config["timer"] = "false";
        config["show_help"] = "true";
        config["window"] = "scalable";

        std::ofstream outFile(filename, std::ios::out);
        for (const auto& [key, value] : config) {
            outFile << key << "=" << value << "\n";
        }
        outFile.close();
    }

    return config;
}

/// @brief Обновляет настройки в конфиг файле
/// @param filename Название конфиг файла 
/// @param updates Список измененных настроек
void Controller::update_config(const std::string& filename, const std::unordered_map<std::string, std::string>& updates) {
    auto config = load_config(filename);
    std::string tempFilename = filename + ".tmp";
    std::ofstream tempFile(tempFilename, std::ios::out);
    
    if (!tempFile.is_open()) {
        std::cerr << "Failed to create temporary file" << tempFilename << std::endl;
        return;
    }
    for (const auto& [key, value] : updates) {
        config[key] = value;
    }

    for (const auto& [key, value] : config) {
        tempFile << key << "=" << value << "\n";
    }
    tempFile.close();

    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Failed to remove old config file" << std::endl;
    } else {
        if (std::rename(tempFilename.c_str(), filename.c_str()) != 0) {
            std::cerr << "Failed to rename temporary file to config file" << std::endl;
        }
    }
}
