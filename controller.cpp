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
    vector <char> moves = {'U', 'F', 'B', 'R', 'L', 'd', 'f', 'b', 'r', 'l'};
    // vector <char> moves = {'U', 'D', 'F', 'B', 'R', 'L', 'u', 'd', 'f', 'b', 'r', 'l'};
    char move;
    std::srand(std::time(0));
    for (int i = 0; i < difficulty; i++) {
        // move = moves[std::rand() % 12];
        move = moves[std::rand() % 10];
        current_cube.rotate_side(move);
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
/// @return 
int Controller::parse_console_commands(string& str) {
    regex pattern("(help)|(exit)|(scramble)|(hide)|(moves)", std::regex_constants::icase);
    auto begin = sregex_iterator(str.begin(), str.end(), pattern);
    auto end = sregex_iterator();

    if (begin == end) {
        return 0;
    }

    for (sregex_iterator i = begin; i != end; ++i) {
        smatch s = *i;
        string match = s.str();
        std::transform(match.begin(), match.end(), match.begin(), [](unsigned char c) { return tolower(c); });

        if (match.find("exit") != string::npos) {
            return 1;
        } 
        if (match.find("moves") != string::npos) {
            return 2;
        }
        if (match.find("scramble") != string::npos) {
            return 3;
        } 
        if (match.find("hide") != string::npos && flags["show_help"] == true) {
            return 4;
        }
        if (match.find("help") != string::npos && flags["show_help"] == false) {
            return 5;
        }
    }
    return 0;
}

/// @brief Запускает игру
void Controller::game(bool from_save, std::string filename) {
    load_settings();
    if (from_save) {
        load_saved_cube(filename);
    }
    if (flags["show_help"]) {
        hello_game();
    } else {
        console->clear();
    }
    console->print_cube(current_cube, 7 * flags["show_help"]);

    while (true) {
        string input;
        std::getline(std::cin, input);
        switch (parse_console_commands(input)) {
        case 0: { // parse cube commands
            parse_cube_commands(input);
            while (! command_sequence.empty()) {
                move(command_sequence.front());
                command_sequence.pop();
            }
            console->clear_line();
            break;
        }
        case 1: {   // exit
            command_sequence = std::queue<char>();  
            console->clear();
            save();
            return;
        }
        case 2: {   // moves
            console->clear();
            console->help();
            console->clear();
            if (flags["show_help"])
                hello_game();
            break;
            }
        case 3: {   // scramble
            scramble();
            console->clear_line();
            break;
        }
        case 4: {   // hide
            std::unordered_map<std::string, std::string> updates;
            flags["show_help"] = false;
            updates["show_help"] = "false";
            update_config("game.config", updates);
            console->clear();
            console->find_scale(current_cube.size());
            console->clear_line();
            break;
        }
        case 5: { // help
            std::unordered_map<std::string, std::string> updates;
            flags["show_help"] = true;
            updates["show_help"] = "true";
            update_config("game.config", updates);
            console->clear();
            hello_game();
            console->find_scale(current_cube.size());
            console->clear_line();
            break;
        }
        default:
            console->clear_line();
            break;
        }
        console->print_cube(current_cube, 7 * flags["show_help"]);
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

void Controller::save() {
    std::cout << "Do you want to save game? [y/n]" << std:: endl;
    char ans = 'n';
    std::cin >> ans;
    if (ans == 'n' || ans == 'N')
        return;

    std::string filename;
    std::cout << "Enter save name: ";
    std::cin >> filename;
    bool flag = file_exists(filename);  // если существует, то не нужно добавлять в saves
    std::ofstream savefile(filename);

    if (!savefile) {
        std::cerr << "Unable to  open file: " << filename << std::endl;
    }
    savefile << current_cube.size() << std::endl;

    std::vector<std::vector<Piece>> parts = current_cube.get_parts();

    for (auto slice : parts) {
        for (auto item : slice) {
            std::vector <int> angles = item.get_pos();
            std::vector <Colors> color = item.get_color();
            for (int ang : angles) {
                savefile << ang << ' ';
            }
            for (Colors col : color) {
                savefile << colortochar(col) << ' ';
            }
            savefile << std::endl;
        }
    }
    savefile.close();

    if (flag) 
        return;

    std::ofstream output("saves", std::ios::app);
    if (!output) {
        std::cerr << "Unable to open file: saves" << std::endl;
    }
    output << filename << std::endl;
    output.close();
}

void Controller::load_saved_cube(std::string filename) {
    std::vector<std::vector<int>> angles;
    std::vector<std::vector<Colors>> colors;

    std::ifstream input(filename);
    int dim;
    input >> dim;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim*dim; j++) {
            vector<int> temp_angles;
            vector<Colors> temp_colors;
            int temp;
            char ch;
            for (int k = 0; k < dim; k++) {
                input >> temp;
                temp_angles.push_back(temp);                
            }
            for (int k = 0; k < dim; k++) {
                input >> ch;
                temp_colors.push_back(chartocolor(ch));                
            }
            angles.push_back(temp_angles);
            colors.push_back(temp_colors);
        }
    }
    current_cube = Cube(dim, angles, colors);
}

