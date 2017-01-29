#include "WorldObject.h"
#include <iostream>

WorldObject::WorldObject(int x, int y, int z):x(x), y(y), z(z)
{
	
}

int WorldObject::getX()
{
	return x;
}

int WorldObject::getY()
{
	return y;
}

int WorldObject::getZ()
{
	return z;
}


void WorldObject::render()
{
	std::cout << "rendering world object" << std::endl;
}

