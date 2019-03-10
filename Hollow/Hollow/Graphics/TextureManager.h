#pragma once
#include "DirectX/D3DTexture.h"
#include <unordered_map>
#include "Hollow/Common/Log.h"

class HOLLOW_API TextureManager
{
private:
	std::unordered_map<std::string, D3DTexture*> textureList;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	static TextureManager* _instance;
public:
	TextureManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext) :
		device(device), deviceContext(deviceContext)
	{
		if (_instance == nullptr) {
			_instance = this;
		}
	}

	static TextureManager* instance() { return _instance;  }

	D3DTexture* CreateTexture(std::string filename);
};