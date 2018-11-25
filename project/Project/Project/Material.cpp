#include "Material.h"



Material::Material()
{
	specularIntensity = 0;
	shine = 0;
}

Material::Material(GLfloat intens, GLfloat shine)
{
	specularIntensity = intens;
	this->shine = shine;
}
void Material::useMaterial(GLuint specLocation, GLuint shineLocation)
{
	glUniform1f(specLocation, specularIntensity);
	glUniform1f(shineLocation, shine);
}

Material::~Material()
{
}
