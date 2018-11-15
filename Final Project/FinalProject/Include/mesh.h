#ifndef MESH
#define MESH
#include "maths_funcs.h"
#include "shader_s.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

using namespace std;
#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

#pragma endregion SimpleTypes
ModelData load_mesh(const char* file_name);

#endif