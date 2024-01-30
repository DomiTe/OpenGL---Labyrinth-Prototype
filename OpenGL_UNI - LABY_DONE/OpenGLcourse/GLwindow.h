#pragma once

#include <stdio.h>

#include <Gl/glew.h>
#include <GLFW/glfw3.h>

class GLwindow
{
public:
	GLwindow();
	GLwindow( GLint windowWidth, GLint windowHeight );

	int initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose( mainWindow ); }

	bool *getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers( mainWindow ); }

	~GLwindow();
private:
	GLFWwindow *mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseMoved;

	static void handleMouse( GLFWwindow *window, double xPos, double yPos );
	void createCallbacks();
	static void handleKeys( GLFWwindow *window, int key, int code, int action, int mode );
};
