#pragma once

class Tile
{
public:
	Tile(int positionX, int positionY);
	int getHeight();
private:
	int positionX, positionY, height;
};