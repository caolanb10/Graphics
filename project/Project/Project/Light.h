#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>


class Light
{
public:

	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a, GLfloat dIntensity);
	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

};

