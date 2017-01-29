#pragma once
#include "WorldObject.h"
#include <vector>
#include <glm/detail/type_vec3.hpp>
#include <GL/glew.h>
#include "Shader.h"


class RotationalSweepObject :public WorldObject
{
public:
	RotationalSweepObject(int x, int y, int z, std::vector<glm::vec3*>* profileCurve, int span);

	void render() override;


private:
	int size;
	GLfloat* vertices;
	GLuint* indices;
	int indicesSize;
};
