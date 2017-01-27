#pragma once
#include "WorldObject.h"
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>
#include "Shader.h"


class TransitionalSweepObject:public WorldObject
{
public:
	TransitionalSweepObject(int x, int y, std::vector<glm::vec3*>* profileCurve, std::vector<glm::vec3*>* trajectoryCurve);
	
	void render() override;


private:
	int size;
	GLfloat* vertices; 
	GLuint* indices;
	int indicesSize;
};
