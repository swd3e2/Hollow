#pragma once

#ifndef HW_OGL_TEXTURE_MANAGER_H
#define HW_OGL_TEXTURE_MANAGER_H

#include "OGLTexture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"

class OGLTextureManager : public TextureManager
{
public:
	virtual Texture* Create2dTexture(TEXTURE_DESC* desc) override { return nullptr; }
	virtual Texture* Create3dTexture(TEXTURE_DESC** desc) override { return nullptr; return nullptr;}
private:
};

#endif