#pragma once
#include <GL/GLEW.H>
#include "Shader.h"
#include <glm/detail/type_vec3.hpp>
#include "VaoObject.h"

class WorldObject
{
public:
	WorldObject(int x, int y, int z);
	Shader* program;
	int getX();
	int getY();
	int getZ();

	virtual void render();

	VaoObject* getVaoObject();


	glm::vec3 rotation;
protected:
	int x, y, z;
	VaoObject* vao;
private:
	
	
};
