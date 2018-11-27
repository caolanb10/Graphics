#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL\glew.h>
#include "Common.h"

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateModel(ModelData mod);
	void RenderMesh();
	void RenderModel();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	int size;
};

