#include "World.h"

World::World(int width, int height): width(width), height(height)
{
	
}

int World::getWidth()
{
	return width;
}

int World::getHeight()
{
	return height;
}

std::vector<WorldObject*>* World::getWorldObjects()
{
	return &worldObjects;
}

void World::spawnWorldObject(WorldObject* worldObject)
{
	worldObjects.push_back(worldObject);
}
