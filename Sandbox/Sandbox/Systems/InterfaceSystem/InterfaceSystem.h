#pragma once
#include "d3d11.h"
#include "Sandbox/ImGui/imgui.h"
#include "Sandbox/ImGui/imgui_impl_win32.h"
#include "Sandbox/ImGui/imgui_impl_dx11.h"
#include "Hollow/Common/Log.h"
#include "Hollow/ECS/Systems/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include <string>
#include "Hollow/Graphics/Renderer.h"
#include "Hollow/Graphics/DirectXRenderer.h"
#include "Hollow/Containers/vector.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/ECS/Entities/IEntity.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Utils/FileSystem.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Hollow/Input/InputManager.h"
#include "Hollow/Containers/vector.h"
#include "Sandbox/Components/MoveComponent.h"

class InterfaceSystem : public Hollow::System<InterfaceSystem>
{
private:
	Hollow::DirectXRenderer*			renderer;
	Hollow::EntityManager*				entityManager;
	Hollow::ComponentManager*			componentManager;
	Hollow::FileSystem					fileSystem;
	int									currentPixelShader = 0;
	int									currentVertexShader = 0;
	std::vector<const char*>			pixelShadersList;
	std::vector<const char*>			vertexShadersList;
	Hollow::IEntity*					entity = nullptr;
	std::vector<std::string>*			v = nullptr;
	std::string							currentFilePath = "C:\\";
	std::string							filePath;
	std::string							selectedFile;
	bool								file_path_opened = false;

	float* currentEntityPosition = new float[3];
	float* currentEntityRotation = new float[3];
	float* currentEntityScale = new float[3];
	
	MeshComponent*						meshComponent;
	PositionComponent *					posComponent;
	Hollow::Material*					material;
	DirectX::XMFLOAT4 temp;
	ImVec2 childSize = { 120, 150 };
public:
	InterfaceSystem(HWND* hwnd)
		: entityManager(Hollow::Engine::Get()->GetEntityManager()), componentManager(Hollow::Engine::Get()->GetComponentManager())
	{
		bool result = true;
		this->renderer = (Hollow::DirectXRenderer*)Hollow::Renderer::Get();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		result = ImGui_ImplWin32_Init(*hwnd);
		if (!result) Hollow::Log::GetCoreLogger()->critical("Can't init imgui_win32");
		result = ImGui_ImplDX11_Init(this->renderer->GetDevice(), this->renderer->GetDeviceContext());
		if (!result) Hollow::Log::GetCoreLogger()->critical("Can't init imgui_dx11");
		ImGui::StyleColorsDark();
		for (auto &it : Hollow::ResourceManager::Get()->pixelShaders)
			pixelShadersList.push_back(it.first.c_str());
		for (auto &it : Hollow::ResourceManager::Get()->vertexShaders)
			vertexShadersList.push_back(it.first.c_str());

		v = fileSystem.read_next_directory("C");
	}

