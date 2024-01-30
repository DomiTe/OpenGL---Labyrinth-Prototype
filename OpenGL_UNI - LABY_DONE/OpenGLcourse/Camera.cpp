#include "Camera.h"

Camera::Camera() { }

Camera::Camera( glm::vec3 sPosition, glm::vec3 sUp, GLfloat sYaw, GLfloat sPitch, GLfloat sMoveSpeed, GLfloat sTurnSpeed )
{
	position = sPosition;
	worldUp = sUp;
	yaw = sYaw;
	pitch = sPitch;
	front = glm::vec3( 0.0f, 0.0f, -1.0f );

	moveSpeed = sMoveSpeed;
	turnSpeed = sTurnSpeed;

	update();
}
void Camera::keyControl( bool *keys, GLfloat deltaTime )
{
	GLfloat velocity = moveSpeed * deltaTime;
	if( keys[GLFW_KEY_W] ){
		position += front * velocity;
	}

	if( keys[GLFW_KEY_S] ){
		position -= front * velocity;
	}

	if( keys[GLFW_KEY_A] ){
		position -= right * velocity;
	}

	if( keys[GLFW_KEY_D] ){
		position += right * velocity;
	}
}

void Camera::mouseControl( GLfloat xChange, GLfloat yChange )
{
	xChange *= turnSpeed;
	yChange = 0.0f; // Ignore vertical mouse movement

	yaw += xChange;

	// Optional: Clamp yaw to avoid continuous rotation
	if( yaw > 360.0f || yaw < -360.0f ){
		yaw = 0.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt( position, position + front, up );
}

glm::vec3 Camera::getCameraPos()
{
	return position;
}

glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize( front );
}

void Camera::update()
{
	// Calculate the new front vector
	glm::vec3 newFront;
	newFront.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	newFront.y = sin( glm::radians( pitch ) );
	newFront.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front = glm::normalize( newFront );

	// Recalculate the right and up vectors
	right = glm::normalize( glm::cross( front, worldUp ) );
	up = glm::normalize( glm::cross( right, front ) );
}

Camera::~Camera()
{
}