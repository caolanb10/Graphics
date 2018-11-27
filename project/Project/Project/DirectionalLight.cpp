#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	dir = glm::vec3(0.0f, -1.0f, 0.0f);
}


DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
								   GLfloat a, GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, a, dIntensity)
{
	dir = glm::vec3(xDir, yDir, zDir);
}


void DirectionalLight::use(GLuint aLoc, GLuint cLoc, GLuint dILoc, GLuint dLoc)
{
	glUniform3f(cLoc, colour.x, colour.y, colour.z);
	glUniform1f(aLoc, ambientIntensity);

	glUniform3f(dLoc, dir.x, dir.y, dir.z);
	glUniform1f(dILoc, diffuseIntensity);
}


DirectionalLight::~DirectionalLight()
{
}
