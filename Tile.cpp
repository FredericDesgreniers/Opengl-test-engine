#include "Tile.h"
#include <algorithm>

Tile::Tile(int positionX, int positionY):positionX(positionX), positionY(positionY), textureId(0)
{
	height = std::max(positionX, positionY);
}
int Tile::getHeight()
{
	return height;
}


int Tile::getTextureId()
{
	return textureId;
}

void Tile::setTextureId(int id)
{
	textureId = id;
}

