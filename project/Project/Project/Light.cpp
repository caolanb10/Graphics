#include "Light.h"

Light::Light() 
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = a;
}

void Light::use(GLfloat aLoc, GLfloat cLoc)
{
	glUniform3f(cLoc, colour.x, colour.y, colour.z);
	glUniform1f(aLoc, ambientIntensity);
}
Light::~Light()
{
}
