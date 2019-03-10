#pragma once
#include "Layer.h"
#include <vector>
#include "Hollow/Platform.h"

class HOLLOW_API LayerStack
{
private:
	std::vector<Layer*> layers;
public:
	void AddLayer(Layer* layer)
	{
		layers.push_back(layer);
	}

	void Update()
	{
		for (auto& it : layers)
		{
			it->Update();
		}
	}

	void PreUpdate()
	{
		for (auto& it : layers)
		{
			it->PreUpdate();
		}
	}

	void PostUpdate()
	{
		for (auto& it : layers)
		{
			it->PostUpdate();
		}
	}
};