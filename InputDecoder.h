#pragma once
#include <string>
#include "WorldObject.h"

class InputDecoder
{
public:
	InputDecoder(std::string fileName);
	WorldObject* decodeWorldObject();

private:
	WorldObject* decodeTransitionalSweep(std::ifstream& file);
	std::string fileName;
};
