#pragma once
#include "Common.h"
#include <string>
#include <vector>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	SkyBox(std::vector<std::string> faceLocations);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projMatrix);

private:
	Mesh * sky;
	Shader* skyS;
	GLuint ID;
	GLuint uniformProj, uniformView;
};