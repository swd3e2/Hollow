#pragma once

#include "LinearMath/btIDebugDraw.h"
#include "Hollow/Graphics/RenderApi.h"
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Common/Log.h"

class PhysicsDebugDraw : public btIDebugDraw
{
private:
	Hollow::s_ptr<Hollow::VertexBuffer> lineVB;
	Hollow::RenderApi* renderer;
public:
	PhysicsDebugDraw(Hollow::RenderApi* renderer) :
		renderer(renderer)
	{
		std::vector<Hollow::Vertex> vertices;
		vertices.push_back(Hollow::Vertex(0.0f, 10.0f, 0.0f, 0.0f, 0.0f));
		vertices.push_back(Hollow::Vertex(0.0f, -10.0f, 0.0f, 0.0f, 1.0f));

		Hollow::VERTEX_BUFFER_DESC desc;
		desc.data = vertices.data();
		desc.size = vertices.size();
		desc.stride = sizeof(Hollow::Vertex);
		desc.isDynamic = true;

		lineVB = Hollow::VertexBuffer::create(desc);
	}

	virtual ~PhysicsDebugDraw() {};

	///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
	virtual void setDefaultColors(const DefaultColors& /*colors*/) {}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Hollow::Vertex vertices[2] = {
			Hollow::Vertex(from.getX(), from.getY(), from.getZ()),
			Hollow::Vertex(to.getX(),	to.getY(),	 to.getZ())
		};
		//HW_INFO("FROM {} {} {} TO {} {} {}", from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ());
		lineVB->update(vertices, sizeof(Hollow::Vertex) * 2);
		renderer->setVertexBuffer(lineVB);
		renderer->draw(2);
	}

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {}

	virtual void reportErrorWarning(const char* warningString) override {}

	virtual void draw3dText(const btVector3& location, const char* textString) override {}

	virtual void setDebugMode(int debugMode) override {}

	virtual int getDebugMode() const override { return 2; }
};