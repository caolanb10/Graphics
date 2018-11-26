#include "Light.h"

Light::Light() 
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat a, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = a;
	diffuseIntensity = dIntensity;
}

Light::~Light()
{
}
