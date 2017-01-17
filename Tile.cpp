#include "Tile.h"
#include <algorithm>

Tile::Tile(int positionX, int positionY):positionX(positionX), positionY(positionY)
{
	height = std::max(positionX, positionY);
}
int Tile::getHeight()
{
	return height;
}
