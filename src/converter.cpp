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
        position[i] = position[i];
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

SCube::SCube(Cube& cube) {
	dimension = cube.size();

	if (dimension > MAX_SIZE) {
		throw std::invalid_argument("Cube dimension exceeds MAX_SIZE");
	}

	auto cube_parts = cube.get_parts();

	for (int layer = 0; layer < dimension; ++layer) {
		for (int pos = 0; pos < dimension * dimension; ++pos) {
			auto piece = cube_parts[layer][pos];

			parts[layer][pos] = SPiece(
				{piece.get_color()[0], piece.get_color()[1], piece.get_color()[2]},
				{piece.get_pos()[0], piece.get_pos()[1], piece.get_pos()[2]},
				piece.get_type()
			);
		}
	}
}

Colors SCube::getCenterColor(std::array<int, 3> direction) const {
   	int layer = 0;
    int pos = 0;

    if (direction[0] == 1) {       		// Правая грань
        layer = dimension - 1;
        pos = (dimension * dimension) / 2;
    } else if (direction[0] == -1) { 	// Левая грань
        layer = 0;
        pos = (dimension * dimension) / 2;
    } else if (direction[1] == 1) {  	// Верхняя грань
        layer = dimension / 2;
        pos = dimension * (dimension - 1) + dimension / 2;
    } else if (direction[1] == -1) { 	// Нижняя грань
        layer = dimension / 2;
        pos = dimension / 2;
    } else if (direction[2] == 1) {  	// Передняя грань
        layer = dimension / 2;
        pos = (dimension * dimension) - dimension / 2 - 1;
    } else if (direction[2] == -1) { 	// Задняя грань
        layer = dimension / 2;
        pos = dimension / 2;
    }

    return parts[layer][pos].getColor()[1]; // Центральный цвет
}

bool SCube::isSolved() {
    // Список всех направлений граней
    constexpr std::array<std::array<int, 3>, 6> directions = {{
        {0, 1, 0},  // Верх
        {0, -1, 0}, // Низ
        {1, 0, 0},  // Право
        {-1, 0, 0}, // Лево
        {0, 0, 1},  // Перед
        {0, 0, -1}  // Зад
    }};

    for (const auto& dir : directions) {
        // Получаем цвет центра текущей грани
        const Colors center = getCenterColor(dir);
        
        // Определяем индекс цвета для направления
        int axis = 0;
        while (axis < 3 && dir[axis] == 0) ++axis;
        const int color_idx = axis;

        // Проверяем элементы грани
        if (dir[1] != 0) { // Верх/Низ
            const int layer = (dir[1] > 0) ? 0 : dimension-1;
            for (const auto& piece : parts[layer]) {
                if (piece.getColor()[color_idx] != center) return false;
            }
        } 
        else if (dir[0] != 0) { // Право/Лево
            const int layer = (dir[0] > 0) ? dimension-1 : 0;
            const int offset = (dir[0] > 0) ? dimension-1 : 0;
            for (int i = 0; i < dimension; ++i) {
                const int pos = i * dimension + offset;
                if (parts[layer][pos].getColor()[color_idx] != center) return false;
            }
        } 
        else { // Перед/Зад
            const int start = (dir[2] > 0) ? dimension*(dimension-1) : 0;
            const int end = start + dimension;
            for (int layer = 0; layer < dimension; ++layer) {
                for (int pos = start; pos < end; ++pos) {
                    if (parts[layer][pos].getColor()[color_idx] != center) return false;
                }
            }
        }
    }
    
    return true;
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