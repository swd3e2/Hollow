#pragma once
#include "Hollow/Graphics/DirectX/D3DTexture.h"
#include "Hollow/Platform.h"
#include <unordered_map>
#include "Hollow/Common/Log.h"
#include "FreeImage.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Graphics/DirectX/D3DContext.h"

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
	D3DContext* context;
	static TextureManager* _instance;
	std::string baseTexturePapth = "C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Textures/";
public:
	void startUp(D3DContext& context)
	{
		this->context = &context;
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