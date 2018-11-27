#ifndef COMMONVALS
#define COMMONVALS
#include "glm/glm.hpp"
#include <vector>
#include "stb_image.h"
#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes
#endif