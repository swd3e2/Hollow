#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Texture.h"
#include "Sandbox/Entities/Light.h"
#include "Sandbox/Components/LightComponent.h"
#include "Sandbox/Components/PhysicsComponent.h"

namespace GUI {
	class HierarchyTab
	{
	public:
		GameObject* selectedGameObject;
		Terrain* selectedTerrain;
		Light* selectedLight;
		RenderableObject* selectedRenderable;
		Hollow::Material* selectedMaterial;
		std::string filename;
		bool openEntityCreationPopup = false;
		char buffer[100];
		const char* lightTypeComboItems[4] = { "Ambient", "Diffuse", "Point", "Spot" };
		const char* currentLightType = nullptr;
	public:
		HierarchyTab() = default;

		void Draw()
		{
			ImGui::Begin("Hierarchy");
			if (ImGui::Button("+")) {
				openEntityCreationPopup = true;
			}

			if (openEntityCreationPopup) {
				ImGui::OpenPopup("Entity creation");
			}

			if (ImGui::BeginPopupModal("Entity creation")) {
				if (ImGui::Button("GameObject")) {
					Hollow::EntityManager::instance()->create<GameObject>();
					ImGui::CloseCurrentPopup();
					openEntityCreationPopup = false;
				}
				if (ImGui::Button("Terrain")) {
					Hollow::EntityManager::instance()->create<Terrain>();
					ImGui::CloseCurrentPopup();
					openEntityCreationPopup = false;
				}
				if (ImGui::Button("Light")) {
					Hollow::EntityManager::instance()->create<Light>();
					ImGui::CloseCurrentPopup();
					openEntityCreationPopup = false;
				}
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
					openEntityCreationPopup = false;
				}
				ImGui::EndPopup();
			}

			int counter = 0;

