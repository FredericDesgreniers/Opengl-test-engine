#pragma once
#include <string>
#include "WorldObject.h"

class InputDecoder
{
public:
	InputDecoder(std::string fileName);
	WorldObject* decodeWorldObject(int x, int y, int z);

private:
	WorldObject* decodeTransitionalSweep(std::ifstream& file,int x, int y, int z);
	WorldObject* decodeRotationalSweep(std::ifstream& file, int x, int y, int z);
	std::string fileName;
};
