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
#include "Sandbox/Components/ParticleComponent.h"

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

		Hollow::Vector3 boxShapeSize;
		float capsuleShapeRadius = 0.0f;
		float capsuleShapeHeight = 0.0f;

		float sphereShapeRadius = 0.0f;

		Hollow::Vector3 planeShapeNormal;
		float planeSize = 0.0f;

		float mass = 0.0f;
		Hollow::Vector3 position;
		Hollow::Vector3 angularFactor;
		int selectedShapeType;
		int tempShapeType = 0;
		const char* shapeTypesSelectables[5] = { "Box", "Sphere", "Plane", "Capsule", "AABB (not working)" };

		Hollow::Vector3 particlePosition;
		Hollow::Vector3 particleVelocity;
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
					if (selectedGameObject->hasComponent<PhysicsComponent>()) {
						PhysicsComponent* physics = selectedGameObject->getComponent<PhysicsComponent>();
						tempShapeType = physics->type;
					} else {
						tempShapeType = 0;
					}
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
						ImGui::ColorEdit4("Color", (float*)& lightComponent->lightData.color);
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
						ImGui::Text("Meshed");
						for (auto& it : renderables) {
							if (ImGui::Selectable(std::string("Mesh " + std::to_string(it->id)).c_str())) {
								selectedRenderable = it.get();
								selectedMaterial = renderableComponent->materials[it->material].get();
							}
						}
						ImGui::Text("Nodes");
						if (renderableComponent->rootNode != nullptr) {
							if (ImGui::TreeNode(renderableComponent->rootNode->name.c_str())) {
								drawNodeHierarchy(renderableComponent->rootNode.get());
								ImGui::TreePop();
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
						PhysicsComponent* physics = nullptr; 
						if (selectedGameObject->getComponent<PhysicsComponent>()) {
							physics = selectedGameObject->getComponent<PhysicsComponent>();
						}
						if (ImGui::DragFloat3("Position", (float*)& component->position, 0.1f)) {
							if (physics != nullptr) {
								physics->setPosition(component->position);
							}
						}
						ImGui::DragFloat3("Rotation", (float*)&component->rotation, 0.1f);
						ImGui::DragFloat3("Scale", (float*)& component->scale, 0.1f);
					}
				}
				if (selectedGameObject->hasComponent<PhysicsComponent>()) {
					if (ImGui::CollapsingHeader("Physics component")) {
						PhysicsComponent* physics = selectedGameObject->getComponent<PhysicsComponent>();
						ImGui::Checkbox("Apply rotation", &physics->applyRotation);

						if (ImGui::BeginCombo("Animation", shapeTypesSelectables[tempShapeType])) {
							for (int n = 0; n < 5; n++) {
								bool is_selected = (tempShapeType == n);
								if (ImGui::Selectable(shapeTypesSelectables[n], is_selected))
									tempShapeType = n;
								if (is_selected)
									ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
							}
							ImGui::EndCombo();
						}
						ImGui::DragFloat3("Shape origin position", (float*)&position, 0.1f);
						ImGui::DragFloat3("Shape angular factor", (float*)&angularFactor, 0.1f);
						ImGui::DragFloat("Shape mass (0 means static object)", &mass, 0.1f);

						if (tempShapeType == PhysicsShapeType::PST_BOX) {
							ImGui::DragFloat3("Box size", (float*)&boxShapeSize, 0.1f);
						} else if (tempShapeType == PhysicsShapeType::PST_CAPSULE) {
							ImGui::DragFloat("Capsule height", &capsuleShapeHeight, 0.1f);
							ImGui::DragFloat("Capsule radius", &capsuleShapeRadius, 0.1f);
						} else if (tempShapeType == PhysicsShapeType::PST_SPHERE) {
							ImGui::DragFloat("Sphere radius", &sphereShapeRadius, 0.1f);
						}

						if (ImGui::Button("Add body")) {
							physics->setPosition(position);
							physics->setMass(mass);
							switch (tempShapeType)
							{
								case PhysicsShapeType::PST_BOX:
									physics->addBoxShape(boxShapeSize);
									break;
								case PhysicsShapeType::PST_SPHERE:
									physics->addSphereShape(sphereShapeRadius);
									break;
								case PhysicsShapeType::PST_PLANE:
									physics->addPlaneShape(planeShapeNormal, planeSize);
									break;
								case PhysicsShapeType::PST_CAPSULE:
									physics->addCapsuleShape(capsuleShapeHeight, capsuleShapeRadius);
									break;
								case PhysicsShapeType::PST_AABB:
									break;
								default:
								break;
							}
							physics->init();
							physics->body->setAngularFactor(btVector3(angularFactor.x, angularFactor.y, angularFactor.z));
							PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());
						}
					}
				}
				
				if (selectedGameObject->hasComponent<AnimationComponent>()) {
					if (ImGui::CollapsingHeader("Animation component")) {
						AnimationComponent* animation = selectedGameObject->getComponent<AnimationComponent>();
						
						ImGui::DragFloat("Blending factor", &animation->blendingFactor, 0.001f, 0.0f, 1.0f);
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
						if (ImGui::TreeNode(animation->rootJoint->name.c_str())) {
							drawAnimationHierarchy(animation->rootJoint);
							ImGui::TreePop();
						}
					}
				}
				if (selectedGameObject->hasComponent<ParticleComponent>()) {
					if (ImGui::CollapsingHeader("Particle component")) {
						ParticleComponent* particle = selectedGameObject->getComponent<ParticleComponent>();

						if (ImGui::CollapsingHeader("Patricles")) {
							for (auto& it : particle->particles) {
								ImGui::Text(("Position: " + std::to_string(it->position.x) + " " + std::to_string(it->position.y) + " " + std::to_string(it->position.z)).c_str());
								ImGui::Text(("Velocity: " + std::to_string(it->velocity.x) + " " + std::to_string(it->velocity.y) + " " + std::to_string(it->velocity.z)).c_str());
								ImGui::Text("-----------------------------------------------------------------");
							}
						}

						ImGui::DragFloat("Particle lifetime", &particle->lifetime);
						ImGui::DragFloat("Particle max particles", &particle->maxParticles);
						ImGui::DragFloat3("Particle position", (float*)&particlePosition);
						ImGui::DragFloat3("Particle velocity", (float*)&particleVelocity);

						if (ImGui::Button("Emit")) {
							particle->emit(particlePosition, particleVelocity);
						}
					}
				}
			}
			ImGui::End();


			ImGui::Begin("Material properties");
			if (selectedMaterial != nullptr) {
				ImGui::ColorEdit4("Base color", (float*)& selectedMaterial->materialData.color);
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
						Hollow::DelayedTaskManager::instance()->add([&]() {
							TextureManager::instance()->remove(selectedMaterial->diffuseTexture);
						});
						selectedMaterial->materialData.hasDiffuseTexture = true;
						selectedMaterial->diffuseTexture = TextureManager::instance()->create2DTextureFromFile(filename, 2);
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
			ImGui::Begin("Textures");
			int imgCounter = 0;
			for (auto& it : TextureManager::instance()->textureList) {
				if (++imgCounter % 3 != 0) ImGui::SameLine();

				if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
					ImGui::Image(std::static_pointer_cast<Hollow::D3D11Texture>(it.second)->m_TextureShaderResource, ImVec2(100, 100));
				} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLTexture>(it.second)->textureId, ImVec2(100, 100));
				}
			}
			ImGui::End();
		}

		private:
			void drawNodeHierarchy(RenderableComponent::Node* node)
			{
				for (auto& it : node->childs) {
					if (ImGui::TreeNode(it->name.c_str())) {
						if (ImGui::TreeNode(("Node info###" + it->name).c_str())) {
							ImGui::Text("Mesh"); 
							ImGui::SameLine(); 
							ImGui::Text(std::to_string(it->renderableId).c_str());
							ImGui::Text("Node transforms");
							ImGui::DragFloat3(("Translation###NT" + it->name).c_str(), (float*)&it->translation, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat3(("Scale###NS" + it->name).c_str(), (float*)&it->scale, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("Rotation###NR" + it->name).c_str(), (float*)&it->rotation, 0.01f, -10000.0f, 10000.0f);
							ImGui::Text("World transform matrix");
							ImGui::DragFloat4(("###" + it->name + "1").c_str(), (float*)&it->worldTransform.r[0], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "2").c_str(), (float*)&it->worldTransform.r[1], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "3").c_str(), (float*)&it->worldTransform.r[2], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "4").c_str(), (float*)&it->worldTransform.r[3], 0.01f, -10000.0f, 10000.0f);
							ImGui::TreePop();
						}
						drawNodeHierarchy(it);
						ImGui::TreePop();
					}
				}
			}

			void drawAnimationHierarchy(Hollow::s_ptr<Joint>& node)
			{
				for (auto& it : node->childs) {
					if (ImGui::TreeNode(it->name.c_str())) {
						if (ImGui::TreeNode(("Node info###" + it->name).c_str())) {
							ImGui::Text("Node transforms");
							ImGui::DragFloat3(("Translation###T" + it->name).c_str(), (float*)&it->translation, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat3(("Scale###S" + it->name).c_str(), (float*)&it->scale, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("Rotation###R" + it->name).c_str(), (float*)&it->rotation, 0.01f, -10000.0f, 10000.0f);
							ImGui::Text("Current transforms");
							ImGui::DragFloat3(("Translation###CT" + it->name).c_str(), (float*)&it->currentTranslation, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat3(("Scale###CS" + it->name).c_str(), (float*)&it->currentScale, 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("Rotation###CR" + it->name).c_str(), (float*)&it->currentRotation, 0.01f, -10000.0f, 10000.0f);
							ImGui::Text("Inverse bind matrix");
							ImGui::DragFloat4(("###" + it->name + "1").c_str(), (float*)&it->inverseBindMatrix.r[0], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "2").c_str(), (float*)&it->inverseBindMatrix.r[1], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "3").c_str(), (float*)&it->inverseBindMatrix.r[2], 0.01f, -10000.0f, 10000.0f);
							ImGui::DragFloat4(("###" + it->name + "4").c_str(), (float*)&it->inverseBindMatrix.r[3], 0.01f, -10000.0f, 10000.0f);
							ImGui::TreePop();
						}
						drawAnimationHierarchy(it);
						ImGui::TreePop();
					}
				}
			}
	};
}