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

void RenderSystem::Update(float_t dt)
{
	for (auto object : *Hollow::Engine::Get()->GetEntityManager()->GetEntitiesList()) {
		MeshComponent * meshComponent = object->GetComponent<MeshComponent>();
		PositionComponent * posComponent = object->GetComponent<PositionComponent>();
		SelectComponent* selectComponent = object->GetComponent<SelectComponent>();

		if (meshComponent == nullptr || posComponent == nullptr || selectComponent == nullptr) continue;

		this->UpdateConstBuffers(posComponent, object->GetEntityID(), selectComponent->selected);
		this->renderer->SetContantBuffer<Transform>(HOLLOW_CONST_BUFFER_MESH_TRANSFORM_SLOT, &transformConstantBuffer);

		for (auto& it : meshComponent->mesh->objects) {
			this->renderer->SetVertexBuffer<SimpleVertex>(&it->buffer);
			if (it->material != nullptr && it->material->diffuse_texture != nullptr && it->material->diffuse_texture->m_TextureShaderResource != nullptr)
				this->renderer->SetShaderResource(HOLLOW_SHADER_RESOURCE_VIEW_DIFFUSE_TEXTURE_SLOT, it->material->diffuse_texture->m_TextureShaderResource);
			else
				this->renderer->FreeShaderResource(HOLLOW_SHADER_RESOURCE_VIEW_DIFFUSE_TEXTURE_SLOT);

			renderer->Draw(it->buffer.BufferSize());
		}
	}
}

void RenderSystem::PostUpdate(float_t dt) 
{
	renderer->Present();
}

void RenderSystem::UpdateConstBuffers(PositionComponent * comp, int entityId, bool selected)
{
	transformConstantBuffer.data.transform = XMMatrixTranspose(
		(XMMatrixScaling(comp->scale.x, comp->scale.y, comp->scale.z)
		* XMMatrixRotationRollPitchYaw(comp->rotation.x, comp->rotation.y, comp->rotation.z))
		* XMMatrixTranslation(comp->position.x, comp->position.y, comp->position.z)
	);
	transformConstantBuffer.data.id = entityId;
	transformConstantBuffer.data.selected = selected;
	transformConstantBuffer.Update();
}

void RenderSystem::UpdateWVP()
{
	WVPConstantBuffer.data.WVP = XMMatrixTranspose(XMMatrixIdentity()
		* camera->GetViewMatrix() 
		* camera->GetProjectionMatrix());
	WVPConstantBuffer.Update();
}
