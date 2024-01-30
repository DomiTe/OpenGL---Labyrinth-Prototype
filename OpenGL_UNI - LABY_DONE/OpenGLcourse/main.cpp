#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "GLwindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Values.h"
#include "Model.h"

const float toRadians = 3.14159 / 180.0f;
GLwindow mainWindow;

std::vector<Mesh *> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
Texture brick;
Texture dirt;
Texture plain;
DirectionalLight ambientLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMaterial;
Material dullMaterial;

Model cat;

// Vertex Shader
static const char *vShader = "Shaders/shader.vert";

//Fragement Shader
static const char *fShader = "Shaders/shader.frag";

void calcAverageNormals( unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset )
{
	for( size_t i = 0; i < indiceCount; i += 3 ){
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1( vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2] );
		glm::vec3 v2( vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2] );
		glm::vec3 normal = glm::cross( v1, v2 );
		normal = glm::normalize( normal );

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for( size_t i = 0; i < verticeCount / vLength; i++ ){
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec( vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2] );
		vec = glm::normalize( vec );
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void createObjects()
{
	//Definde sides of the pyramid
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	// Define vertices for a triangle
	GLfloat vertices[] = {
		//	x		y	  z			u	  v			nx	  ny	nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-20.0f, 0.0f, -20.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		20.0f, 0.0f, -20.0f,	20.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-20.0f, 0.0f, 20.0f,	0.0f, 20.0f,	0.0f, -1.0f, 0.0f,
		20.0f, 0.0f, 20.0f,		20.0f, 20.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int indicesRec[] = {
	0, 1, 2,  // Triangle 1
	2, 3, 0   // Triangle 2
	};

	// Vertices for a rectangle
	GLfloat verticesRec[] = {
		//  x      y      z      u    v     nx    ny   nz
	   -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f, // Bottom-left
		1.0f, -1.0f,  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, // Bottom-right
		1.0f,  2.0f,  0.0f,  1.0f, 1.0f,  1.0f, 0.0f, 1.0f, // Top-right
	   -1.0f,  2.0f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f  // Top-left
	};

	calcAverageNormals( indices, 12, vertices, 32, 8, 5 );
	//pyramids
	Mesh *obj1 = new Mesh();
	obj1->createMesh( vertices, indices, 32, 12 );
	meshList.push_back( obj1 );

	Mesh *obj2 = new Mesh();
	obj2->createMesh( vertices, indices, 32, 12 );
	meshList.push_back( obj2 );
	//floor
	Mesh *obj3 = new Mesh();
	obj3->createMesh( floorVertices, floorIndices, 32, 6 );
	meshList.push_back( obj3 );
	//outer walls
	Mesh *obj4 = new Mesh();
	obj4->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj4 );

	Mesh *obj5 = new Mesh();
	obj5->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj5 );

	Mesh *obj6 = new Mesh();
	obj6->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj6 );

	Mesh *obj7 = new Mesh();
	obj7->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj7 );

	/*
	* inner walls
	*/

	Mesh *obj8 = new Mesh();
	obj8->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj8 );

	Mesh *obj9 = new Mesh();
	obj9->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj9 );

	Mesh *obj10 = new Mesh();
	obj10->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj10 );

	Mesh *obj11 = new Mesh();
	obj11->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj11 );

	Mesh *obj12 = new Mesh();
	obj12->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj12 );

	Mesh *obj13 = new Mesh();
	obj13->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj13 );

	Mesh *obj14 = new Mesh();
	obj14->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj14 );

	/*
	*  second inner circle
	*/

	Mesh *obj15 = new Mesh();
	obj15->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj15 );

	Mesh *obj16 = new Mesh();
	obj16->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj16 );

	Mesh *obj17 = new Mesh();
	obj17->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj17 );

	Mesh *obj18 = new Mesh();
	obj18->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj18 );

	Mesh *obj19 = new Mesh();
	obj19->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj19 );

	Mesh *obj20 = new Mesh();
	obj20->createMesh( verticesRec, indicesRec, 32, 6 );
	meshList.push_back( obj20 );
}

void createShader()
{
	Shader *shader1 = new Shader();
	shader1->createFromFiles( vShader, fShader );
	shaderList.push_back( *shader1 );
}

