#include "World.h"
#include <GLFW/glfw3.h>
#include <ctime>

World::World(int width, int height): width(width), height(height)
{
	tiles = new Tile**[width];
	for(int x=0; x < width; x++)
	{
		tiles[x] = new Tile*[height];
		for(int y=0; y < height; y++)
		{
			tiles[x][y] = new Tile(x,y);
			if(x%15 < 3 || y%15 < 3)
			{
				tiles[x][y]->setTextureId(1);
			}
		}
	}
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
Tile*** World::getTiles()
{
	return tiles;
}


void World::generateRoads()
{

}
