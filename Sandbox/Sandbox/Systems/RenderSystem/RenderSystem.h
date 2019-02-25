#pragma once
#include "Hollow/ECS/Systems/System.h"
#include <windows.h>
#include <d3d11.h>
#include <directXMath.h>
#include <wrl/client.h>
#include "Hollow/Graphics/HollowRenderer.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/Camera.h"
#include "Hollow/Graphics/BufferTemplate/ConstantBuffer.h"
#include "Hollow/Graphics/Shaders/VertexShader.h"
#include "Hollow/Graphics/Shaders/PixelShader.h"
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include "Sandbox/Entities/GameObject.h"
#include "Hollow/Components/MeshComponent.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/Graphics/ConstBufferMapping.h"
#include "Hollow/Graphics/ShaderResourceMapping.h"
#include "Hollow/Resources/ResourceManager.h"
#include "Hollow/Graphics/Renderer.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Engine.h"
#include "Hollow/Containers/vector.h"

using namespace Hollow;

struct WVP
{
	DirectX::XMMATRIX WVP;
};

struct Transform
{
	DirectX::XMMATRIX transform;
	float id;
	bool selected;
};

class RenderSystem : System<RenderSystem>
{
private:
	void UpdateConstBuffers(PositionComponent * comp, int entityId, bool selected);
	void UpdateWVP();
public:
	RenderSystem(int width, int height);

	virtual void PreUpdate(float_t dt) override;
	virtual void Update(float_t dt);
	virtual void PostUpdate(float_t dt) override;
};