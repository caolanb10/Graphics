#pragma once

#include <GL/glew.h>
#include "Common.h"

class Texture
{
public:
	Texture();
	Texture(char* file);
	~Texture();

	bool load();
	bool loadA();

	void use();
	void clear();

private:
	GLuint textID;
	int width, height, depth;
	char* file;

};

