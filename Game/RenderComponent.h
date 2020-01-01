#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Graphics/HardwareBuffer.h>
#include <Hollow/Graphics/Texture.h>

class RenderComponent : public Hollow::Component<RenderComponent>
{
public:
	Hollow::s_ptr<Hollow::VertexBuffer> vertexBuffer;
	Hollow::s_ptr<Hollow::IndexBuffer> indexBuffer;
	Hollow::s_ptr<Hollow::Texture> texture;
};