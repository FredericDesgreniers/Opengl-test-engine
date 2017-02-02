#include "RotationalSweepObject.h"
#include "GLM/glm.hpp"
#include <glm/gtc/matrix_transform.inl>

RotationalSweepObject::RotationalSweepObject(int x, int y, int z, std::vector<glm::vec3*>* profileCurve, int span):WorldObject(x,y,z)
{
	//open assignment 1 shaders
	//program = new Shader("a1Shader.vs", "a1Shader.fs");

	//get the size of the vertices array. 
	int size = profileCurve->size() * (span) * 3;
	//number of indices (2 triangles per "square")
	int indicesSize = (profileCurve->size() - 1) * (span) * 2 * 3;

	vao = new VaoObject(size, indicesSize);

	float angleSize = glm::radians(360.0f / float(span));

	//go through every vector on the profile curve
	for (int s = 0; s < span; s++)
	{
	for (int p = 0; p < profileCurve->size(); p++)
	{
		glm::vec4 pVec = glm::vec4(profileCurve->at(p)->x, profileCurve->at(p)->y, profileCurve->at(p)->z, 1);
		//for each profile point, go through the trajectory vectors
		
			glm::mat4x4 rotMatrix = glm::mat4x4(1.0f);
			glm::vec4 rVec = pVec * glm::rotate(rotMatrix,angleSize * s, glm::vec3(0, 1, 0));
			
			vao->addToVBO(rVec);

			if (p > 0)
			{
				int currentIndex = vao->getCurrentVboIndex() / 3 - 1;

				int topRightPoint = currentIndex;
				int bottomRightPoint = topRightPoint - 1;
				int topLeftPoint = topRightPoint - profileCurve->size();
				int bottomLeftPoint = topLeftPoint - 1;

				if (s > 0) {
					vao->addTrianglesFromPointsToEBO(topRightPoint, bottomRightPoint, topLeftPoint, bottomLeftPoint);
				}else
				{
					int endTopPoint = size / 3 - profileCurve->size() + currentIndex;
					int endBottomPoint = endTopPoint - 1;

					vao->addTrianglesFromPointsToEBO(topRightPoint, bottomRightPoint, endTopPoint, endBottomPoint);
				}
			}

		}
	}

	vao->setupVao();

}

void RotationalSweepObject::render()
{
	glDrawElements(GL_TRIANGLES, vao->getEboSize(), GL_UNSIGNED_INT, 0);
}

