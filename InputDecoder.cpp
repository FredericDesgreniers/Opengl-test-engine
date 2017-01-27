#include "InputDecoder.h"
#include <fstream>
#include <iostream>
#include "TransitionalSweepObject.h"
#include <sstream>

InputDecoder::InputDecoder(std::string fileName):fileName(fileName)
{
	
}

WorldObject* InputDecoder::decodeWorldObject()
{
	std::ifstream file;
	file.open(fileName);
	if (file.is_open())
	{
		std::string line;
		getline(file, line);
		int objectType = stoi(line);
		switch(objectType)
		{
		case 0:
			return decodeTransitionalSweep(file);
			break;
		case 1:
			break;
		}
	}
	return new WorldObject(0, 0);
}

WorldObject* InputDecoder::decodeTransitionalSweep(std::ifstream& file)
{
	std::string line;
	getline(file, line);
	int profileCurveNum = stoi(line);
	std::cout << "Decoding " << profileCurveNum << " profile curve points..." << std::endl;
	std::vector<glm::vec3*>* profileCurve = new std::vector<glm::vec3*>;
	for(int i=0; i < profileCurveNum; i++)
	{	
		getline(file, line);

		std::stringstream  lineStream(line);

		std::string::size_type sz;     // alias of size_t
		
		float x;
		lineStream >> x;
		float y;
		lineStream >> y;
		float z;
		lineStream >> z;

		profileCurve->push_back(new glm::vec3(x, y, z));
		std::cout << x << ", " << y << ", " << z << std::endl;
	}
	getline(file, line);
	int trajectoryCurveNum = stoi(line);
	std::cout << "Decoding " << trajectoryCurveNum << " trajectory curve points..." << std::endl;
	std::vector<glm::vec3*>* trajectoryCurve = new std::vector<glm::vec3*>;
	for (int i = 0; i < trajectoryCurveNum; i++)
	{
		getline(file, line);

		std::stringstream  lineStream(line);

		std::string::size_type sz;     // alias of size_t

		float x;
		lineStream >> x;
		float y;
		lineStream >> y;
		float z;
		lineStream >> z;

		trajectoryCurve->push_back(new glm::vec3(x, y, z));
		std::cout << x << ", " << y << ", " << z << std::endl;
	}

	TransitionalSweepObject* obj = new TransitionalSweepObject(0, 0, profileCurve, trajectoryCurve);

	return obj;
}
