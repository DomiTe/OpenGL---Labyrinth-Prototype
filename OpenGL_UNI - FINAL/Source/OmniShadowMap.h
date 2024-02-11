#pragma once
#include "ShadowMap.h"
class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap();

	bool init( unsigned int width, unsigned int height );

	void write();

	void read( GLenum textureUnit );

	~OmniShadowMap();

private:
};
