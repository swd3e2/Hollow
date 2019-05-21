#pragma once
#include "Layer.h"
#include <vector>
#include "Hollow/Platform.h"

class  LayerStack
{
private:
	std::vector<Layer*> layers;
public:
	void AddLayer(Layer* layer)
	{
		layers.push_back(layer);
	}

	void Update(float dt)
	{
		for (auto& it : layers)
		{
			it->Update(dt);
		}
	}

	void PreUpdate(float dt)
	{
		for (auto& it : layers)
		{
			it->PreUpdate(dt);
		}
	}

	void PostUpdate(float dt)
	{
		for (auto& it : layers)
		{
			it->PostUpdate(dt);
		}
	}
};