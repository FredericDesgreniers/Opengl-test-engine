#pragma once
#include <GL/GLEW.H>
#include "Shader.h"

class WorldObject
{
public:
	WorldObject(int x, int y);
	Shader* program;
	int getX();
	int getY();
	GLuint VBO, VAO, EBO;
	virtual void render();
protected:
	int vertexNum;
private:
	int x, y;
	
};
