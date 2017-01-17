#pragma once
#include <vector>
#include "WorldObject.h"

class World
{
public:
	World(int width, int height);

	int getWidth();
	int getHeight();

	std::vector<WorldObject*>* getWorldObjects();

	void spawnWorldObject(WorldObject* worldObject);
	
private:
	int width, height;
	std::vector<WorldObject*> worldObjects;
};
