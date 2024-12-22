#include "menu.hpp"


using std::string, std::vector, std::ifstream;

// Функция для центрирования текста
void center_text(int y, int x, const std::string& text) {
    int len = text.length();
    mvprintw(y, (COLS - len) / 2, "%s", text.c_str());
}

std::string prompt_input(const std::string& prompt, const std::string& defaultValue) {
    echo();
    mvprintw(LINES - 2, (COLS - prompt.length() - 20) / 2, "%s: ", prompt.c_str());
    char input[20];
    getnstr(input, sizeof(input) - 1);
    noecho();
    return std::string(input).empty() ? defaultValue : std::string(input);
}

void error_message() {
    move(LINES - 2, 0);
    mvprintw(LINES - 2, (COLS - 40) / 2, "Invalid input. Please try again.");
    getch();
    clrtoeol(); 
}

int color_input(const std::string& prompt) {
    int input;
    std::string inputStr;
    while (true) {
        inputStr = prompt_input(prompt, "0");
        try {
            input = stoi(inputStr);
            if (input >= 0 && input <= 255) {
                return input;
            } else {
                error_message();
            }
        } catch (const std::invalid_argument& e) {
            error_message();
        }
    }
}

void settings_menu() {
    std::unordered_map<std::string, std::string> config = load_config("game.config");
    bool show_help = config["show_help"].find("true") != std::string::npos ? true : false;
    bool timer = config["timer"].find("true") != std::string::npos ? true : false;
    int size = std::stoi(config["size"]);
    int difficulty = std::stoi(config["difficulty"]);
    std::string window = config["window"];

    // Цвета
    int color_top = std::stoi(config["color_top"]);
    int color_bottom = std::stoi(config["color_bottom"]);
    int color_right = std::stoi(config["color_right"]);
    int color_left = std::stoi(config["color_left"]);
    int color_front = std::stoi(config["color_front"]);
    int color_back = std::stoi(config["color_back"]);

    while (true) {
        clear();

        int startX = (COLS - 50) / 2;  // 50 - ширина меню
        int startY = (LINES - 10) / 2;  // 10 - количество строк меню

        mvprintw(startY, startX, "Settings Menu");
        // mvprintw(startY + 2, startX, "1. Cube size (2-5): %d", size);
        mvprintw(startY + 3, startX, "2. Difficulty (1-20): %d", difficulty);
        mvprintw(startY + 4, startX, "3. Window: %s", window.c_str());
        mvprintw(startY + 5, startX, "4. Show Help: [%s]", show_help ? "X" : " ");
        mvprintw(startY + 6, startX, "5. Timer: [%s]", timer ? "X" : " ");
        
        mvprintw(startY + 7, startX, "F1. Color Top: %d", color_top);
        mvprintw(startY + 8, startX, "F2. Color Bottom: %d", color_bottom);
        mvprintw(startY + 9, startX, "F3. Color Right: %d", color_right);
        mvprintw(startY + 10, startX, "F4. Color Left: %d", color_left);
        mvprintw(startY + 11, startX, "F5. Color Front: %d", color_front);
        mvprintw(startY + 12, startX, "F6. Color Back: %d", color_back);

        mvprintw(startY + 13, startX, "Press 'r' to reset settings");
        mvprintw(startY + 14, startX, "Press 'q' to quit");
        refresh();

        int ch = getch();

        switch (ch) {
            // case '1':  // Изменение размера
            // try {
            //         size = std::stoi(prompt_input("Enter Size (2-5)", std::to_string(size)));
            //         if (size < 1) size = 1;
            //         if (size > 5) size = 5;
            //     }
            //     catch(const std::exception& e) {
            //         error_message();
            //     }
            //     break;
            case '2':  // Изменение сложности
                try {
                    difficulty = std::stoi(prompt_input("Enter Difficulty (1-20)", std::to_string(difficulty)));
                    if (difficulty < 1) difficulty = 1;
                    if (difficulty > 20) difficulty = 20;
                    config["difficulty"] = difficulty;
                }
                catch(const std::exception& e) {
                    error_message();
                }                
                break;
            case '3': {  // Изменение режима отрисовки
                std::string temp;
                temp = prompt_input("Enter Window (scalable/default)", temp);
                std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) { return tolower(c); });

                if (temp.find("scalable") != std::string::npos) {
                    window = "scalable";
                } else if (temp.find("default") != std::string::npos) {
                    window = "default";
                }
                break;
                }
            case '4':  // Переключатель Show Help
                show_help = !show_help;
                break;
            case '5':  // Переключатель Timer
                timer = ! timer;
                break;
            case KEY_F(1):
                color_top = color_input("Enter Color Top (0-255)");
                break;
            case KEY_F(2):
                color_bottom = color_input("Enter Color Bottom (0-255)");
                break;
            case KEY_F(3):
                color_right = color_input("Enter Color Right (0-255)");
                break;
            case KEY_F(4):
                color_left = color_input("Enter Color Left (0-255)");
                break;
            case KEY_F(5):
                color_front = color_input("Enter Color Front (0-255)");
                break;
            case KEY_F(6):
                color_back = color_input("Enter Color Back (0-255)");
                break;
            case 'q': {  // Выход из меню настроек
                std::transform(window.begin(), window.end(), window.begin(), [](unsigned char c) { return tolower(c); });
                if (window.find("default") != std::string::npos) {
                    config["window"] = "default";
                } else {
                    config["window"] = "scalable";
                }
                config["size"] = std::to_string(size);
                config["difficulty"] = std::to_string(difficulty);
                config["color_front"] = std::to_string(color_front);
                config["color_back"] = std::to_string(color_back);
                config["color_left"] = std::to_string(color_left);
                config["color_right"] = std::to_string(color_right);
                config["color_top"] = std::to_string(color_top);
                config["color_bottom"] = std::to_string(color_bottom);
                config["show_help"] = show_help ? "true" : "false";
                config["timer"] = timer ? "true" : "false";
                for (auto item : config) {
                    std::cout << item.first << " "  << item.second << std::endl;
                }
                update_config("game.config", config);
                return;
                }
            case 'r': {
                std::unordered_map<std::string, std::string> updates;
                size = 3;
                color_front = 40;
                color_back = 12;
                color_left = 208;
                color_right = 196;
                color_top = 15;
                color_bottom = 11;
                difficulty = 5;
                timer = false;
                show_help = true;
                window = "scalable";
                break;
            }
            default:
                break;
        }
    }
}

