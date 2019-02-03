#include "RenderSystem.h"

RenderSystem::RenderSystem(HWND * hwnd, int width, int height)
	: m_Renderer(new Hollow::Core::Graphics::HollowDirectXRenderer(hwnd, width, height))
{
	Hollow::Log::GetCoreLogger()->debug("RenderSystem: Initialized.");

	this->camera = new Camera();
	this->camera->SetProjectionValues(85.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	this->camera->SetPosition(3.0f, 2.0f, -5.0f);

	transformConstantBuffer.Init(this->m_Renderer->GetDevice(), this->m_Renderer->GetDeviceContext());
	WVPConstantBuffer.Init(this->m_Renderer->GetDevice(), this->m_Renderer->GetDeviceContext());

	D3D11_INPUT_ELEMENT_DESC bxlayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(bxlayout);

	vertexShader = new VertexShader(this->m_Renderer->GetDevice(), L"Sandbox/Resources/Shaders/vs.hlsl", bxlayout, numElements);
	pixelShader = new PixelShader(this->m_Renderer->GetDevice(), L"Sandbox/Resources/Shaders/ps.hlsl");

	Texture* texture = Hollow::ResourceManager::Get()->CreateTextureResource(
		this->GetDevice(),
		this->GetDeviceContext(),
		(wchar_t*)L"Sandbox/Resources/Textures/metal_texture.jpg"
	);
}

void RenderSystem::PreUpdate(float_t dt) 
{
	this->camera->Update();
	this->UpdateWVP();

	this->m_Renderer->Clear();
	this->m_Renderer->PreUpdateFrame();

	this->m_Renderer->SetContantBuffer<WVP>(HOLLOW_CONST_BUFFER_WVP_SLOT, &WVPConstantBuffer);
	this->m_Renderer->SetVertexShader(vertexShader);
	this->m_Renderer->SetPixelShader(pixelShader);
}

void RenderSystem::Update(float_t dt, std::vector<GameObject*>& gameObjects)
{
	for (auto object : gameObjects) {
		MeshComponent * meshComponent = object->GetComponent<MeshComponent>();
		PositionComponent * posComponent = object->GetComponent<PositionComponent>();

		if (meshComponent == nullptr || posComponent == nullptr) continue;

		for (auto& it : meshComponent->mesh->objects) {
			this->m_Renderer->SetVertexBuffer<SimpleVertex>(&it->buffer);
			this->m_Renderer->SetShaderResource(HOLLOW_SHADER_RESOURCE_VIEW_DIFFUSE_TEXTURE_SLOT, it->material->diffuse_texture->m_TextureShaderResource);
			this->UpdateTransform(posComponent, false);
			this->m_Renderer->SetContantBuffer(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, &transformConstantBuffer);
			m_Renderer->Draw(it->buffer.BufferSize());
		}
	}
}

void RenderSystem::PostUpdate(float_t dt) 
{
	m_Renderer->Present();
}

void RenderSystem::UpdateTransform(PositionComponent * comp, bool has_texture)
{
	transformConstantBuffer.data.transform = XMMatrixTranspose(
		(XMMatrixTranslation(comp->position.x, comp->position.y, comp->position.z)
		* XMMatrixScaling(comp->scale.x, comp->scale.y, comp->scale.z))
		* XMMatrixRotationRollPitchYaw(comp->rotation.x, comp->rotation.y, comp->rotation.z)
	);
	//transformConstantBuffer.data.bowol = has_texture;	
	transformConstantBuffer.Update();
}

void RenderSystem::UpdateWVP()
{
	WVPConstantBuffer.data.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* camera->GetViewMatrix() 
		* camera->GetProjectionMatrix());
	WVPConstantBuffer.Update();
}
