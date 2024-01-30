#include "GLwindow.h"

GLwindow::GLwindow()
{
	width = 1024;
	height = 800;
	xChange = 0.0f;
	yChange = 0.0f;

	for( size_t i = 0; i < 1024; i++ ){
		keys[i] = 0;
	}
}

GLwindow::GLwindow( GLint windowWidth, GLint windowHeight )
{
	width = windowWidth;
	height = windowHeight;
}

int GLwindow::initialise()
{
	if( !glfwInit() ){
		printf( "GLFW initialisation failed!" );
		glfwTerminate();
		return 1;
	}
	// Set GLFW window hints for OpenGL version and profile
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

	// Core Profil = No Backwards compatibility
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

	// Create a GLFW window
	mainWindow = glfwCreateWindow( width, height, "First Window", NULL, NULL );
	// Check if window creation is successful
	if( !mainWindow ){
		printf( "Window creation failed!" );
		glfwTerminate();
		return 1;
	}

	// Get framebuffer size for setting up OpenGL viewport
	glfwGetFramebufferSize( mainWindow, &bufferWidth, &bufferHeight );

	// Set the created window as the current OpenGL context
	glfwMakeContextCurrent( mainWindow );

	//Handle Key and mouse
	createCallbacks();
	glfwSetInputMode( mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	// Allow modern OpenGL features with GLEW
	glewExperimental = GL_TRUE;
	if( glewInit() != GLEW_OK ){
		printf( "Glew initialisation failed!" );
		glfwDestroyWindow( mainWindow );
		glfwTerminate();
		return 1;
	}

	glEnable( GL_DEPTH_TEST );

	// Setup Viewpoint size
	glViewport( 0, 0, bufferWidth, bufferHeight );

	glfwSetWindowUserPointer( mainWindow, this );
}
void GLwindow::createCallbacks()
{
	glfwSetKeyCallback( mainWindow, handleKeys );
	glfwSetCursorPosCallback( mainWindow, handleMouse );
}
GLfloat GLwindow::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}
GLfloat GLwindow::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void GLwindow::handleKeys( GLFWwindow *window, int key, int code, int action, int mode )
{
	GLwindow *theWindow = static_cast< GLwindow * >( glfwGetWindowUserPointer( window ) );

	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ){
		glfwSetWindowShouldClose( window, GL_TRUE );
	}
	if( key >= 0 && key < 1024 ){
		if( action == GLFW_PRESS ){
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if( action == GLFW_RELEASE ){
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void GLwindow::handleMouse( GLFWwindow *window, double xPos, double yPos )
{
	GLwindow *theWindow = static_cast< GLwindow * >( glfwGetWindowUserPointer( window ) );

	if( theWindow->mouseMoved ){
		theWindow->lastX = 0.0f;
		theWindow->lastY = 0.0f;
		theWindow->mouseMoved = false;
	}
	else{
		theWindow->xChange = xPos - theWindow->lastX;
		theWindow->yChange = theWindow->lastY - yPos;

		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}
}

GLwindow::~GLwindow()
{
}