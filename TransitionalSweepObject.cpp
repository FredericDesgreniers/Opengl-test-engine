
#include "TransitionalSweepObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.inl>
#include <string>


TransitionalSweepObject::TransitionalSweepObject(int x, int y, int z, std::vector<glm::vec3*>* profileCurve, std::vector<glm::vec3*>* trajectoryCurve):WorldObject(x,y,z)
{
	//open assignment 1 shaders
	program = new Shader("a1Shader.vs", "a1Shader.fs");
	//get the size of the vertices array. 
	size = profileCurve->size() * trajectoryCurve->size() * 3;
	
	//matrix of vertices
	vertices = new GLfloat[size];
	
	//number of indices (2 triangles per "square")
	indicesSize = (profileCurve->size()-1) * (trajectoryCurve->size()-1) *2*3;
	
	//indices matrix
	indices = new GLuint[indicesSize];
	
	//indice counter
	int iIndex = 0;
	//vertex counter
	int index = 0;

	//go through every vector on the profile curve
	for(int p=0; p < profileCurve->size(); p++)
	{
		glm::vec3* pVec = profileCurve->at(p);
		//for each profile point, go through the trajectory vectors
		for (int t = 0; t < trajectoryCurve->size(); t++)
		{
			glm::vec3* tVec = trajectoryCurve->at(t);
				
			//translate the profile vector by the trajectory vector and add it to the vector array
			vertices[index] = pVec->x + tVec->x;
			vertices[index+1] = pVec->y + tVec->y;
			vertices[index+2] = pVec->z + tVec->z;
			index+= 3;
			
			//if we're not in a bottom / left row, find the indices that compose the two triangles
			//the two triangles compose the square that is to the bottom-left of the current vector
			if(t > 0 && p > 0)
			{
				//first triangle
				indices[iIndex] = index / 3 - 1;
				indices[iIndex+1] = index / 3 - 1 - 1;
				indices[iIndex+2] = index / 3 - 1-trajectoryCurve->size() - 1;
				iIndex += 3;
				
				//second triangle
				indices[iIndex] = index / 3 - 1;
				indices[iIndex + 1] = index / 3 - trajectoryCurve->size() - 1;
				indices[iIndex + 2] = index / 3 - 1 - trajectoryCurve->size() - 1;
				iIndex += 3;
			}

		}
	}
	//set the vertex num, starts at 1 instead of 0 so add 1
	vertexNum = index+1;
	//print the two matrix arrays for debug 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	std::cout << size << std::endl;
}

void TransitionalSweepObject::render()
{
	//render as a triangle
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
	
}
