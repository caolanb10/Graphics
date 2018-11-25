#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>


class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a,
	      GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void use(GLuint aLoc, GLuint cLoc, GLuint dILoc, GLuint dLoc);
	
	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	
	glm::vec3 dir;
	GLfloat diffuseIntensity;

};

