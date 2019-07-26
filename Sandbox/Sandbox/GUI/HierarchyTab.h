#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Texture.h"

using namespace Hollow;

namespace GUI {
	class HierarchyTab
	{
	public:
		GameObject* selectedGameObject;
		Terrain* selectedTerrain;
		RenderableObject* selectedRenderable;
		Material* selectedMaterial;
		std::string filename;
		char buffer[100];
	public:
		HierarchyTab() = default;

		void Draw()
		{
			ImGui::Begin("Hierarchy");
			int counter = 0;

			if (ImGui::BeginTabBar("MyTabBar"))
			{
				if (ImGui::BeginTabItem("Gameobjects"))
				{
					ImGui::BeginChild("GameObjectsList", ImVec2(ImGui::GetWindowContentRegionWidth(), 330), false, ImGuiWindowFlags_HorizontalScrollbar);
					for (auto& entity : EntityManager::instance()->container<GameObject>()) {
						if (ImGui::Selectable(entity.name.size() ? entity.name.c_str() : ("Entity " + std::to_string(counter++)).c_str())) {
							selectedGameObject = &entity;
							selectedTerrain = nullptr;
						}
						if (ImGui::BeginPopupContextItem())
						{
							ImGui::Text("Add component:");
							if (ImGui::Button("Renderable")) {
								entity.addComponent<RenderableComponent>();
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Transform")) {
								entity.addComponent<TransformComponent>();
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Move")) {
								entity.addComponent<MoveComponent>();
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("Selected")) {
								entity.addComponent<SelectComponent>(true);
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
					}
					ImGui::EndChild();

					if (ImGui::Button("Add gameobject", ImVec2(ImGui::GetWindowWidth() - 20, 30))) {
						EntityManager::instance()->create<GameObject>();
					}

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Terrains"))
				{
					counter = 0;
					for (auto& terrain : EntityManager::instance()->container<Terrain>()) {
						if (ImGui::Selectable(("Terrain " + std::to_string(counter++)).c_str())) {
							selectedGameObject = nullptr;
							selectedTerrain = &terrain;
						}
						if (ImGui::BeginPopupContextItem())
						{
							ImGui::Text("Add component:");
							if (ImGui::Button("Transform")) {
								terrain.addComponent<TransformComponent>();
								ImGui::CloseCurrentPopup();
							}
							if (ImGui::Button("TerrainData")) {
								terrain.addComponent<TerrainData>();
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
					}

					if (ImGui::Button("Add terrain", ImVec2(ImGui::GetWindowWidth() - 20, 30))) {
						EntityManager::instance()->create<Terrain>();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Cucumber"))
				{
					ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();



			ImGui::Begin("Inspector");
			// Terrain
			if (selectedTerrain != nullptr) {
				if (selectedTerrain->hasComponent<TransformComponent>()) {
					if (ImGui::CollapsingHeader("Transform component")) {
						TransformComponent* component = selectedTerrain->getComponent<TransformComponent>();
						ImGui::DragFloat3("Position", (float*)& component->position, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Rotation", (float*)& component->rotation, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Scale", (float*)& component->scale, 0.1f, -10000.0f, 10000.0f);
					}
				}
			}

			if (selectedTerrain != nullptr) {
				if (selectedTerrain->hasComponent<TerrainData>()) {
					if (ImGui::CollapsingHeader("TerrainData component")) {
						TerrainData* terrainDataComponent = selectedTerrain->getComponent<TerrainData>();

						if (terrainDataComponent->vBuffer == nullptr) {
							if (ImGui::Button("Load")) {
								filename = Hollow::FileSystem::openFile("");
								if (filename.size()) {
									terrainDataComponent->load(filename);
								}
							}
						}
					}
				}
			}
			// Game object
			if (selectedGameObject != nullptr) {
				if (ImGui::InputText("Name", buffer, 100)) {
					selectedGameObject->SetName(buffer);
				}
				if (selectedGameObject->hasComponent<RenderableComponent>()) {
					if (ImGui::CollapsingHeader("Renderable component")) {
						RenderableComponent* renderableComponent = selectedGameObject->getComponent<RenderableComponent>();
						std::vector<RenderableObject>& renderables = renderableComponent->renderables;
						for (auto& it : renderables) {
							if (ImGui::Selectable(std::string("Mesh " + std::to_string(it.id)).c_str())) {
								selectedRenderable = &it;
								selectedMaterial = &renderableComponent->materials[it.material];
							}
						}
						if (ImGui::Button("Load from file")) {
							filename = Hollow::FileSystem::openFile("");
							if (filename.size()) {
								DelayedTaskManager::instance()->add([&, renderableComponent]() { renderableComponent->load(filename); });
							}
						}
					}
				}
			}

			if (selectedGameObject != nullptr) {
				if (selectedGameObject->hasComponent<TransformComponent>()) {
					if (ImGui::CollapsingHeader("Transform component")) {
						TransformComponent* component = selectedGameObject->getComponent<TransformComponent>();
						ImGui::DragFloat3("Position", (float*)& component->position, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Rotation", (float*)& component->rotation, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Scale", (float*)& component->scale, 0.1f, -10000.0f, 10000.0f);
					}
				}
			}
			ImGui::End();


			ImGui::Begin("Material properties");
			if (selectedMaterial != nullptr) {
				ImGui::DragFloat4("Base color", (float*)& selectedMaterial->materialData.color, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Metallic", &selectedMaterial->materialData.metallicFactor, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Emissive", &selectedMaterial->materialData.emissiveFactor, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Roughness", &selectedMaterial->materialData.roughnessFactor, 0.001f, 0.0f, 1.0f);

				ImGui::Text("Diffuse texture");
				if (selectedMaterial->diffuseTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->diffuseTexture)->m_TextureShaderResource, ImVec2(100, 100));
					}
					else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->diffuseTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##diffuse_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						TextureManager::instance()->remove(selectedMaterial->diffuseTexture);
						selectedMaterial->diffuseTexture = TextureManager::instance()->createTextureFromFile(filename, false);
					}
				}

				ImGui::Text("Normal texture");
				if (selectedMaterial->normalTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->normalTexture)->m_TextureShaderResource, ImVec2(100, 100));
					}
					else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->normalTexture)->textureId, ImVec2(100, 100));
					}

					ImGui::SameLine();
				}

				if (ImGui::Button("Change##normal_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						TextureManager::instance()->remove(selectedMaterial->normalTexture);
						selectedMaterial->normalTexture = TextureManager::instance()->createTextureFromFile(filename, false);
					}
				}

				ImGui::Text("Specular texture");
				if (selectedMaterial->specularTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->specularTexture)->m_TextureShaderResource, ImVec2(100, 100));
					}
					else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->specularTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##specular_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						TextureManager::instance()->remove(selectedMaterial->specularTexture);
						selectedMaterial->specularTexture = TextureManager::instance()->createTextureFromFile(filename, false);
					}
				}
			}
			ImGui::End();
		}
	};
}