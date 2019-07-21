#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Hollow/Core/DelayedTaskManager.h"

using namespace Hollow;

namespace GUI {
	class InspectorTab
	{
	public:
		RenderableObject* selectedRenderable;
		Material* selectedMaterial;
		std::string filename;
	public:
		InspectorTab() {}

		void Draw(GameObject* selectedGameObject)
		{
			ImGui::Begin("Inspector");
			if (selectedGameObject != nullptr) {
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
		}
	};
}
