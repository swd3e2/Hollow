#pragma once

class Layer
{
public:
	virtual void Update(double dt) = 0;
	virtual void PreUpdate(double dt) = 0;
	virtual void PostUpdate(double dt) = 0;
};