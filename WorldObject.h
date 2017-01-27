#pragma once
#include <GL/GLEW.H>
#include "Shader.h"

class WorldObject
{
public:
	WorldObject(int x, int y, int z);
	Shader* program;
	int getX();
	int getY();
	int getZ();
	GLuint VBO, VAO, EBO;
	virtual void render();
protected:
	int vertexNum;
	int x, y, z;
private:
	
	
};
