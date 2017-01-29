#include "RotationalSweepObject.h"
#include "GLM/glm.hpp"
#include <glm/gtc/matrix_transform.inl>

RotationalSweepObject::RotationalSweepObject(int x, int y, int z, std::vector<glm::vec3*>* profileCurve, int span):WorldObject(x,y,z)
{
	//open assignment 1 shaders
	program = new Shader("a1Shader.vs", "a1Shader.fs");
	//get the size of the vertices array. 
	size = profileCurve->size() * (span) * 6;

	//matrix of vertices
	vertices = new GLfloat[size];

	//number of indices (2 triangles per "square")
	indicesSize = (profileCurve->size() - 1) * (span) * 2 * 3;

	//indices matrix
	indices = new GLuint[indicesSize];
	std::cout << "Span of " << span << std::endl;
	//indice counter
	int iIndex = 0;
	//vertex counter
	int index = 0;

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
			
			float height = (float(p) / float(profileCurve->size()));
			//translate the profile vector by the trajectory vector and add it to the vector array
			vertices[index] = rVec.x;
			vertices[index + 1] = rVec.y;
			vertices[index + 2] = rVec.z;
			vertices[index + 3] = height;
			vertices[index + 4] = height;
			vertices[index + 5] = 0;
			index += 6;

			//if we're not in a bottom / left row, find the indices that compose the two triangles
			//the two triangles compose the square that is to the bottom-left of the current vector
			if (s > 0 && p > 0)
			{
				//first triangle
				indices[iIndex] = index / 6 - 1;
				indices[iIndex + 1] = index / 6 - 1 - 1;
				indices[iIndex + 2] = index / 6 - 1 - profileCurve->size() - 1;
				iIndex += 3;

				//second triangle
				indices[iIndex] = index / 6 - 1;
				indices[iIndex + 1] = index / 6 - profileCurve->size() - 1;
				indices[iIndex + 2] = index / 6 - 1 - profileCurve->size() - 1;
				iIndex += 3;
			}else if (p > 0)
			{
				//first triangle
				indices[iIndex] = index / 6 - 1;
				indices[iIndex + 1] = index / 6 - 1 - 1;
				indices[iIndex + 2] = size /6 - 1 - profileCurve->size()+index/6 - 1;
				iIndex += 3;

				//second triangle
				indices[iIndex] = index / 6 - 1;
				indices[iIndex + 1] = size /6-profileCurve->size() + index/6 - 1;
				indices[iIndex + 2] = size /6- profileCurve->size() +index/6 - 1 - 1;
				iIndex += 3;
			}

		}
	}
	//set the vertex num, starts at 1 instead of 0 so add 1
	vertexNum = index + 1;
	//print the two matrix arrays for debug 
	/*
	std::cout << "Vertexes " << size << std::endl;
	int c = 0;
	for (int i = 0; i < size; i++)
	{
		printf("%f ", vertices[i]);
		c++;
		if (c > 2)
		{
			c = 0;
			std::cout << std::endl;
		}
	}
*	 
	int c = 0;
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
//*	 */
	//bind the VAO/VBO/EBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//bind the vertices array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*size, vertices, GL_STATIC_DRAW);

	//bind indices array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*indicesSize, indices, GL_STATIC_DRAW);

	//tell opengl to look at them in pairs of 3 (x,y,z)
	//TODO add color here, so (x,y,z,r,g,b)?
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << size << std::endl;
}

void RotationalSweepObject::render()
{
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

