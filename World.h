#pragma once
#include <vector>
#include "WorldObject.h"
#include "Tile.h"

class World
{
public:
	World(int width, int height);

	int getWidth();
	int getHeight();

	std::vector<WorldObject*>* getWorldObjects();

	void spawnWorldObject(WorldObject* worldObject);

	Tile*** getTiles();

	void generateRoads();
	
private:
	int width, height;
	std::vector<WorldObject*> worldObjects;
	Tile*** tiles;
};
