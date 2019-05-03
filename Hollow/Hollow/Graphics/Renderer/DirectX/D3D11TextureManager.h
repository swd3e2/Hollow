#pragma once

#ifndef HW_D3D11_TEXTURE_MANAGER_H
#define HW_D3D11_TEXTURE_MANAGER_H

#include "D3D11Texture.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/TextureManager.h"

class HOLLOW_API D3D11TextureManager : public TextureManager
{
public:
	virtual Texture* CreateTextureInternal(TEXTURE_DESC*) override;
private:
};

#endif