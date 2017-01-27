#include "WorldObject.h"
#include <iostream>

WorldObject::WorldObject(int x, int y, int z):x(x), y(y), z(z)
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

int WorldObject::getZ()
{
	return z;
}


void WorldObject::render()
{
	std::cout << "rendering world object" << std::endl;
}

