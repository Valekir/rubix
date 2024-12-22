#include "utils.hpp"

using std::cout, std::cerr, std::endl, std::cos, std::sin, std::string;


void Timer::start() {
    if (!running) {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }
}

void Timer::stop() {
    if (running) {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Time: " << duration.count() << " ms.\n";
    }
}

// Colors -> char
char colortochar(Colors col) {
    if (col == Colors::W)
        return 'W';
    else if (col == Colors::G)
        return 'G';
    else if (col == Colors::R)
        return 'R';
    else if (col == Colors::O)
        return 'O';
    else if (col == Colors::B)
        return 'B';
    else if (col == Colors::Y)
        return 'Y';
    return 'K';
}

// char -> Colors
Colors chartocolor(char col) {
    if (col == 'W')
        return Colors::W;
    else if (col == 'G')
        return Colors::G;
    else if (col == 'R')
        return Colors::R;
    else if (col == 'O')
        return Colors::O;
    else if (col == 'B')
        return Colors::B;
    else if (col == 'Y')
        return Colors::Y;
    return Colors::K;
}


// Устанавливает 8-битный цвет заливки с кодом n
void set_background_color(int n) {
    cout << "\033[48;5;" << (int) n << "m";
}

// Формирует строку отступа длиной n, состоящую из пробелов
string form_indent(int n) {
    string res;
    for (int i = 0; i < 2*n; i++) {
        res += " ";
    }
    return res;
}

// Функция определяет направление по осям xyz, в котором будет направлен элемент
vector <int> find_angles(int layer, int i, int dim) {
    vector <int> angles;

    if (i % dim == dim-1)               // правая сторона
        angles.push_back(1);
    else if (i % dim == 0)              // левая сторона
        angles.push_back(-1);
    else
        angles.push_back(0);

    if (layer == 0)                     // верхняя сторона
        angles.push_back(1);
    else if (layer == dim-1)            // нижняя сторона
        angles.push_back(-1);
    else
        angles.push_back(0);

    if (i > dim*dim - dim - 1)          // передняя сторона
        angles.push_back(1);
    else if (i < dim)                   // задняя сторона
        angles.push_back(-1);
    else
        angles.push_back(0);
    return angles;
}

// 1 - белый 2 - зеленый 3 - красный 4 - оранжевый 5 - синий 6 - 0 - нет цвета
// Поворачивает вектор по осям xyz, в соответствии с командой. 
vector <Colors> find_colors(int layer, int i, int dim) {
    vector <Colors> colors;

    if (i % dim == dim-1)
        colors.push_back(Colors::R);
    else if (i % dim == 0)
        colors.push_back(Colors::O);
    else
        colors.push_back(Colors::K);

    if (layer == 0)
        colors.push_back(Colors::W);
    else if (layer == dim-1)
        colors.push_back(Colors::Y);
    else
        colors.push_back(Colors::K);

    if (i > dim*dim - dim - 1)
        colors.push_back(Colors::G);
    else if (i < dim)
        colors.push_back(Colors::B);
    else
        colors.push_back(Colors::K);

    return colors;
}

// Поворачивает единичный вектор по осям xyz, в соответствии с командой. 
vector <int> rotate_vector(vector<int> vec, char dir) {
    int x = vec[0], y = vec[1], z = vec[2];
    double angle = 0;
    if (tolower(dir) == dir)
        angle = M_PI_2;
    else
        angle = -M_PI_2;
    int c = cos(angle);
    int s = sin(angle);
    if (dir == 'x' || dir == 'X') {
        y = vec[1]*c - vec[2]*s;
        z = vec[1]*s + vec[2]*c;
    } else if (dir == 'y' || dir == 'Y') {
        x = vec[0]*c - vec[2]*s;
        z = vec[0]*s + vec[2]*c;
    } else if (dir == 'z' || dir == 'Z') {
        x = vec[0]*c - vec[1]*s;
        y = vec[0]*s + vec[1]*c;
    } else {
        cerr << "Something went wrong!\n";
        return vec;
    }
    vec = {x, y, z};
    return vec;
}

/// @brief Заполняет участок консоли цветом color
/// @param x Начальная координата x
/// @param y Начальная координата y
/// @param n Размер квадрата
void fill_area(int x, int y, int n, int color) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n * 2; j++) {
            cout << "\033[" << y + i << ";" << x + j << "H";		    // Перемещает курсор
            cout << "\033[48;5;" << (int) color << "m" << " ";			// Красит одну ячейку консоли
        }
    }
	cout << "\033[0m";
}

/// @brief Читает настройки из файла filename
/// @return 
std::unordered_map<std::string, std::string> load_config(const std::string& filename) {
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
void update_config(const std::string& filename, const std::unordered_map<std::string, std::string>& updates) {
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

// Функция для чтения сохранений из файла
std::vector<std::string> load_saves() {
    std::vector<std::string> saves;
    std::ifstream saveFile("saves");
    if (saveFile.is_open()) {
        std::string line;
        while (getline(saveFile, line)) {
            saves.push_back(line);
        }
        saveFile.close();
    }
    return saves;
}

void remove_string(std::string filename, std::string str) {
    std::ifstream input(filename);
    std::stringstream buffer;

    if (!input) {
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line != str) {
            buffer << line << std::endl;
        }
    }

    input.close();

    std::ofstream output(filename);
    if (!output) {
        return;
    }

    output << buffer.str();
    output.close();
}

void delete_save(std::string filename) {
    std::vector<std::string> saves;
    saves = load_saves();   // названия сохранений
    if ((std::find(saves.begin(), saves.end(), filename)) == saves.end()) {
        std::cerr << "File " << filename << " doesn't exist" << std::endl;
        return;
    }
    remove_string("saves", filename);
    remove(filename.c_str());
}

bool file_exists(std::string filename) {
    std::ifstream file(filename);
    return file.good();
}