			ImGui::BulletText("GameObjects");
			ImGui::Indent();
			for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
				size_t entityId = entity.getId();
				ImGui::PushID(entityId);
				if (ImGui::Selectable(entity.name.size() ? entity.name.c_str() : ("Entity " + std::to_string(entity.getId())).c_str())) {
					selectedGameObject = &entity;
					selectedTerrain = nullptr;
					selectedLight = nullptr;
				}
				
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
					ImGui::SetDragDropPayload("Entity", &entityId, sizeof(size_t));        // Set payload to carry the index of our item (could be anything)
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity")) {
						HW_INFO("{}", *reinterpret_cast<size_t*>(payload->Data));
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();
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
					if (ImGui::Button("Delete")) {
						Hollow::DelayedTaskManager::instance()->add([&entity]() { Hollow::EntityManager::instance()->destroy(entity.getId()); });
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::Unindent();
			counter = 0;
			ImGui::BulletText("Lights");
			ImGui::Indent();
			for (auto& light : Hollow::EntityManager::instance()->container<Light>()) {
				if (ImGui::Selectable(("Light " + std::to_string(counter++)).c_str())) {
					selectedGameObject = nullptr;
					selectedTerrain = nullptr;
					selectedLight = &light;
				}
				if (ImGui::BeginPopupContextItem())
				{
					ImGui::Text("Add component:");
					if (ImGui::Button("Transform")) {
						light.addComponent<TransformComponent>();
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::Button("LightComponent")) {
						light.addComponent<LightComponent>();
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			ImGui::Unindent();
			ImGui::End();

			ImGui::Begin("Inspector");
			// Light
			if (selectedLight != nullptr) {
				if (selectedLight->hasComponent<LightComponent>()) {
					if (ImGui::CollapsingHeader("Light component")) {
						LightComponent* lightComponent = selectedLight->getComponent<LightComponent>();
						ImGui::DragFloat3("Position", (float*)& lightComponent->lightData.position, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Direction", (float*)& lightComponent->lightData.direction, 0.1f, -10000.0f, 10000.0f);
						ImGui::ColorEdit3("Color", (float*)& lightComponent->lightData.color);
						ImGui::DragFloat("Constant", &lightComponent->lightData.constant, 0.01f);
						ImGui::DragFloat("Linear", &lightComponent->lightData.linear, 0.01f);
						ImGui::DragFloat("Quadratic", &lightComponent->lightData.quadratic, 0.01f);
						ImGui::DragFloat("Cutoff", &lightComponent->lightData.cutoff, 0.01f);
						ImGui::DragFloat("Distance", &lightComponent->lightData.distance, 0.01f);


						if (ImGui::BeginCombo("Light type", lightTypeComboItems[lightComponent->lightData.type])) {
							for (int n = 0; n < 4; n++) {
								bool is_selected = (lightTypeComboItems[lightComponent->lightData.type] == lightTypeComboItems[n]);
								if (ImGui::Selectable(lightTypeComboItems[n], is_selected))
									lightComponent->lightData.type = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
							}
							ImGui::EndCombo();
						}
					}
				}
			}

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
						std::vector<Hollow::s_ptr<RenderableObject>>& renderables = renderableComponent->renderables;
						for (auto& it : renderables) {
							if (ImGui::Selectable(std::string("Mesh " + std::to_string(it->id)).c_str())) {
								selectedRenderable = it.get();
								selectedMaterial = renderableComponent->materials[it->material].get();
							}
						}
						if (ImGui::Button("Load from file")) {
							filename = Hollow::FileSystem::openFile("");
							if (filename.size()) {
								//DelayedTaskManager::instance()->add([&, renderableComponent]() { renderableComponent->load(filename); });
							}
						}
					}
				}

				if (selectedGameObject->hasComponent<TransformComponent>()) {
					if (ImGui::CollapsingHeader("Transform component")) {
						TransformComponent* component = selectedGameObject->getComponent<TransformComponent>();
						if (ImGui::DragFloat3("Position", (float*)& component->position, 0.1f, -10000.0f, 10000.0f)) {
							if (selectedGameObject->hasComponent<PhysicsComponent>()) {
								PhysicsComponent* physics = selectedGameObject->getComponent<PhysicsComponent>();

								btTransform origin;
								physics->body->getMotionState()->getWorldTransform(origin);
								btVector3& originVec = origin.getOrigin();
								physics->body->activate(true);
								physics->body->translate(btVector3(
									component->position.x - originVec.getX(),
									component->position.y - originVec.getY(),
									component->position.z - originVec.getZ()
								));
							}
						}
						ImGui::DragFloat3("Rotation", (float*)&component->rotation, 0.1f, -10000.0f, 10000.0f);
						ImGui::DragFloat3("Scale", (float*)& component->scale, 0.1f, -10000.0f, 10000.0f);
					}
				}
				if (selectedGameObject->hasComponent<AnimationComponent>()) {
					if (ImGui::CollapsingHeader("Animation component")) {
						AnimationComponent* animation = selectedGameObject->getComponent<AnimationComponent>();

						if (ImGui::BeginCombo("Animation", animation->animations[animation->currentAnimation]->name.c_str())) {
							for (int n = 0; n < animation->animations.size(); n++) {
								bool is_selected = (animation->animations[animation->currentAnimation] == animation->animations[n]);
								if (ImGui::Selectable(animation->animations[n]->name.c_str(), is_selected))
									animation->currentAnimation = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
							}
							ImGui::EndCombo();
						}
						if (ImGui::Button("Play")) {
							animation->play();
						}
						ImGui::SameLine();
						if (ImGui::Button("Pause")) {
							animation->pause();
						}
						ImGui::SameLine();
						if (ImGui::Button("Stop")) {
							animation->stop();
						}
						if (ImGui::TreeNode(animation->rootNode->name.c_str())) {
							drawAnimationHierarchy(animation->rootNode);
							ImGui::TreePop();
						}
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
					if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(std::static_pointer_cast<Hollow::D3D11Texture>(selectedMaterial->diffuseTexture)->m_TextureShaderResource, ImVec2(100, 100));
					} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLTexture>(selectedMaterial->diffuseTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##diffuse_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						//TextureManager::instance()->remove(selectedMaterial->diffuseTexture);
						selectedMaterial->materialData.hasDiffuseTexture = true;
						//selectedMaterial->diffuseTexture = Hollow::TextureManager::instance()->createTextureFromFile("2.png");
						//selectedMaterial->diffuseTexture = Hollow::TextureManager::instance()->createTextureFromFile(filename, false);
					}
				}

				ImGui::Text("Normal texture");
				if (selectedMaterial->normalTexture != nullptr) {
					if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(std::static_pointer_cast<Hollow::D3D11Texture>(selectedMaterial->normalTexture)->m_TextureShaderResource, ImVec2(100, 100));
					}
					else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLTexture>(selectedMaterial->normalTexture)->textureId, ImVec2(100, 100));
					}

					ImGui::SameLine();
				}

				if (ImGui::Button("Change##normal_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						/*TextureManager::instance()->remove(selectedMaterial->normalTexture);
						selectedMaterial->normalTexture = TextureManager::instance()->createTextureFromFile(filename, false);*/
					}
				}

				ImGui::Text("Specular texture");
				if (selectedMaterial->specularTexture != nullptr) {
					if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(std::static_pointer_cast<Hollow::D3D11Texture>(selectedMaterial->specularTexture)->m_TextureShaderResource, ImVec2(100, 100));
					}
					else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLTexture>(selectedMaterial->specularTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##specular_texture")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						/*TextureManager::instance()->remove(selectedMaterial->specularTexture);
						selectedMaterial->specularTexture = TextureManager::instance()->createTextureFromFile(filename, false);*/
					}
				}
			}
			ImGui::End();
		}

		private:
			void drawAnimationHierarchy(Hollow::s_ptr<Node>& node)
			{
				for (auto& it : node->childs) {
					if (ImGui::TreeNode(it->name.c_str())) {
						if (ImGui::TreeNode(("Local transform###" + it->name).c_str())) {
							ImGui::DragFloat4(("###" + it->name + "1").c_str(), (float*)&it->localTransform.r[0], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "2").c_str(), (float*)&it->localTransform.r[1], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "3").c_str(), (float*)&it->localTransform.r[2], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "4").c_str(), (float*)&it->localTransform.r[3], 0.01f, -10000.0f, 10000.0f);
							ImGui::TreePop();
						}
						drawAnimationHierarchy(it);
						ImGui::TreePop();
					}
				}
			}
	};
}