	virtual void PreUpdate(float_t dt) override
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	virtual void Update(float_t dt) override
	{
		PreUpdate(dt);

		ImGui::Begin("Resource manager"); 
		ImGui::Text("Current pixel shader");
		ImGui::Combo("", &currentPixelShader, pixelShadersList.data(), pixelShadersList.size(), 10);
		ImGui::Text("Current vertex shader");
		ImGui::Combo("", &currentVertexShader, vertexShadersList.data(), vertexShadersList.size(), 10);

		if (ImGui::Button("Add entity")) {
			GameObject* newEntity = this->entityManager->CreateEntity<GameObject>();
			newEntity->AddComponent<MeshComponent, Hollow::Mesh*>(
				Hollow::ResourceManager::Get()->CreateMeshResource(
					this->renderer->GetDevice(),
					this->renderer->GetDeviceContext(),
					filePath.c_str(),
					fileSystem.get_current_file_path().c_str(),
					false)
				);
			newEntity->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });
			newEntity->AddComponent<SelectComponent, bool>(false);
		}
		if (ImGui::Button("Destroy entity")) {
			if (entity != nullptr) {
				entityManager->DestroyEntity<GameObject>(entity->GetEntityID());
			}
		}

		ImGui::BeginChild("Entities", childSize, ImGuiWindowFlags_AlwaysUseWindowPadding);

		Hollow::Containers::Vector<Hollow::IEntity*>* container = entityManager->GetEntitiesList();

		for (Hollow::IEntity* it : *container) {
			SelectComponent* select = it->GetComponent<SelectComponent>();
			std::string id = std::to_string(it->GetEntityID());
			id.insert(0, "Entity ");
			if (select != nullptr) {
				if (ImGui::Selectable(id.c_str(), select->selected)) {
					select->selected = true;
					entity = it;
				}
				else if (entity == nullptr || entity->GetEntityID() != it->GetEntityID()) {
					select->selected = false;
				}
			}
		}

		delete container;

		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("File path", childSize, ImGuiWindowFlags_AlwaysUseWindowPadding);
		ImGui::Text(currentFilePath.c_str());
		for (auto& it : *v) {
			if (it.find(".obj") != -1) {
				if (ImGui::Selectable(it.c_str(), it.c_str() == selectedFile.c_str())) {
					selectedFile = it;
					filePath = fileSystem.get_current_file_path();
					filePath.append(it);
				}
			} else {
				if (ImGui::Selectable(it.c_str(), false)) {
					v = fileSystem.read_next_directory(it);
					currentFilePath = fileSystem.get_current_file_path();
					break;
				}
			}
		}
		ImGui::EndChild();
		ImGui::Text("delta: %f", dt);

		ImGui::Text("Mouse x: %f", Hollow::InputManager::mcx);
		ImGui::SameLine();
		ImGui::Text("Mouse x: %f", Hollow::InputManager::mcy);
		ImGui::Text("Mouse x: %f", Hollow::InputManager::pcx);
		ImGui::SameLine();
		ImGui::Text("Mouse x: %f", Hollow::InputManager::pcy);

		XMStoreFloat4(&temp, Hollow::InputManager::pickRayInWorldSpacePos);

		ImGui::Text("Mouse position x: %f", temp.x);
		ImGui::SameLine();
		ImGui::Text("Mouse position y: %f", temp.y);
		ImGui::SameLine();
		ImGui::Text("Mouse position z: %f", temp.z);

		XMStoreFloat4(&temp, Hollow::InputManager::pickRayInWorldSpaceDir);

		ImGui::Text("Mouse direction x: %f", temp.x);
		ImGui::SameLine();
		ImGui::Text("Mouse direction y: %f", temp.y);
		ImGui::SameLine();
		ImGui::Text("Mouse direction z: %f", temp.z);

		ImGui::Text("Intersect x: %f", Hollow::InputManager::pix);
		ImGui::SameLine();
		ImGui::Text("Intersect y: %f", Hollow::InputManager::piy);
		ImGui::SameLine();
		ImGui::Text("Intersect z: %f", Hollow::InputManager::piz);
		ImGui::End();

		ImGui::Begin("Entity editor");
		if (entity != nullptr) {
			ImGui::Text("Entity %d", entity->GetEntityID());

			meshComponent = entity->GetComponent<MeshComponent>();
			posComponent = entity->GetComponent<PositionComponent>();
			MoveComponent* moveComponent = entity->GetComponent<MoveComponent>();

			if (meshComponent != nullptr && meshComponent->IsActive()) {
				ImGui::Text("Objects:");
				for (auto& object : meshComponent->mesh->objects) {
					ImGui::Text("%s", object->name.c_str());
				}

				ImGui::Text("Materials:");
				for (auto& object : meshComponent->mesh->objects) {
					if (ImGui::Selectable(object->material->name.c_str(), material && material->name == object->material->name)) {
						material = object->material;
					}
				}
			}

			if (posComponent != nullptr) {
				currentEntityPosition[0] = posComponent->position.x;
				currentEntityPosition[1] = posComponent->position.y;
				currentEntityPosition[2] = posComponent->position.z;

				currentEntityRotation[0] = posComponent->rotation.x;
				currentEntityRotation[1] = posComponent->rotation.y;
				currentEntityRotation[2] = posComponent->rotation.z;

				currentEntityScale[0] = posComponent->scale.x;
				currentEntityScale[1] = posComponent->scale.y;
				currentEntityScale[2] = posComponent->scale.z;

				ImGui::DragFloat3("Position", currentEntityPosition, 0.01);
				ImGui::DragFloat3("Rotation", currentEntityRotation, 0.01);
				ImGui::DragFloat3("Scale", currentEntityScale, 0.01);

				posComponent->setTransform(currentEntityPosition, currentEntityScale, currentEntityRotation);
			}

			if (moveComponent != nullptr) {
				ImGui::Text("Distance %f", moveComponent->distance);
				ImGui::Text("Distance passed %f", moveComponent->distancePassed);
			}
		}

		if (material != nullptr) {
			ImGui::Text("Material: %s", material->name.c_str());
			if (material->diffuse_texture != nullptr && !material->diffuse_texture->name.empty()) {
				ImGui::Text("Material: %s", material->diffuse_texture->name.c_str());
			}
		}

		ImGui::End();

		PostUpdate(dt);
	}

	virtual void PostUpdate(float_t dt) override
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		this->renderer->SetPixelShader(Hollow::ResourceManager::Get()->GetPixelShader(this->pixelShadersList[currentPixelShader]));
		this->renderer->SetVertexShader(Hollow::ResourceManager::Get()->GetVertexShader(this->vertexShadersList[currentVertexShader]));
	}
};
