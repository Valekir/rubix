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
    for (int i = 0; i < n; i++) {
        move = moves[std::rand() % 12];
        std::cout << move;
        rotateSide(move);
    }
    std::cout << std::endl;
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


// Для хранения предвычисленных индексов граней
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


void SCube::rotateX(char face, int start_offset) {
    constexpr int n = 3;
    const bool is_right = (face == 'R' || face == 'r');
    const bool clockwise = (face == 'R' || face == 'l');
    
    const auto& indices = is_right ? RIGHT_FACE_INDICES : LEFT_FACE_INDICES;

    // 1. Собираем элементы грани
    std::array<SPiece, 9> face_pieces;
    for (int i = 0; i < 9; ++i) {
        face_pieces[i] = parts[indices[i].first][indices[i].second];
    }

    // 2. Применяем поворот
    constexpr std::array<int, 9> ROT_CW = {6, 3, 0, 7, 4, 1, 8, 5, 2};
    constexpr std::array<int, 9> ROT_CCW = {2, 5, 8, 1, 4, 7, 0, 3, 6};
    const auto& rot_indices = clockwise ? ROT_CW : ROT_CCW;

    std::array<SPiece, 9> rotated;
    for (int i = 0; i < 9; ++i) {
        rotated[i] = face_pieces[rot_indices[i]];
    }

    // 3. Обновляем элементы в кубе
    for (int i = 0; i < 9; ++i) {
        parts[indices[i].first][indices[i].second] = rotated[i];
    }

    // 4. Поворачиваем ориентацию
    const char dir = is_right ? (clockwise ? 'X' : 'x') : (clockwise ? 'x' : 'X');
    for (auto& piece : rotated) {
        piece.rotatePiece(dir);
    }
}


void SCube::rotateY(char face, int /*start_offset*/) {
    constexpr int n = 3;
    const bool is_upper = (face == 'U' || face == 'u');
    const bool clockwise = (face == 'U' || face == 'd');
    const int layer = is_upper ? 0 : 2;

    // Предопределенные индексы для поворотов
    static constexpr std::array<int, 9> CW_INDICES = {6,3,0,7,4,1,8,5,2};
    static constexpr std::array<int, 9> CCW_INDICES = {2,5,8,1,4,7,0,3,6};
    
    // Временное хранилище для элементов грани
    std::array<SPiece, 9> temp;
    
    // Копируем элементы грани
    for (int i = 0; i < 9; ++i) {
        temp[i] = parts[layer][i];
    }

    // Применяем поворот
    const auto& indices = clockwise ? CW_INDICES : CCW_INDICES;
    for (int i = 0; i < 9; ++i) {
        parts[layer][i] = temp[indices[i]];
    }

    // Поворачиваем ориентацию элементов
    const char dir = clockwise ? 'Y' : 'y';
    for (auto& piece : parts[layer]) {
        piece.rotatePiece(dir);
    }
}

