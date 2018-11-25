#include "Light.h"

Light::Light() 
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	dir = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = a;
	dir = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;

}

void Light::use(GLuint aLoc, GLuint cLoc, GLuint dILoc, GLuint dLoc)
{
	glUniform3f(cLoc, colour.x, colour.y, colour.z);
	glUniform1f(aLoc, ambientIntensity);

	glUniform3f(dLoc, dir.x, dir.y, dir.z);
	glUniform1f(dILoc, diffuseIntensity);
}

Light::~Light()
{
}
