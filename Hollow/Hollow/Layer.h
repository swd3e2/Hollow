#pragma once

class Layer
{
public:
	virtual void Update() = 0;
	virtual void PreUpdate() = 0;
	virtual void PostUpdate() = 0;
};