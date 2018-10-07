
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <shader_s.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


//Global variable for VAO
GLuint VAObject = 0;
GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.0f, 0.5f, 0.0f };
GLuint ShaderID;
GLuint transformLocation;
glm::mat4 translation();

using namespace std;

int init()
{
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
}

GLuint vertexArrayObject(GLuint* VAO, GLuint* VBO, GLfloat vertices[], GLuint numVertices)
{

	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	//Bind vertex array object first
	glBindVertexArray(*VAO);
	
	//Binds current buffer with VBO
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);

	//Provides data for buffer
	glBufferData(GL_ARRAY_BUFFER, numVertices * 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	//Creates vertex attribute pointer	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*) 0);
	
	//Enables it
	glEnableVertexAttribArray(0);

	//Reset buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//Resets bound vertex arrray object
	glBindVertexArray(0);

	return *VAO;
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	// NB: Make the call to draw the geometry in the currently activated vertex buffer.
	//This is where the GPU starts to work!
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

void createObject(GLuint* VAObject, GLuint* VAO, GLuint* VBO)
{
	GLuint numVertices = sizeof(vertices) / (sizeof(vertices[0]) * 4);
	*VAObject = vertexArrayObject(VAO, VBO, vertices, numVertices);

}


// Assignment 2
/*
• Keyboard control
• Keypress to show: Rotation around the x- y- and z-axis (~20%)
• Keypress to show: Translation in the x- y- and z- direction (~20%)
• Keypress to show: Uniform and non-uniform Scaling (~20%)
• Keypress to show: Combined Transformations (~20%)
• Multiple triangles in the scene, using the same buffer but creating a new transformation
matrix for each one. (~20%)
*/
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float xRot, yRot, zRot = 0;
float xTrans, yTrans, zTrans = 0;
float xScale, yScale, zScale = 0;

float * axisP = &xRot;

glm::mat4 rotater()
{
	//Identity matrix
	glm::mat4 trans = glm::mat4(1.0f);
	//Translate to origin
	//trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));

	//Scale at origin
	trans = glm::scale(trans, glm::vec3((1.0f + xScale), (1.0f + yScale), (1.0f + zScale)));
	
	//Rotate in x,y,z plane
	trans = glm::rotate(trans, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, zRot, glm::vec3(0.0f, 0.0f, 1.0f));

	//Translate back to position
	trans = glm::translate(trans, glm::vec3(xTrans, yTrans, zTrans));
	
	return trans;
}

glm::mat4 translation()
{

	std::cout << "translation";
	//Identity matrix
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::rotate(trans, xRot, glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, yRot, glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, zRot, glm::vec3(0.0f, 0.0f, 1.0f));

	//Translate to position
	trans = glm::translate(trans, glm::vec3(xTrans, yTrans, zTrans));
	return trans;
}

void keyboard(unsigned char key, int x, int y)
{
	glm::mat4 trans;
	switch (key)
	{
	case('1'):
		xTrans -= 0.05; trans = translation(); break;
	case('2'):
		yTrans -= 0.05; trans = translation(); break;
	case('3'):
		zTrans -= 0.05; trans = translation(); break;
	case('x'):
		xTrans += 0.05; trans = translation(); break;
	case('y'):
		yTrans += 0.05; trans = translation(); break;
	case('z'):
		zTrans += 0.05; trans = translation(); break;
	case('t'):
		xRot += 0.1; trans = rotater();  break;
	case('u'):
		yRot += 0.1; trans = rotater(); break;
	case('v'):
		zRot += 0.1; trans = rotater(); break;
	case('4'):
		xRot -= 0.1; trans = rotater(); break;
	case('5'):
		yRot -= 0.1; trans = rotater(); break;
	case('6'):
		zRot -= 0.1; trans = rotater(); break;
	case('+'):
		xScale += 0.1f;
		yScale += 0.1f;
		zScale += 0.1f; 
		trans = rotater();  break;
	case('-'):
		xScale -= 0.1f;
		yScale -= 0.1f;
		zScale -= 0.1f; 
		trans = rotater(); break;
	case('n'):
		xScale += 0.2f; 
		trans = rotater(); break;
	default:
		break;
	}

	//Pass matrix to shader program
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
}
void draw()
{
	glutPostRedisplay();
}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hello Triangle");

	//glEnable(GL_DEPTH_TEST);
	
	// Tell glut where the display function is
	glutDisplayFunc(display);
	int error = init();
	if (error)
		return 1;

	Shader myShader("C:/Users/Caolan/Desktop/4th Year/Computer Graphics/Assignments/Assignment1/Shaders/vertexShader.vs", 
					"C:/Users/Caolan/Desktop/4th Year/Computer Graphics/Assignments/Assignment1/Shaders/fragmentShader.fs"); 

	GLuint VAO = 0, VBO = 0;
	createObject(&VAObject, &VAO, &VBO);

	//Use object for rendering
	glBindVertexArray(VAObject);
	myShader.use();

	//Get transform matrix location from vertex shader program
	unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
	transformLocation = transformLoc;
	
	/*
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	*/

	//Initialise identity matrix
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	
	//Pass matrix to shader program
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	
	//Register callbackl  for input
	glutKeyboardFunc(keyboard);
	
	//Always redraw
	glutIdleFunc(draw);

	//Event loop
	glutMainLoop();
    return 0;
}