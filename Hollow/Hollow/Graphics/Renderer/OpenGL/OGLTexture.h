#pragma once

#ifndef HW_OGL_TEXTURE_H
#define HW_OGL_TEXTURE_H

#include "Hollow/Graphics/Renderer/Base/Texture.h"

class OGLTexture : public Texture
{
public:
	unsigned int textureId;
public:
	OGLTexture(int width, int height) : Texture(width, height), textureId(0) {}
};

#endif