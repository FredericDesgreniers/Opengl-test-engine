#pragma once

class Tile
{
public:
	Tile(int positionX, int positionY);
	int getHeight();

	void setTextureId(int id);
	int getTextureId();
private:
	int positionX, positionY, height;
	int textureId;
};