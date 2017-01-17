#include "WorldObject.h"
#include <iostream>

WorldObject::WorldObject(int x, int y):x(x), y(y)
{
	
}

int WorldObject::getX()
{
	return y;
}

int WorldObject::getY()
{
	return x;
}

void WorldObject::render()
{
	std::cout << "rendering world object" << std::endl;
}

