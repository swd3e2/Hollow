#pragma once
#include "DirectX/D3DTexture.h"
#include <unordered_map>
#include "Hollow/Common/Log.h"

class TextureManager
{
private:
	std::unordered_map<std::string, D3DTexture*> textureList;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
public:
	TextureManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
		device(device), deviceContext(deviceContext)
	{}

	D3DTexture* CreateTexture(std::string filename);
};