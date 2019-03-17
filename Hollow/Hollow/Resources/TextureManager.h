#pragma once
#include "Hollow/Graphics/DirectX/D3DTexture.h"
#include "Hollow/Platform.h"
#include <unordered_map>
#include "Hollow/Common/Log.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"

struct TextureData
{
	~TextureData()
	{
		if (data != nullptr) {
			free(data);
		}
	}
	int width;
	int height;
	int pitch;
	void* data;
};

class HOLLOW_API TextureManager : public CModule<TextureManager>
{
private:
	std::unordered_map<std::string, D3DTexture*> textureList;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	static TextureManager* _instance;
	std::string baseTexturePapth = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
public:
	void startUp(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		this->device = device;
		this->deviceContext = deviceContext;
		setStartedUp();
	}

	void shutdown()
	{
		setShutdown();
	}

	D3DTexture* CreateTexture(std::string filename, bool loadFromDefaultDir = true);

	std::unordered_map<std::string, D3DTexture*>* getTexuresList();
private:
	TextureData* loadTexture(std::string filaname);
};