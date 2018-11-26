#include "Terrain.h"

Terrain::Terrain()
{
	const int count = 16384;
	const int vert = 127;
	float vertices[count * 5];
	unsigned int indices[6*vert*vert];

	int vertexPointer = 0;
	for (int i = 0; i < VERTEX; i++)
	{
		for (int j = 0; j < VERTEX; j++)
		{
			vertices[vertexPointer * 3] = (float)j / ((float)vert) * SIZE;
			vertices[(vertexPointer * 3) + 1] = 0;
			vertices[(vertexPointer * 3) + 2] = (float)i/((float)vert) * SIZE;
			vertices[(vertexPointer * 3) + 3] = (float)j / ((float)vert - 1);
			vertices[(vertexPointer * 3) + 4] = (float)i / ((float)vert - 1);
			vertexPointer++;
		}
	}

	int pointer = 0;
	for (int k = 0; k < VERTEX; k++)
	{
		for (int l = 0; l < VERTEX; l++)
		{
			int topLeft = (k*VERTEX) + l;
			int topRight = topLeft + 1;
			int bottomLeft = ((k + 1) * VERTEX) + l;
			int bottomRight = topLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}
	Mesh * ter = new Mesh();
	ter->CreateMesh(vertices, indices, (count * 5), (6 * vert*vert));
	this->model = ter;
}

Mesh * Terrain::getModel()
{
	return this->model;
}

Terrain::~Terrain()
{

}