int main()
{
	mainWindow = GLwindow( 1366, 768 );
	mainWindow.initialise();
	// Create the triangle and compile the shaders
	createObjects();
	createShader();

	camera = Camera( glm::vec3( 0.0f, -0.5f, 18.0f ), glm::vec3( 0.0f, 0.5f, 0.0f ), -90.0f, 0.0f, 5.0f, 0.3f );

	brick = Texture( ( char * ) ( "Textures/brick.png" ) );
	brick.loadTextureA();

	dirt = Texture( ( char * ) ( "Textures/dirt.png" ) );
	dirt.loadTextureA();

	plain = Texture( ( char * ) ( "Textures/plain.png" ) );
	plain.loadTextureA();

	shinyMaterial = Material( 4.0f, 256 );
	dullMaterial = Material( 0.3f, 4 );

	cat = Model();
	cat.loadModel( "Models/12221_Cat_v1_l3.obj" );

	ambientLight = DirectionalLight( 1.0f, 1.0f, 1.0f,
		0.1f, 0.1f,
		0.0f, 0.0f, -1.0f );

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight( 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		2.0f, 0.0f, 1.0f,
		0.3f, 0.2f, 0.1f );
	//pointLightCount++;

	pointLights[1] = PointLight( 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,
		-2.0f, 0.0f, -1.0f,
		0.2f, 0.1f, 0.1f );
	//pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight( 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		20.0f );
	spotLightCount++;

	spotLights[1] = SpotLight( 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, 3.0f,
		0.0f, -1.0f, -2.0f,
		0.3f, 0.2f, 0.1f,
		30.0f );
	spotLightCount++;

	spotLights[2] = SpotLight( 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, -10.0f,
		0.0f, -1.0f, 2.0f,
		0.3f, 0.2f, 0.1f,
		30.0f );
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePos = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective( 45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f );

	// Main rendering loop
	while( !mainWindow.getShouldClose() ){
		// Get and handle user input events

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.keyControl( mainWindow.getsKeys(), deltaTime );
		camera.mouseControl( mainWindow.getXChange(), mainWindow.getYChange() );

		// Clear the color buffer
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Use the shader program
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformEyePos = shaderList[0].getEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
		uniformShininess = shaderList[0].getShininessLocation();
		/*
		*	Cone effect for light on camera
		*/
		glm::vec3 lowerLight = camera.getCameraPos();
		lowerLight.y -= 0.5f;
		spotLights[0].setFlash( lowerLight, camera.getCameraDirection() );
		/*
		*
		*/
		shaderList[0].setDirectionalLight( &ambientLight );
		shaderList[0].setPointLights( pointLights, pointLightCount );
		shaderList[0].setSpotLights( spotLights, spotLightCount );

		/*ambientLight.useLight(uniformAmbientIntensity, uniformAmbientColour,
			uniformDiffuseIntensity, uniformDirection);*/

		glUniformMatrix4fv( uniformProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
		glUniformMatrix4fv( uniformView, 1, GL_FALSE, glm::value_ptr( camera.calculateViewMatrix() ) );
		glUniform3f( uniformEyePos, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z );

		/*
		*  2 Pyramids
		*/
		glm::mat4 model( 1.0f );
		model = glm::translate( model, glm::vec3( -5.0f, 0.5f, -3.5f ) );
		//model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale( model, glm::vec3( 2.5f, 2.5f, 2.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		// Bind the Vertex Array Object (VAO)
		meshList[0]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 5.0f, 0.5f, -3.5f ) );
		model = glm::scale( model, glm::vec3( 2.5f, 2.5f, 2.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		shinyMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[1]->renderMesh();

		//floor
		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -2.0f, 0.0f ) );
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		dirt.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[2]->renderMesh();

		/*
		* outer wall
		*/

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -1.0f, -20.0f ) );
		model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 20.0f, 1.0f, 20.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[3]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -1.0f, 20.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 20.0f, 1.0f, 20.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[4]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 20.0f, -1.0f, 0.0f ) );
		model = glm::rotate( model, 90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 20.0f, 1.0f, 20.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[5]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -20.0f, -1.0f, 0.0f ) );
		model = glm::rotate( model, -90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 20.0f, 1.0f, 20.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[6]->renderMesh();

		/*
		* first inner circle
		*/

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -1.0f, -15.0f ) );
		model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 15.0f, 1.0f, 15.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[7]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 6.0f, -1.0f, 15.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 9.0f, 1.0f, 9.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[8]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 15.0f, -1.0f, 0.0f ) );
		model = glm::rotate( model, 90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 15.0f, 1.0f, 15.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[9]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -15.0f, -1.0f, 0.0f ) );
		model = glm::rotate( model, -90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 15.0f, 1.0f, 15.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[10]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -3.0f, -1.0f, 17.5f ) );
		model = glm::rotate( model, -90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 2.5f, 1.0f, 2.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[11]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -7.0f, -1.0f, 12.5f ) );
		model = glm::rotate( model, -90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 2.5f, 1.0f, 2.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[12]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -11.0f, -1.0f, 15.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 4.0f, 1.0f, 4.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[13]->renderMesh();

		/*
		*  second inner circle
		*/

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 1.5f, -1.0f, 10.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 8.5f, 1.0f, 8.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[13]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -1.0f, -10.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 10.0f, 1.0f, 10.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[13]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 10.0f, -1.0f, 0.0f ) );
		model = glm::rotate( model, 90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 10.0f, 1.0f, 10.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[13]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -10.0f, -1.0f, -2.5f ) );
		model = glm::rotate( model, -90.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 7.5f, 1.0f, 7.5f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[13]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( -6.0f, -1.0f, 5.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 4.0f, 1.0f, 4.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[14]->renderMesh();

		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 6.0f, -1.0f, 5.0f ) );
		//model = glm::rotate( model, 180.0f * toRadians, glm::vec3( 0.0f, 1.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 4.0f, 1.0f, 4.0f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		brick.useTexture();
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		meshList[15]->renderMesh();

		//cat
		model = glm::mat4( 1.0f );
		model = glm::translate( model, glm::vec3( 0.0f, -2.0f, -3.0f ) );
		model = glm::rotate( model, 90.0f * toRadians, glm::vec3( -1.0f, 0.0f, 0.0f ) );
		model = glm::scale( model, glm::vec3( 0.1f, 0.1f, 0.1f ) );
		glUniformMatrix4fv( uniformModel, 1, GL_FALSE, glm::value_ptr( model ) );
		dullMaterial.useMaterial( uniformSpecularIntensity, uniformShininess );
		cat.RenderModel();

		// Unuse the shader program
		glUseProgram( 0 );

		//swap the front and back buffers
		mainWindow.swapBuffers();
	}

	// Cleanup and terminate GLFW
	glfwTerminate();

	return 0;
}