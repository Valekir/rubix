#include "converter.hpp"
#include "utils.hpp"


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

    if (tolower(dir) == 'x') {
        swap(color[1], color[2]);
    } else if (tolower(dir) == 'y') {
        swap(color[0], color[2]);
    } else if (tolower(dir) == 'z') {
        swap(color[0], color[1]);
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
    } else if (t_side == 's') {
        char t_dir = side == 's' ? 'F': 'f';
        for (int i = 1; i <= n-2; i++) {
            rotateZ(t_dir, i);
        }
    } else if (t_side == 'r' || t_side == 'l') {
        rotateX(side);
    } else if (t_side == 'm') {
        char t_dir = side == 'm' ? 'r': 'R';
        for (int i = 1; i <= n-2; i++) {
            rotateX(t_dir, i);
        }
    } else if (t_side == 'u' || t_side == 'd') {
        rotateY(side);
    } else if (t_side == 'e') {
        char t_dir = side == 'e' ? 'u': 'U';
        for (int i = 1; i <= n-2; i++) {
            rotateY(t_dir, i);
        }
    }
}

void SCube::rotateX(char face, int start_offset) {
    int n = dimension, offset = 0, k = 0;

    if (face == 'r' || face == 'R') {
        offset = n - 1 - start_offset;
    } else {
        offset = 0;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(parts[i][n*j + offset], parts[j][n*i + offset]);
        }
    }

    if (face == 'r' || face == 'L') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][n*j + offset], parts[i][(n - 1 - j) * n + offset]);
            }
        }
    } else if (face == 'R' || face == 'l') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][(n-i-1)*n + offset], parts[n - 1 - j][(n-i-1)*n + offset]);
            }
        }
    }

    char dir = (face == 'R' || face == 'l') ? 'X' : 'x';
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            parts[i][n*j + offset].rotatePiece(dir);
        }
    }
}

void SCube::rotateY(char face, int start_offset) {
    int n = dimension;
    int slice = 0;
    int so = start_offset;

    if (tolower(face) == 'u') {
        slice = 0;
    } else if (tolower(face) == 'd') {
        slice = n - 1;
    } 

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            swap(parts[so+slice][j*n + i], parts[so+slice][i*n + j]);
        }
    }

    int k = n/2;            // ОЧЕНЬ ВАЖНО. без этого ломаются большие кубы
    if (n > 3) k -= 1;
    if (face == 'U' || face == 'd') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                swap(parts[so+slice][i*n + j], parts[so+slice][i*n + (n - 1 - j)]);
            }
        }
    } else if (face == 'u' || face == 'D') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                swap(parts[so+slice][i], parts[so+slice][(n - 1 - j) * n + i]);
            }
        }
    }

    char dir = (face == 'U' || face == 'd') ? 'Y' : 'y';
    for (int i = 0; i < n*n; i++) {
        parts[so+slice][i].rotatePiece(dir);
    }
}

void SCube::rotateZ(char face, int start_offset) {
    int n = dimension, offset = 0;

    if (tolower(face) == 'f') {
        offset = n*n - n - n*start_offset;
    } else if (tolower(face) == 'b') {
        offset = 0 + n*start_offset;
    }

    for (int i = 0; i < n; i++) {
         for (int j = i + 1; j < n; j++) {
             swap(parts[i][j + offset], parts[j][i + offset]);
         }
    }

    if (face == 'f' || face == 'B')  {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[j][i + offset], parts[n - j - 1][i + offset]);
            }
        }
    } else if (face == 'F' || face == 'b') {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n/2; j++) {
                swap(parts[i][offset + j], parts[i][offset + n - 1 - j]);
            }
        }
    }

    char dir = (face == 'F' || face == 'b') ? 'z' : 'Z';
    for (int i = 0; i < n; i++) {
        for (int j = offset; j < offset + n; j++) {
            parts[i][j].rotatePiece(dir);
        }
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
