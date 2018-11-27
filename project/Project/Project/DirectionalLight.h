#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{

public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat a, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void use(GLuint aLoc, GLuint cLoc, GLuint dILoc, GLuint dLoc);


	~DirectionalLight();

private:
	glm::vec3 dir; 
};

