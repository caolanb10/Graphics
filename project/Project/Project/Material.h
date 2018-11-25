#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat intens, GLfloat shine);

	void useMaterial(GLuint specLocation, GLuint shineLocation);
	~Material();



private:

	GLfloat specularIntensity;
	GLfloat shine;
};

