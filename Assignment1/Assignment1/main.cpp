// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <algorithm>
#include "shader_s.h"

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"

#undef min
#undef max


/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here

#define MESH_NAME "C:/Users/Caolan/Desktop/Computer Graphics/Assignment1/monkeyhead_smooth.dae"
#define HAND "C:/Users/Caolan/Desktop/Computer Graphics/Assignment1/Rigged Hand_dae.dae"
#define HOUSE "C:/Users/Caolan/Desktop/Computer Graphics/Assignment1/Medieval_House.obj"
#define CYL "C:/Users/Caolan/Desktop/Computer Graphics/Assignment1/cyl.dae"


/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;
GLuint shaderProgramID;

ModelData mesh_data;
GLuint mesh_vao1 = 0;
GLuint mesh_vao2 = 0;

int width = 800;
int height = 600;

GLuint loc1, loc2, loc3;
GLfloat rotate_y = 0.0f;
GLfloat translate_x = 0.0f;
GLfloat monkey_x = 0.0f;
GLfloat monkey2_x = 0.0f;

#pragma region MESH LOADING		
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name, 
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	); 

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING


// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh(const char* mesh, GLuint* vao) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	mesh_data = load_mesh(mesh);
	unsigned int vp_vbo = 0;

	
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);
	
	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	//unsigned int vao = 0;
	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
}
#pragma endregion VBO_FUNCTIONS


void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);


	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");
	
	// Root of scene
	mat4 view = identity_mat4();
	mat4 persp_proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	vec3 translation = vec3(translate_x, 0.0f, -5.0f);
	view = translate(view, translation);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view.m);


	// Monkey Head 1
	glBindVertexArray(mesh_vao2);
	mat4 monkey = identity_mat4();
	monkey = translate(monkey, vec3((-3.0f + monkey_x), 0.0f, 0.0f));
	monkey = view * monkey;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, monkey.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// Set up cylinder 1
	glBindVertexArray(mesh_vao1);
	mat4 modelChild = identity_mat4();
	modelChild = rotate_y_deg(modelChild, rotate_y);
	modelChild = scale(modelChild, vec3(0.5f, 0.5f, 1.0f));
	modelChild = translate(modelChild, vec3(-0.3f, 1.6f, 0.0f));

	//Set up cylinder 2
	mat4 modelChild2 = identity_mat4();
	modelChild2 = rotate_y_deg(modelChild2, rotate_y);
	modelChild2 = scale(modelChild2, vec3(0.5f, 0.5f, 1.0f));
	modelChild2 = translate(modelChild2, vec3(0.3f, 1.6f, 0.0f));

	// Apply the root matrix to the child matrices
	modelChild = monkey * modelChild;
	modelChild2 = monkey * modelChild2;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, modelChild2.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);


	// Monkey head 2 
	glBindVertexArray(mesh_vao2);
	mat4 monkey2 = identity_mat4();
	monkey2 = translate(monkey2, vec3((3.0f + monkey2_x), 0.0f, 0.0f));
	monkey2 = view * monkey2;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, monkey2.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);


	// Cylinder for monkey head 2
	glBindVertexArray(mesh_vao1);
	mat4 cyl3 = identity_mat4();
	cyl3 = scale(cyl3, vec3(0.5, 0.5f, 1.0f));
	cyl3 = translate(cyl3, vec3(-0.3f, 1.6f, 0.0f));
	cyl3 = monkey2 * cyl3;
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, cyl3.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);
	glutSwapBuffers();
}


void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;

	// Rotate the model slowly around the y axis at 20 degrees per second
	rotate_y += 20.0f * delta;
	rotate_y = fmodf(rotate_y, 360.0f);

	// Draw the next frame
	glutPostRedisplay();
}


int init()
{
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Set up the shaders
		Shader myShader("simpleFragmentShader.txt", "simpleVertexShader.txt");
	myShader.use();

	// load mesh into a vertex buffer array
	generateObjectBufferMesh(CYL, &mesh_vao1);
	generateObjectBufferMesh(MESH_NAME, &mesh_vao2);

	return 0;
}

//Keyboard controls
void keypress(unsigned char key, int x, int y) {
	// x and y move "scene"
	if (key == 'x') {
		translate_x += 0.5f;
	}
	if (key == 'y') {
		translate_x -= 0.5f;
	}

	// m and n move "monkey"
	if (key == 'm') {
		monkey_x += 0.5f;
	}
	if (key == 'n') {
		monkey_x -= 0.5f;
	}

	// t and u move "monkey2"
	if (key == 't') {
		monkey2_x += 0.5f;
	}
	if (key == 'u') {
		monkey2_x -= 0.5f;
	}
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("OpenGLProject");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);

	// Set up your objects and shaders
	int err = init();
	if (err)
		std::cout << "error";
		return -1;

	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
