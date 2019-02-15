#include "RenderSystem.h"

RenderSystem::RenderSystem(int width, int height)
{
	Hollow::Log::GetCoreLogger()->debug("RenderSystem: Initialized.");
	this->renderer = static_cast<Core::Graphics::DirectXRenderer*>(Renderer::Get());

	this->camera = new Camera();
	this->camera->SetProjectionValues(85.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	this->camera->SetPosition(3.0f, 2.0f, -5.0f);

	transformConstantBuffer.Init(this->renderer->GetDevice(), this->renderer->GetDeviceContext());
	WVPConstantBuffer.Init(this->renderer->GetDevice(), this->renderer->GetDeviceContext());

	this->renderer->SetVertexShader(ResourceManager::Get()->GetVertexShader("simple"));
	this->renderer->SetPixelShader(ResourceManager::Get()->GetPixelShader("simple"));
}

void RenderSystem::PreUpdate(float_t dt) 
{
	this->camera->Update();
	this->UpdateWVP();

	this->renderer->Clear();
	this->renderer->PreUpdateFrame();

	this->renderer->SetContantBuffer<WVP>(HOLLOW_CONST_BUFFER_WVP_SLOT, &WVPConstantBuffer);
}

void RenderSystem::Update(float_t dt, std::vector<GameObject*>& gameObjects)
{
	for (auto object : gameObjects) {
		MeshComponent * meshComponent = object->GetComponent<MeshComponent>();
		PositionComponent * posComponent = object->GetComponent<PositionComponent>();

		if (meshComponent == nullptr || posComponent == nullptr) continue;

		this->UpdateConstBuffers(posComponent, object->GetEntityID());
		this->renderer->SetContantBuffer<Transform>(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, &transformConstantBuffer);

		for (auto& it : meshComponent->mesh->objects) {
			this->renderer->SetVertexBuffer<SimpleVertex>(&it->buffer);
			if (it->material != nullptr && it->material->diffuse_texture != nullptr && it->material->diffuse_texture->m_TextureShaderResource != nullptr)
				this->renderer->SetShaderResource(HOLLOW_SHADER_RESOURCE_VIEW_DIFFUSE_TEXTURE_SLOT, it->material->diffuse_texture->m_TextureShaderResource);
			
			renderer->Draw(it->buffer.BufferSize());
		}
	}
}

void RenderSystem::PostUpdate(float_t dt) 
{
	renderer->Present();
}

void RenderSystem::UpdateConstBuffers(PositionComponent * comp, int entityId)
{
	transformConstantBuffer.data.transform = XMMatrixTranspose(
		(XMMatrixTranslation(comp->position.x, comp->position.y, comp->position.z)
		* XMMatrixScaling(comp->scale.x, comp->scale.y, comp->scale.z))
		* XMMatrixRotationRollPitchYaw(comp->rotation.x, comp->rotation.y, comp->rotation.z)
	);
	transformConstantBuffer.data.id = entityId;
	transformConstantBuffer.Update();
}

void RenderSystem::UpdateWVP()
{
	WVPConstantBuffer.data.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* camera->GetViewMatrix() 
		* camera->GetProjectionMatrix());
	WVPConstantBuffer.Update();
}
