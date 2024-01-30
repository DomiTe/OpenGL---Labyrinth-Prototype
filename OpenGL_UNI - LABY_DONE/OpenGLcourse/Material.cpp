#include "Material.h"

Material::Material()
{
	specularIntensity = 0.0f;
	shiny = 0.0f;
}

Material::Material( GLfloat sIntensity, GLfloat shine )
{
	specularIntensity = sIntensity;
	shiny = shine;
}

void Material::useMaterial( GLuint specularIntensityLoc, GLuint shininessLoc )
{
	glUniform1f( specularIntensityLoc, specularIntensity );
	glUniform1f( shininessLoc, shiny );
}

Material::~Material()
{
}