#pragma once
#include <GL/GLEW.H>
#include "Shader.h"
#include <glm/detail/type_vec3.hpp>

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

	glm::vec3 rotation;
protected:
	int vertexNum;
	int x, y, z;
private:
	
	
};
