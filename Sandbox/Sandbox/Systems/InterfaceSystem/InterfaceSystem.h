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

	float* newEntityPosition;
	float* newEntityRotation;
	float* newEntityScale;

	float* currentEntityPosition = new float[3];
	float* currentEntityRotation = new float[3];
	float* currentEntityScale = new float[3];
	MeshComponent*						meshComponent;
	PositionComponent *					posComponent;
	Hollow::Material*					material;
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

		newEntityPosition = new float[3];
		newEntityPosition[0] = 0.0f;
		newEntityPosition[1] = 0.0f;
		newEntityPosition[2] = 0.0f;

		newEntityRotation = new float[3];
		newEntityRotation[0] = 0.0f;
		newEntityRotation[1] = 0.0f;
		newEntityRotation[2] = 0.0f;

		newEntityScale = new float[3];
		newEntityScale[0] = 0.0f;
		newEntityScale[1] = 0.0f;
		newEntityScale[2] = 0.0f;
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

		if (ImGui::Button("Add entity", ImVec2(100, 20))) {
			GameObject* newEntity = this->entityManager->CreateEntity<GameObject>();
			newEntity->AddComponent<MeshComponent, Hollow::Mesh*>(
				Hollow::ResourceManager::Get()->CreateMeshResource(
					this->renderer->GetDevice(),
					this->renderer->GetDeviceContext(),
					filePath.c_str(),
					fileSystem.get_current_file_path().c_str(),
					true)
				);
			newEntity->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ -17.0f, 17.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });
			newEntity->AddComponent<SelectComponent, bool>(false);
		}
		ImGui::SameLine();
		if (ImGui::Button("Destroy entity", ImVec2(100, 20))) {
			if (entity != nullptr) {
				entityManager->DestroyEntity<GameObject>(entity->GetEntityID());
			}
		}

		ImGui::BeginChild("Entities", ImVec2(120, 150), ImGuiWindowFlags_AlwaysUseWindowPadding);
		for (auto& it : *entityManager->GetEntitiesList()) {
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
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("File path", ImVec2(200, 150), ImGuiWindowFlags_AlwaysUseWindowPadding);
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
		ImGui::DragFloat3("Position", newEntityPosition);
		ImGui::DragFloat3("Rotation", newEntityRotation);
		ImGui::DragFloat3("Scale", newEntityScale);

		ImGui::BeginChild("Entity editor", ImVec2(330, 200), ImGuiWindowFlags_AlwaysUseWindowPadding);
		if (entity != nullptr) {
			ImGui::Text("Entity %d", entity->GetEntityID());

			meshComponent = entity->GetComponent<MeshComponent>();
			posComponent = entity->GetComponent<PositionComponent>();

			if (meshComponent != nullptr && meshComponent->IsActive()) {
				ImGui::TextColored(ImVec4(0.7f, 0.43f, 0.0f, 1.0f), "Objects:");
				for (auto& object : meshComponent->mesh->objects) {
					ImGui::Text("%s", object->name.c_str());
				}

				ImGui::TextColored(ImVec4(0.33f, 0.7f, 0.19f, 1.0f), "Materials:");
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

				ImGui::DragFloat3("Position", currentEntityPosition);
				ImGui::DragFloat3("Rotation", currentEntityRotation);
				ImGui::DragFloat3("Scale", currentEntityScale);

				posComponent->setTransform(currentEntityPosition, currentEntityScale, currentEntityRotation);
			}
		}
		ImGui::EndChild();

		ImGui::BeginChild("Material editor", ImVec2(330, 200), ImGuiWindowFlags_AlwaysUseWindowPadding);
		if (material != nullptr) {
			ImGui::TextColored(ImVec4(0.2f, 0.43f, 0.7f, 1.0f), "Material: %s", material->name.c_str());
			if (material->diffuse_texture != nullptr && !material->diffuse_texture->name.empty()) {
				ImGui::Text("Material: %s", material->diffuse_texture->name.c_str());
			}
		}

		ImGui::EndChild();
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
