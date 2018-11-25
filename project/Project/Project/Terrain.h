#pragma once

#include "Mesh.h"
#include "Texture.h"
 
class Terrain
{
public:
	Terrain();
	Terrain(Texture tex);
	Mesh * getModel();
	~Terrain();

private:
	float SIZE = 800;
	int VERTEX = 128;

	Mesh * model;
	Texture tex;
};