void draw_menu(const std::string choices[], int highlight, int size) {
    int startY = (LINES - size) / 2;
    int startX = (COLS - 20) / 2;

    for (int i = 0; i < size; ++i) {
        if (i == highlight) {
            attron(A_REVERSE);
            mvprintw(startY + i * 3 + 1, startX + 1, "%s", choices[i].c_str());
            attroff(A_REVERSE);
        } else {
            mvprintw(startY + i * 3 + 1, startX + 1, "%s", choices[i].c_str());
        }
    }
}

int menu_control() {
    const std::string choices[] = {
        "New Game",
        "Load Game",
        "Settings",
        "Exit"
    };
    int size = sizeof(choices) / sizeof(choices[0]);

    int highlight = 0;
    int choice = -1;

    while (true) {
        clear();
        draw_menu(choices, highlight, size);
        refresh();

        int ch = getch();

        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? size - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == size - 1) ? 0 : highlight + 1;
                break;
            case 10:    // ENTER
                choice = highlight;
                return choice;
            default:
                break;
        }
    }
}

void draw_save_menu(std::vector<std::string> saves, int choice, int num_saves, int startY) {
    clear();
    center_text(startY - 2, 0, "Load Game");

    for (int i = 0; i < num_saves; ++i) {
        int y = startY + i * 2;
        if (i + 1 == choice) {
            center_text(y, 0, "> " + saves[i] + " <");
        } else {
            center_text(y, 0, saves[i]);
        }
    }

    center_text(startY + num_saves * 2 , 0, "Press Enter to load" );
    center_text(startY + num_saves * 2 +  1, 0, "Press 'q' to to return" );
    center_text(startY + num_saves * 2  + 2, 0, "Press 'd' delete to delete save" );
}

std::string save_menu() {
    vector<string> saves = load_saves();
    int num_saves = saves.size();
    clear();

    if (num_saves == 0) {
        center_text((LINES - 1) / 2, 0, "No saved games found!");
        getch();
        return "";
    }

    int startY = (LINES - 2 * num_saves - 1) / 2;
    int choice = 1;
    int last_num = num_saves;

    while (true){
        draw_save_menu(saves, choice, num_saves, startY);
        int ch = getch();
        switch (ch) {

            case KEY_UP:
                if (choice > 1) choice--;
                else choice = num_saves;
                break;
            case KEY_DOWN:
                if (choice < num_saves) choice++;
                else choice = 1;
                break;
            case 10: {  // ENTER
                clear();
                center_text((LINES - 1) / 2, 0, "Loading save " + saves[choice - 1]);
                return saves[choice - 1];
                }
            case 'q':   // ESC
                return "";
            case 'd':{
                delete_save(saves[choice - 1]);
                saves.erase(saves.begin() + choice - 1);
                num_saves--;
                }
            default:
                break;
        }
        refresh();
    }
}
