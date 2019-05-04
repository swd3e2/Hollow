#pragma once

class Layer
{
public:
	virtual void Update(float dt) = 0;
	virtual void PreUpdate(float dt) = 0;
	virtual void PostUpdate(float dt) = 0;
};