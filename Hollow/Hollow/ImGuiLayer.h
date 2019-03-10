#pragma once
#include "Layer.h"
#include "Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Common/Log.h"
#include <vector>
#include "Graphics/DirectX/D3DRenderable.h"

class ImGuiLayer : public Layer
{
private:
	D3DRenderable* selected;
	RenderableObject* selectedObject;
	float currentPosition[3] = {};
	float currentRotation[3] = {};
	float currentScale[3] = {};
	float lightColor[3] = {};
	float lightDirection[3] = {};
	D3DRenderer* renderer;
	std::vector<IRenderable*>* list;
public:
	ImGuiLayer(D3DRenderer* renderer, std::vector<IRenderable*>* list) :
		renderer(renderer), list(list)
	{
		bool result = true;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		result = ImGui_ImplWin32_Init(*((Win32Window*)Win32Window::instance())->getHWND());
		if (!result) HW_ERROR("Can't init imgui_win32");
		result = ImGui_ImplDX11_Init(renderer->getDevice(), renderer->getDeviceContext());
		if (!result) HW_ERROR("Can't init imgui_dx11");
		ImGui::StyleColorsDark();
	}

	~ImGuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	virtual void PreUpdate() override
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	virtual void Update() override
	{
		ImGui::Begin("Resource manager");

		ImGui::DragFloat3("Light color", lightColor, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("Light direction", lightDirection, 0.01f, -1.0f, 1.0f);

		renderer->light.direction.x = lightDirection[0];
		renderer->light.direction.y = lightDirection[1];
		renderer->light.direction.z = lightDirection[2];

		renderer->light.ambient.x = lightColor[0];
		renderer->light.ambient.y = lightColor[1];
		renderer->light.ambient.z = lightColor[2];


		for (auto& it : *list) 
		{
			D3DRenderable* renderable = (D3DRenderable*)it;
			if (ImGui::Selectable(renderable->name.c_str()))
			{
				selected = renderable;
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (selected != nullptr)
		{
			currentPosition[0] = selected->transform->position.x;
			currentPosition[1] = selected->transform->position.y;
			currentPosition[2] = selected->transform->position.z;

			currentRotation[0] = selected->transform->rotation.x;
			currentRotation[1] = selected->transform->rotation.y;
			currentRotation[2] = selected->transform->rotation.z;

			currentScale[0] = selected->transform->scale.x;
			currentScale[1] = selected->transform->scale.y;
			currentScale[2] = selected->transform->scale.z;

			ImGui::DragFloat3("Position", currentPosition, 0.1f);
			ImGui::DragFloat3("Rotation", currentRotation, 0.1f);
			ImGui::DragFloat3("Scale", currentScale, 0.1f);

			selected->transform->setPosition(currentPosition[0], currentPosition[1], currentPosition[2]);
			selected->transform->setRotation(currentRotation[0], currentRotation[1], currentRotation[2]);
			selected->transform->setScale(currentScale[0], currentScale[1], currentScale[2]);

			for (auto& it : selected->renderableObjects)
			{
				if (ImGui::Selectable(it->name.c_str()))
				{
					selectedObject = it;
				}
			}
		}
		ImGui::End();
	}


	virtual void PostUpdate() override
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
};