#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Camera
{
public:	
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat pitch1, GLfloat speed, GLfloat speed1);
	
	void keyControl(bool * keys, GLfloat deltaT);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();

};

