#pragma once

class WorldObject
{
public:
	WorldObject(int x, int y);
	
	int getX();
	int getY();

	virtual void render();
private:
	int x, y;
};