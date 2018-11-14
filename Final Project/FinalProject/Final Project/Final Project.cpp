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

#include "maths_funcs.h"
#include "shader_s.h"
//#include "mesh.h"
#include <filesystem> // Microsoft-specific implementation header file name  
using namespace std::experimental::filesystem::v1;


#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
}

ModelData;

#pragma endregion SimpleTypes
int width = 800;
int height = 600;

//Getting directories for shaders
std::string shaders = "C:/Users/Caolan/Desktop/Computer Graphics/Final Project/FinalProject/Shaders";

std::string vs1 = shaders + "/VertexShader.vs";
std::string fs1 = shaders + "/FragmentShader.fs";

const char * vs = vs1.c_str();
const char * fs = fs1.c_str();

Shader * mainS;
GLuint shaderProgramID;
ModelData mesh_data;

void updateScene()
{
	glutPostRedisplay();

}

void initialiseShaders(const char* vs, const char* fs, Shader* s)
{
	Shader myShader(vs, fs);
	s = &myShader;
}

void display()
{

}

void keypress(unsigned char key, int x, int y)
{

}

void createObject(const char* mesh, GLuint* vao)
{
	//mesh_data = load_mesh(mesh);
	GLuint loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	GLuint loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	GLuint loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

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

GLfloat vertices[] = { -1.0f, -1.0f, 0.0f,
					   1.0f, -1.0f, 0.0f,
					   0.0f, 1.0f, 0.0f };

int main(int argc, char** argv)
{

	path pat = current_path();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("OpenGL Project");

	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	initGlew();
	initialiseShaders(vs, fs, mainS);
	mainS->use;
	glutMainLoop();
	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
