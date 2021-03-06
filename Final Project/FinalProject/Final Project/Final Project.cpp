#include <iostream>
#include <windows.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <filesystem>
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <filesystem> // Microsoft-specific implementation header file name 

//Header Files
#include "maths_funcs.h"
#include "shader_s.h"
#include "graphicObject.h"
#include "ShaderAndObjects.h"
 
using namespace std::experimental::filesystem::v1;

int width = 800;
int height = 600;

Shader * mainS;
GLuint shaderProgramID;
GraphicObject * monkey;
GraphicObject * cylinder;

void updateScene()
{
	glutPostRedisplay();
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mainS->use();

	int matrix_location = glGetUniformLocation(mainS->ID, "model");
	int view_mat_location = glGetUniformLocation(mainS->ID, "view");
	int proj_mat_location = glGetUniformLocation(mainS->ID, "proj");

	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	vec3 translation = vec3(0.0f, 0.0f, -5.0f);
	view = translate(view, translation);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);

	monkey->makeActive();
	monkey->attach(view);

	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, monkey->position.m);
	glDrawArrays(GL_TRIANGLES, 0, monkey->mesh_data.mPointCount);
	glutSwapBuffers();
}

void keypress(unsigned char key, int x, int y)
{

}

void initGlew()
{
	int err = 0;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		err = 1;
	}
	if (err == 1)
		std::cout << "error";
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("OpenGL Project");

	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	initGlew();

	mainS = initialiseShaders(vs, fs, mainS);
	mainS->use();
	shaderProgramID = mainS->ID;
	std::cout << monk;
	std::cout << cyl;

	GraphicObject new1(monk, mainS->ID);
	GraphicObject new2(cyl, mainS->ID);

	monkey = &new1;
	cylinder = &new2;
	
	glutMainLoop();
	return 0;
}
