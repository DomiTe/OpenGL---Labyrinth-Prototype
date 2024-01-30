#pragma once

#include<GL/glew.h>

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera( glm::vec3 sPosition, glm::vec3 sUp, GLfloat sYaw, GLfloat sPitch, GLfloat sMoveSpeed, GLfloat sTurnSpeed );

	void keyControl( bool *keys, GLfloat deltaTime );

	void mouseControl( GLfloat xChange, GLfloat yChange );

	glm::vec3 getCameraPos();
	glm::vec3 getCameraDirection();

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

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};
