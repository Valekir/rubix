#pragma once
#include <array>
#include "cube.hpp"

class SPiece {
  protected:
	std::array <Colors, 3> color;
	std::array <int , 3> position;
	char type;

	
  public:
	SPiece();
	SPiece(std::array <Colors, 3> _color, std::array <int, 3> _position, char _type);

	std::array<int, 3> getPosition() const { return position; }
	std::array<Colors, 3> getColor() const { return color; }
	char getType() const { return type; }
	
	void rotatePiece(char dir);
	size_t hash() const;
};

class SCube {
  private:
	static const int MAX_SIZE = 3;
	int dimension;
	SPiece parts[3][9];
	
	void rotateX(char face, int start_offset=0);
	void rotateY(char face, int start_offset=0);
	void rotateZ(char face, int start_offset=0);
  public:
	SCube(Cube);
	Colors getCenterColor(std::array<int, 3> direction) const;
	std::vector<SPiece> getFaceElements(const std::array<int, 3>& dir) const;

	auto getParts() const { return parts; }
	int size() const { return dimension; }

	bool isSolved() const;
	size_t hash() const;
	void rotateSide(char side);

	void scramble(int n);
};

