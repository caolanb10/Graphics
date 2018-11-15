#ifndef GRAPHIC_OBJECT
#define GRAPHIC_OBJECT

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

//Headers
#include "maths_funcs.h"
#include "shader_s.h"

using namespace std;
#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;

#pragma endregion SimpleTypes
ModelData load_mesh(const char* file_name);
void initialiseShaders(const char* vs, const char* fs, Shader* s);

class GraphicObject
{
public:
	GLuint loc1, loc2, loc3;
	const char * mesh;
	GLuint* vao;
	GLuint shaderProgramID;
	int matrix_location;
	int view_mat_location;
	int proj_mat_location;
	Shader * thisShader;
	ModelData mesh_data;
	
	GraphicObject(const char* m, GLuint* v, GLuint s, Shader* ) 
	{
		mesh = m;
		vao = v;
		shaderProgramID = s;
		createObject();
		//matrix_location = glGetUniformLocation(shaderProgramID, "model");
		//view_mat_location = glGetUniformLocation(shaderProgramID, "view");
		//proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
		
	}

	void createObject()
	{
		//mesh_data = load_mesh(mesh);
		//GLuint loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
		//GLuint loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
		//GLuint loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");
		mesh_data = load_mesh(mesh);

		unsigned int vp_VBO;
		glGenVertexArrays(1, vao);
		glGenBuffers(1, &vp_VBO);
		glBindVertexArray(*vao);
		glBindBuffer(GL_ARRAY_BUFFER, vp_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

		unsigned int vn_VBO = 0;
		glGenBuffers(1, &vn_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, vn_VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

		//Enable attribute pointer
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_VBO);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vp_VBO);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//Unbind
		glBindVertexArray(0);
	}


};


#endif