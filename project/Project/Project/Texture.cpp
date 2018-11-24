#include "Texture.h"

Texture::Texture()
{
	textID = 0;
	width = 0;
	height = 0;
	depth = 0;
	this->file = (char *) "";
}
Texture::Texture(char * file)
{
	textID = 0;
	width = 0;
	height = 0;
	depth = 0;
	this->file = file;
}

void Texture::load()
{
	unsigned char* text = stbi_load(file, &width, &height, &depth, 0);
	if (!text)
		printf("CANT FIND FILE: %s\n", this->file);

	glGenTextures(1, &textID);
	glBindTexture(GL_TEXTURE_2D, textID);
	
	//Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, text);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(text);
}

void Texture::clear()
{
	glDeleteTextures(1, &textID);
	textID = 0;
	width = 0;
	height = 0;
	depth = 0;
	file = (char *) "";

}

void Texture::use()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textID);

}

Texture::~Texture()
{

}
