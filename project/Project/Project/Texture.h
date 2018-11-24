#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* file);
	~Texture();

	void load();
	void use();
	void clear();

private:
	GLuint textID;
	int width, height, depth;
	char* file;

};