void SCube::rotateZ(char face, int /*start_offset*/) {
    constexpr int n = 3;
    const bool is_front = (face == 'F' || face == 'f');
    const bool clockwise = (face == 'F' || face == 'b');
    
    // Предопределенные индексы граней
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

    // Индексы для поворота
    static constexpr std::array<int, 9> CW_INDICES = {6,3,0,7,4,1,8,5,2};
    static constexpr std::array<int, 9> CCW_INDICES = {2,5,8,1,4,7,0,3,6};

    // Выбираем нужную грань
    const auto& face_indices = is_front ? FRONT_INDICES : BACK_INDICES;
    
    // Временное хранилище
    std::array<SPiece, 9> temp;
    
    // Копируем элементы грани
    for (int i = 0; i < 9; ++i) {
        const auto& [layer, pos] = face_indices[i];
        temp[i] = parts[layer][pos];
    }

    // Применяем поворот
    const auto& rot_indices = clockwise ? CW_INDICES : CCW_INDICES;
    for (int i = 0; i < 9; ++i) {
        const auto& [layer, pos] = face_indices[i];
        parts[layer][pos] = temp[rot_indices[i]];
    }

    // Поворачиваем ориентацию элементов
    const char dir = (is_front == clockwise) ? 'z' : 'Z';
    for (int i = 0; i < 9; ++i) {
        const auto& [layer, pos] = face_indices[i];
        parts[layer][pos].rotatePiece(dir);
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



// void SCube::rotateY(char face, int start_offset) {
//     int n = dimension;
//     int so = start_offset;
//     int slice = 0;

//     int ind1, ind2, ind3;

//     if (tolower(face) == 'u') {
//         slice = 0;
//     } else if (tolower(face) == 'd') {
//         slice = n - 1;
//     } 

//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             // a = parts[][];

//             // parts[][] = parts[][];
//             // parts[][] = a;

//             swap(parts[so+slice][j*n + i], parts[so+slice][i*n + j]);
//         }
//     }

//     int k = n/2;            // ОЧЕНЬ ВАЖНО. без этого ломаются большие кубы
//     if (n > 3) k -= 1;
//     if (face == 'U' || face == 'd') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < k; j++) {
//                                 // ind1 = ;
//                 // ind2 = ;
//                 // a = parts[][];

//                 // parts[][] = parts[][];
//                 // parts[][] = a;
//                 swap(parts[so+slice][i*n + j], parts[so+slice][i*n + (n - 1 - j)]);
//             }
//         }
//     } else if (face == 'u' || face == 'D') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < k; j++) {
//                 swap(parts[so+slice][i], parts[so+slice][(n - 1 - j) * n + i]);
//             }
//         }
//     }

//     char dir = (face == 'U' || face == 'd') ? 'Y' : 'y';
//     for (int i = 0; i < n*n; i++) {
//         parts[so+slice][i].rotatePiece(dir);
//     }
// }


// void SCube::rotateZ(char face, int start_offset) {
//     int n = dimension, offset = 0;

//     if (tolower(face) == 'f') {
//         offset = n*n - n - n*start_offset;
//     } else if (tolower(face) == 'b') {
//         offset = 0 + n*start_offset;
//     }

//     for (int i = 0; i < n; i++) {
//          for (int j = i + 1; j < n; j++) {
//              swap(parts[i][j + offset], parts[j][i + offset]);
//          }
//     }

//     if (face == 'f' || face == 'B')  {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[j][i + offset], parts[n - j - 1][i + offset]);
//             }
//         }
//     } else if (face == 'F' || face == 'b') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[i][offset + j], parts[i][offset + n - 1 - j]);
//             }
//         }
//     }

//     char dir = (face == 'F' || face == 'b') ? 'z' : 'Z';
//     for (int i = 0; i < n; i++) {
//         for (int j = offset; j < offset + n; j++) {
//             parts[i][j].rotatePiece(dir);
//         }
//     }
// }

// void SCube::rotateX(char face, int start_offset) {
//     int n = dimension;

//     int offset = (face == 'r' || face == 'R') ? n - 1 - start_offset : 0;

//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             swap(parts[i][n*j + offset], parts[j][n*i + offset]);
//         }
//     }

//     bool clockwise = (face == 'R' || face == 'l');

//     if (!clockwise) {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[i][n*j + offset], parts[i][(n - 1 - j) * n + offset]);
//             }
//         }
//     } else if (clockwise) {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[j][(n-i-1)*n + offset], parts[n - 1 - j][(n-i-1)*n + offset]);
//             }
//         }
//     }

//     char dir = (face == 'R' || face == 'l') ? 'X' : 'x';
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             parts[i][n*j + offset].rotatePiece(dir);
//         }
//     }
// }

// void SCube::rotateY(char face, int start_offset) {
//     int n = dimension;
//     int so = start_offset;
//     int slice = 0;

//     if (tolower(face) == 'u') {
//         slice = 0;
//     } else if (tolower(face) == 'd') {
//         slice = n - 1;
//     } 

//     for (int i = 0; i < n; i++) {
//         for (int j = i + 1; j < n; j++) {
//             swap(parts[so+slice][j*n + i], parts[so+slice][i*n + j]);
//         }
//     }

//     int k = n/2;            // ОЧЕНЬ ВАЖНО. без этого ломаются большие кубы
//     if (n > 3) k -= 1;
//     if (face == 'U' || face == 'd') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < k; j++) {
//                 swap(parts[so+slice][i*n + j], parts[so+slice][i*n + (n - 1 - j)]);
//             }
//         }
//     } else if (face == 'u' || face == 'D') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < k; j++) {
//                 swap(parts[so+slice][i], parts[so+slice][(n - 1 - j) * n + i]);
//             }
//         }
//     }

//     char dir = (face == 'U' || face == 'd') ? 'Y' : 'y';
//     for (int i = 0; i < n*n; i++) {
//         parts[so+slice][i].rotatePiece(dir);
//     }
// }

// void SCube::rotateZ(char face, int start_offset) {
//     int n = dimension, offset = 0;

//     if (tolower(face) == 'f') {
//         offset = n*n - n - n*start_offset;
//     } else if (tolower(face) == 'b') {
//         offset = 0 + n*start_offset;
//     }

//     for (int i = 0; i < n; i++) {
//          for (int j = i + 1; j < n; j++) {
//              swap(parts[i][j + offset], parts[j][i + offset]);
//          }
//     }

//     if (face == 'f' || face == 'B')  {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[j][i + offset], parts[n - j - 1][i + offset]);
//             }
//         }
//     } else if (face == 'F' || face == 'b') {
//         for (int i = 0; i < n; i++) {
//             for (int j = 0; j < n/2; j++) {
//                 swap(parts[i][offset + j], parts[i][offset + n - 1 - j]);
//             }
//         }
//     }

//     char dir = (face == 'F' || face == 'b') ? 'z' : 'Z';
//     for (int i = 0; i < n; i++) {
//         for (int j = offset; j < offset + n; j++) {
//             parts[i][j].rotatePiece(dir);
//         }
//     }
// }

