
#include "TransitionalSweepObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.inl>
#include <string>


TransitionalSweepObject::TransitionalSweepObject(int x, int y, int z, std::vector<glm::vec3*>* profileCurve, std::vector<glm::vec3*>* trajectoryCurve):WorldObject(x,y,z)
{
	//open assignment 1 shaders

	//program = new Shader("a1Shader.vs", "a1Shader.fs");


	//get the size of the vertices array. 
	int size = profileCurve->size() * trajectoryCurve->size() * 3;
	

	
	//number of indices (2 triangles per "square")
	int indicesSize = (profileCurve->size()-1) * (trajectoryCurve->size()-1) *2*3;
	vao = new VaoObject(size, indicesSize);


	//go through every vector on the profile curve
	for(int p=0; p < profileCurve->size(); p++)
	{
		glm::vec3* pVec = profileCurve->at(p);
		//for each profile point, go through the trajectory vectors
		for (int t = 0; t < trajectoryCurve->size(); t++)
		{
			float height = (float(p) / float(profileCurve->size()));
			glm::vec3* tVec = trajectoryCurve->at(t);
			

			//translate the profile vector by the trajectory vector and add it to the vector array
			vao->addToVBO(*pVec + *tVec - *profileCurve->at(0));

			//if we're not in a bottom / left row, find the indices that compose the two triangles
			//the two triangles compose the square that is to the bottom-left of the current vector
			if(t > 0 && p > 0)
			{
				int currentIndex = vao->getCurrentVboIndex() / 3 - 1;

				int topRightPoint = currentIndex;
				int bottomRightPoint = topRightPoint - 1;
				int topLeftPoint = topRightPoint - trajectoryCurve->size();
				int bottomLeftPoint = topLeftPoint - 1;

				vao->addTrianglesFromPointsToEBO(topRightPoint, bottomRightPoint, topLeftPoint, bottomLeftPoint);

			}

		}

	}
	vao->setupVao();
}

void TransitionalSweepObject::render()
{
	//render as a triangle
	glDrawElements(GL_TRIANGLES, vao->getEboSize(), GL_UNSIGNED_INT, 0);
	
}
