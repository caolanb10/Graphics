#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "Light.h"
#include "SkyBox.h"
#include "Common.h"

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

const float toRadians = 3.1415926f / 180.0f;

Window mainWindow;
Camera cam;
DirectionalLight dirLight;
SkyBox sky;

Texture brick;
Texture dirt;

Material shiny;
Material dull;

//Used for movements
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//ArrayLists of meshes and shaders
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex Shader code
static const char* vShader = "Shaders/VertexShader.glsl";

// Fragment Shader
static const char* fShader = "Shaders/FragmentShader.glsl";

static const char* monk = "Objects/monkeyhead_smooth.dae";
 
static const char* ship = "Objects/spaceship.dae";

static const char* wing = "Objects/wing.dae";



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
				modelData.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));
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

//Normal calculations
void calcAverageNormals(unsigned int * indices, unsigned indiceCount,
						GLfloat * vertices, unsigned int verticeCount,
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}

}
void CreateObjects()
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};

	GLfloat vertices[] = {
		//x		y	   z		u		v		nx     ny   nz
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int planeVertices[] =
	{
		0,2,1,
		1,2,3
	};

	GLfloat plane[] = {
		-10.0f, 0.0f, -10.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,		10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,		0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,			10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);
	
	//Terrain
	Mesh * obj3 = new Mesh();
	obj3->CreateMesh(plane, planeVertices, 32, 6);
	meshList.push_back(obj3);

	Mesh * shipMesh = new Mesh();
	ModelData newD = load_mesh(ship);
	shipMesh->CreateModel(newD);
	meshList.push_back(shipMesh);
		
	Mesh * wingMesh = new Mesh();
	ModelData newD1 = load_mesh(wing);
	wingMesh->CreateModel(newD1);
	meshList.push_back(wingMesh);

}

void CreateShaders()
{
	Shader * shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	//Create Window
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	cam = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.5f);
	brick = Texture((char *) "Textures/brick.png");
	dirt = Texture((char *) "Textures/dirt.png");

	brick.loadA();
	dirt.loadA();

	shiny = Material(1.0f, 32.0f);
	dull = Material(0.3f, 4.0f);
	

	dirLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
							0.5f, 0.8f,
							1.0f, -1.0f, 1.0f);

	std::vector<std::string> faces;

	faces.push_back("Textures/SkyBox/oasisnight_rt.tga");	
	faces.push_back("Textures/SkyBox/oasisnight_lf.tga");
	faces.push_back("Textures/SkyBox/oasisnight_up.tga");
	faces.push_back("Textures/SkyBox/oasisnight_dn.tga");
	faces.push_back("Textures/SkyBox/oasisnight_bk.tga");
	faces.push_back("Textures/SkyBox/oasisnight_ft.tga");
	

	sky = SkyBox(faces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	GLuint uniformAmbientIntensity = 0, uniformAmbientColour = 0;
	GLuint uniformDirection = 0, uniformDiffuseIntensity = 0;
	GLuint uniformCameraPosition = 0, uniformSpecularIntensity = 0;
	GLuint uniformShine = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth()/(GLfloat) mainWindow.getBufferHeight(), 0.1f, 100.0f);

	float curAngle = 0;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{		

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sky.DrawSkybox(cam.calculateViewMatrix(), projection);

		GLfloat now = glfwGetTime(); 
		deltaTime = now - lastTime;
		lastTime = now;

		curAngle += deltaTime;

		// Get + Handle user input events
		glfwPollEvents();

		cam.keyControl(mainWindow.getKey(), deltaTime);
		cam.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());



		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformCameraPosition = shaderList[0].GetCameraLocation();
		uniformShine = shaderList[0].GetShineLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();

		dirLight.use(uniformAmbientIntensity, uniformAmbientColour,
				uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(cam.calculateViewMatrix()));
		glUniform3f(uniformCameraPosition, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);


		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brick.use();
		shiny.useMaterial(uniformSpecularIntensity, uniformShine);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.use();
		dull.useMaterial(uniformSpecularIntensity, uniformShine);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -10.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirt.use();
		dull.useMaterial(uniformSpecularIntensity, uniformShine);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dull.useMaterial(uniformSpecularIntensity, uniformShine);
		
		meshList[3]->RenderModel();
		
		glm::mat4 modelChild1 = glm::mat4(1.0f);
		modelChild1 = glm::translate(modelChild1, glm::vec3(-6.0f, 1.0f, -2.5f));
		modelChild1 = glm::rotate(modelChild1 , curAngle * 1000 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelChild1 = model * modelChild1;

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelChild1));

		meshList[4]->RenderModel();


		glm::mat4 modelChild2 = glm::mat4(1.0f);
		modelChild2 = glm::translate(modelChild2, glm::vec3(6.0f, 1.0f, -2.5f));
		modelChild2 = model * modelChild2;
		modelChild2 = glm::rotate(modelChild2, curAngle * 1000 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelChild2));
		
		meshList[4]->RenderModel();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}

	return 0;
}