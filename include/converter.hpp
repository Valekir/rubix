#include <array>
#include "utils.hpp"
#include "cube.hpp"

class SPiece : public Piece {
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

class SCube : public Cube {
  private:
	static const int MAX_SIZE = 5;
	int dimension;
	std::array<std::array<SPiece, MAX_SIZE>, MAX_SIZE> parts;
	
	void rotateX(char face, int start_offset=0);
	void rotateY(char face, int start_offset=0);
	void rotateZ(char face, int start_offset=0);
  public:
	SCube(Cube&);
	Colors getCenterColor(std::array<int, 3> direction) const;

	std::array<std::array<SPiece, MAX_SIZE>, MAX_SIZE> getParts() const { return parts; }
	int size() const { return dimension; }

	bool isSolved();
	size_t hash() const;
	void rotate_side(char side);
};
