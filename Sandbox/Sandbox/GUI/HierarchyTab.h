#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/MoveComponent.h"

using namespace Hollow;

namespace GUI {
	class HierarchyTab
	{
	public:
		GameObject* selectedGameObject;
	public:
		HierarchyTab() {}

		void Draw()
		{
			ImGui::Begin("Hierarchy");
			int counter = 0;
			for (auto& entity : EntityManager::instance()->container<GameObject>()) {
				if (ImGui::Selectable(("Entity" + std::to_string(counter++)).c_str())) {
					selectedGameObject = &entity;
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

			if (ImGui::Button("Add enitity", ImVec2(ImGui::GetWindowWidth() - 10, 30))) {
				GameObject* entity = EntityManager::instance()->create<GameObject>();
			}
			ImGui::End();
		}
	};
}
