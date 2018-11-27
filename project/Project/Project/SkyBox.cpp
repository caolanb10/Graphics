#include "SkyBox.h"



SkyBox::SkyBox()
{
}

SkyBox::SkyBox(std::vector<std::string> faceLocations)
{
	//SkyBox Shader
	skyS = new Shader();
	skyS->CreateFromFiles("Shaders/VertexShaderSky.glsl", "Shaders/FragmentShaderSky.glsl");
	uniformProj = skyS->GetProjectionLocation();
	uniformView = skyS->GetViewLocation();

	//Texture Setup
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	int width, height, depth;

	for (size_t i = 0; i < 6; i++)
	{
		unsigned char* text = stbi_load(faceLocations[i].c_str(), &width, &height, &depth, 0);
		if (!text)
		{
			printf("CANT FIND FILE: %s\n", faceLocations[i].c_str());
			return;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, text);
		stbi_image_free(text);

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Mesh setup
	unsigned int skyBoxIndices[] =
	{
		0,1,2,2,1,3,		//Box Front
		2,3,5,5,3,7,		//Box Right
		5,7,4,4,7,6,		//Box Back
		4,6,0,0,6,1,		//Box Left
		0,5,2,0,4,5,		//Box Top
		1,6,3,3,6,7			//Box Bottom
	};

	float skyBoxVertices[] =
	{
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Front Top Left
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Front Bottom Left
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Front Top Right
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Front Bottom Right

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Back Top Left
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Back Top Right
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,	//	Back Bottom Left
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f	//	Back Bottom Right
	};

	sky = new Mesh();
	sky->CreateMesh(skyBoxVertices, skyBoxIndices, 64, 36);
}

void SkyBox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);
	
	skyS->UseShader();

	glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(projMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	sky->RenderMesh();

	glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{

}
