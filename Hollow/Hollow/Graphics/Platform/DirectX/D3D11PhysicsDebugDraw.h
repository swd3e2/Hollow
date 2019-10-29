#pragma once

#include "LinearMath/btIDebugDraw.h"

class D3D11PhysicsDebugDraw : public btIDebugDraw
{
public:
	D3D11PhysicsDebugDraw() {}
	virtual ~D3D11PhysicsDebugDraw() {};

	///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
	virtual void setDefaultColors(const DefaultColors& /*colors*/) {}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{

	}

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) = 0;

	virtual void reportErrorWarning(const char* warningString) = 0;

	virtual void draw3dText(const btVector3& location, const char* textString) = 0;

	virtual void setDebugMode(int debugMode) = 0;

	virtual int getDebugMode() { return 3; }
};