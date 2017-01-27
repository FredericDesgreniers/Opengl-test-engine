#include "TransitionalSweepObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.inl>
#include <string>


TransitionalSweepObject::TransitionalSweepObject(int x, int y, std::vector<glm::vec3*>* profileCurve, std::vector<glm::vec3*>* trajectoryCurve):WorldObject(x,y)
{
	program = new Shader("a1Shader.vs", "a1Shader.fs");
	size = profileCurve->size() * trajectoryCurve->size() * 3;
	vertices = new GLfloat[size];
	indicesSize = (profileCurve->size()-1) * (trajectoryCurve->size()-1) *2*3;
	indices = new GLuint[indicesSize];

	int iIndex = 0;
	int index = 0;

	for(int p=0; p < profileCurve->size(); p++)
	{
		glm::vec3* pVec = profileCurve->at(p);
		for (int t = 0; t < trajectoryCurve->size(); t++)
		{
			glm::vec3* tVec = trajectoryCurve->at(t);

			vertices[index] = pVec->x + tVec->x;
			vertices[index+1] = pVec->y + tVec->y;
			vertices[index+2] = pVec->z + tVec->z;
			index+= 3;

			if(t > 0 && p > 0)
			{
				indices[iIndex] = index / 3 - 1;
				indices[iIndex+1] = index / 3 - 1 - 1;
				indices[iIndex+2] = index / 3 - 1-trajectoryCurve->size() - 1;
				iIndex += 3;

				indices[iIndex] = index / 3 - 1;
				indices[iIndex + 1] = index / 3 - trajectoryCurve->size() - 1;
				indices[iIndex + 2] = index / 3 - 1 - trajectoryCurve->size() - 1;
				iIndex += 3;
			}

		}
	}
	
	vertexNum = index+1;
	std::cout << "Vertexes " << size << std::endl;
	int c = 0;
	for (int i = 0; i < size; i++)
	{
		printf("%f ",vertices[i]);
		c++;
		if(c > 2)
		{
			c = 0;
			std::cout << std::endl;
		}
	}
	c = 0;
	std::cout << "Indices " << indicesSize << std::endl;
	for (int i = 0; i < indicesSize; i++)
	{
		printf("%d ", indices[i]);
		c++;
		if (c > 2)
		{
			c = 0;
			std::cout << std::endl;
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*size, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << size << std::endl;
}

void TransitionalSweepObject::render()
{
	
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	
}
