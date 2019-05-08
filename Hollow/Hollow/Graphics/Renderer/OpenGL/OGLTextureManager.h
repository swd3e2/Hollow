#pragma once

#ifndef HW_OGL_TEXTURE_MANAGER_H
#define HW_OGL_TEXTURE_MANAGER_H

#include "OGLTexture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"

class HOLLOW_API OGLTextureManager : public TextureManager
{
public:
	virtual Texture* CreateTextureInternal(TEXTURE_DESC*) override;
private:
};

#endif