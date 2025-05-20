#include "converter.hpp"


//____________________________________________SPiece_______________________________________________

SPiece::SPiece() {
	color = {Colors::K, Colors::K, Colors::K};
	position = {0, 0, 0};
    type = '0';
}

SPiece::SPiece(std::array <Colors, 3> _color, std::array <int, 3> _position, char _type) {
    for (int i = 0; i < 3; i++) {
        color[i]  = _color[i];
        position[i] = _position[i];
    }
    type = _type;
}

void SPiece::rotatePiece(char dir) {
    position = rotateArray(position, dir);   
    Colors a, b;
    switch (tolower(dir)) {
        case 'x':
            a = color[1];
            b = color[2]; 
            color[1] = b;
            color[2] = a;
            break;
        case 'y':
            a = color[0];
            b = color[2]; 
            color[0] = b;
            color[2] = a;
            break;
        case 'z':
            a = color[0];
            b = color[1]; 
            color[0] = b;
            color[1] = a;
            break;
    }
}
//____________________________________________SCube________________________________________________

SCube::SCube(Cube cube) {
	dimension = cube.size();

	if (dimension > MAX_SIZE) {
		throw std::invalid_argument("Cube dimension exceeds MAX_SIZE");
	}

	auto cube_parts = cube.get_parts();

	for (int layer = 0; layer < dimension; ++layer) {
		for (int i = 0; i < dimension * dimension; ++i) {
			auto piece = cube_parts[layer][i];
			
			
	        auto colors = piece.get_color();
            auto pos = piece.get_pos();
            auto type = piece.get_type();

           	parts[layer][i] = SPiece(
				{colors[0], colors[1], colors[2]},
				{pos[0], pos[1], pos[2]},
				type
			);
		}		
	}
}

Colors SCube::getCenterColor(std::array<int, 3> direction) const {
   	int layer = 0;
    int pos = 0;

    int dir = 0;
    if (direction[1] != 0) dir = 1;
    if (direction[2] != 0) dir = 2;

    if (direction[0] == 1) {       		// Правая грань
        layer = 1;
        pos = 5;
    } else if (direction[0] == -1) { 	// Левая грань
        layer = 1;
        pos = 3;
    } else if (direction[1] == 1) {  	// Верхняя грань
        layer = 0;
        pos = 5;
    } else if (direction[1] == -1) { 	// Нижняя грань
        layer = 2;
        pos = 5;
    } else if (direction[2] == 1) {  	// Передняя грань
        layer = 1;
        pos = 7;
    } else if (direction[2] == -1) { 	// Задняя грань
        layer = 1;
        pos = 1;
    }
    
    return parts[layer][pos].getColor()[dir];
}


void SCube::scramble(int n) {
    std::array<char, 12> moves = {'F','f','B','b','D','d','U','u','R','r','L','l'};
    char move;
    std::srand(std::time(0));
    std::cout << "\033[32m" << "\nScramble: ";
    for (int i = 0; i < n; i++) {
        move = moves[std::rand() % 12];
        std::cout << move;
        rotateSide(move);
    }
    std::cout << "\033[0m\n" << std::endl;
}

//___________________________________________Is solved______________________________________________



// Вспомогательная функция для получения элементов грани
std::vector<SPiece> SCube::getFaceElements(const std::array<int, 3>& dir) const {
    std::vector<SPiece> elements;
    int axis = -1;
    int value = 0;

    // Определяем ось и значение направления
    for (int i = 0; i < 3; ++i) {
        if (dir[i] != 0) {
            axis = i;
            value = dir[i];
            break;
        }
    }

    // Собираем элементы для каждой грани
    switch (axis) {
        case 0: {
            const int layer = (value > 0) ? dimension - 1 : 0;
            for (int x = 0; x < dimension; ++x) {
                for (int y = 0; y < dimension; ++y) {
                    elements.push_back(parts[x][y * dimension + layer]);
                }
            }
            break;
        }
        case 1: {
            const int layer = (value > 0) ? 0 : dimension - 1;
            for (int y = 0; y < dimension; ++y) {
                for (int z = 0; z < dimension; ++z) {
                    elements.push_back(parts[layer][y * dimension + z]);
                }
            }
            break;
        }
        case 2: {
            const int layer = (value > 0) ? dimension - 1 : 0;
            for (int x = 0; x < dimension; ++x) {
                for (int z = 0; z < dimension; ++z) {
                    elements.push_back(parts[x][layer * dimension + z]);
                }
            }
            break;
        }
    }

    return elements;
}

bool SCube::isSolved() {
    constexpr std::array<std::array<int, 3>, 6> directions = {{
        {0, 1, 0},  // Верх
        {0, -1, 0}, // Низ
        {1, 0, 0},  // Право
        {-1, 0, 0}, // Лево
        {0, 0, 1},  // Перед
        {0, 0, -1}  // Зад
    }};

    for (const auto& dir : directions) {
        const Colors center_color = getCenterColor(dir);
        const auto face_elements = getFaceElements(dir);
        
        int index = 0;
        if (dir[1] != 0) index = 1;
        if (dir[2] != 0) index = 2;
        
        
        for (const auto& piece : face_elements) {
            auto colors = piece.getColor();

                if (colors[index] != center_color) return false;

        }
    }
    return true;
}



//_____________________________________Rotating cube sides_________________________________________

void SCube::rotateSide(char side) {
    int n = dimension;
    char t_side = tolower(side);
    
    if (t_side == 'f' || t_side == 'b') {
        rotateZ(side);
    } else if (t_side == 'r' || t_side == 'l') {
        rotateX(side);
    } else if (t_side == 'u' || t_side == 'd') {
        rotateY(side);
    }
}

// Для хранения предвычесленных индексов поворота
static constexpr std::array<int, 9> CW_INDICES = {6,3,0,7,4,1,8,5,2};
static constexpr std::array<int, 9> CCW_INDICES = {2,5,8,1,4,7,0,3,6};

// Для хранения предвычисленных индексов граней по оси X
static constexpr std::array<std::pair<int, int>, 9> RIGHT_FACE_INDICES = {{
    {0, 2}, {0, 5}, {0, 8},
    {1, 2}, {1, 5}, {1, 8},
    {2, 2}, {2, 5}, {2, 8}
}};

static constexpr std::array<std::pair<int, int>, 9> LEFT_FACE_INDICES = {{
    {0, 0}, {0, 3}, {0, 6}, 
    {1, 0}, {1, 3}, {1, 6}, 
    {2, 0}, {2, 3}, {2, 6}
}};

// Для хранения предвычисленных индексов граней по оси Y
static constexpr std::array<std::pair<int, int>, 9> TOP_INDICES = {
    {{0,0}, {0,1}, {0,2},
    {0,3}, {0,4}, {0,5},
    {0,6}, {0,7}, {0,8},}
};

static constexpr std::array<std::pair<int, int>, 9> BOTTOM_INDICES = {
    {{2,0}, {2,1}, {2,2},
    {2,3}, {2,4}, {2,5},
    {2,6}, {2,7}, {2,8},}
};

// Для хранения предвычисленных индексов граней по оси Z
static constexpr std::array<std::pair<int, int>, 9> FRONT_INDICES = {
    {{0,6}, {0,7}, {0,8},
        {1,6}, {1,7}, {1,8},
        {2,6}, {2,7}, {2,8}}
};

static constexpr std::array<std::pair<int, int>, 9> BACK_INDICES = {
    {{0,0}, {0,1}, {0,2},
        {1,0}, {1,1}, {1,2},
        {2,0}, {2,1}, {2,2}}
};

void SCube::rotateX(char face, int layer) {
    const bool clockwise = (face == 'R' || face == 'l');
    const auto& indices = (face == 'R' || face == 'r') ? RIGHT_FACE_INDICES : LEFT_FACE_INDICES;

    std::array<SPiece, 9> buffer;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        buffer[i] = parts[row][col + layer];
    }

    const auto& map = clockwise ? CW_INDICES : CCW_INDICES;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row][col + layer] = buffer[map[i]];
    }

    char dir = (face == 'R' || face == 'l') ? 'X' : 'x';
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row][col + layer].rotatePiece(dir);
    }
}

void SCube::rotateY(char face, int layer) {
    const bool clockwise = (face == 'U' || face == 'd');
    const auto& indices = (face == 'U' || face == 'u') ? TOP_INDICES : BOTTOM_INDICES;

    std::array<SPiece, 9> buffer;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        buffer[i] = parts[row + layer][col];
    }

    const auto& map = clockwise ? CW_INDICES : CCW_INDICES;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row + layer][col] = buffer[map[i]];
    }

    char dir = clockwise ? 'Y' : 'y';
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row + layer][col].rotatePiece(dir);
    }
}


void SCube::rotateZ(char face, int layer) {
    const bool clockwise = (face == 'F' || face == 'b');
    const auto& indices = (face == 'F' || face == 'f') ? FRONT_INDICES : BACK_INDICES;

    std::array<SPiece, 9> buffer;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        buffer[i] = parts[row][col + layer];
    }

    const auto& map = clockwise ? CW_INDICES : CCW_INDICES;
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row][col + layer] = buffer[map[i]];
    }

    char dir = clockwise ? 'Z' : 'z';
    for (int i = 0; i < 9; ++i) {
        const auto& [row, col] = indices[i];
        parts[row][col + layer].rotatePiece(dir);
    }
}

//___________________________________________Hashing_______________________________________________

size_t SPiece::hash() const {
	const size_t prime = 0x100000001B3; // Большое простое число
    size_t hash = static_cast<size_t>(type);

    // Хэшируем цвета
    for (Colors item : color) {
        hash = hash * prime + static_cast<size_t>(item);
    }

    // Хэшируем позицию
    for (int coord : position) {
        hash = hash * prime + static_cast<size_t>(coord + 1); // +1 чтобы избежать -0/+0
    }

    return hash;
}

size_t SCube::hash() const {
	const size_t seed = 0xC0FFEE; // Начальное значение хэша
    size_t hash = seed;

    // Хэшируем все элементы кубика
    for (const auto& layer : parts) {
        for (const auto& piece : layer) {
            hash ^= piece.hash() + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
    }

    // Хэшируем размерность кубика
    hash ^= std::hash<int>{}(dimension) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

    return hash;
}
