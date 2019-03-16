#pragma once
#include <vector>
#include "Layer.h"
#include "Hollow/Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Graphics/DirectX/D3DRenderable.h"
#include "Hollow/Resources/TextureManager.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Resources/ShaderManager.h"

class ImGuiLayer : public Layer
{
private:
	D3DRenderable* selected;
	RenderableObject* selectedObject;
	float currentPosition[3] = {};
	float currentRotation[3] = {};
	float currentScale[3] = {};
	float ambientLightColor[3] = {};
	float ambientLightDirection[3] = {};

	float pointLightColor[3] = {};
	float pointLightPosition[3] = {};
	float pointLightStrenght = 0;
	float pointLightSpecularPower = 0;

	D3DRenderer* renderer;
	ShaderManager* shaderManager;
	D3DVertexShader* vShader;
	D3DPixelShader* pShader;
	bool* p_open;
	std::vector<IRenderable*>* list;
	const char* current_item = NULL;
public:
	ImGuiLayer(D3DRenderer* renderer, std::vector<IRenderable*>* list) :
		renderer(renderer), list(list)
	{
		bool result = true;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		result = ImGui_ImplWin32_Init(*((Win32Window*)Win32Window::instance())->getHWND());
		if (!result) HW_ERROR("Can't init imgui_win32");
		result = ImGui_ImplDX11_Init(renderer->getDevice(), renderer->getDeviceContext());
		if (!result) HW_ERROR("Can't init imgui_dx11");
		ImGui::StyleColorsDark();

		p_open = new bool;
		*p_open = false;
		
		shaderManager = ShaderManager::instance();
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

		// Begin docking viewport
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruDockspace;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	virtual void Update() override
	{
		ImGui::Begin("Editor");
		if (ImGui::CollapsingHeader("Header"))
		{
			ImGui::Text("Ambient");
			ImGui::Spacing();

			ImGui::Text("Ambient Light color");
			ImGui::ColorEdit3("###", ambientLightColor);
			ImGui::Spacing();

			ImGui::Text("Ambient Light direction");
			ImGui::DragFloat3("###", ambientLightDirection, 0.01f, -1.0f, 1.0f);
			ImGui::Spacing();

			ImGui::Text("Point");
			ImGui::ColorEdit3("Point Light color", pointLightColor);
			ImGui::DragFloat3("Point Light position", pointLightPosition, 0.01f, -30.0f, 30.0f);
			ImGui::SliderFloat("Point Light range", &pointLightStrenght, -5.0f, 5.0f);
			ImGui::SliderFloat("Point Light specular power", &pointLightSpecularPower, -5.0f, 5.0f);

			if (renderer->pointLight != nullptr) {
				renderer->pointLight->setColor(pointLightColor);
				renderer->pointLight->setPosition(pointLightPosition);
			}
		}
		if (ImGui::CollapsingHeader("Shaders"))
		{
			if (ImGui::BeginCombo("##pixelShaderCombo", "Pixel shader")) // The second parameter is the label previewed before opening the combo.
			{
				for (auto& it : *shaderManager->getPixelShaderList())
				{
					if (ImGui::Selectable(it.first.c_str(), pShader == it.second)) {
						pShader = it.second;
						renderer->SetPixelShader(pShader);
					}
					if (pShader)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("##vertexShaderCombo", "Vertex shader")) // The second parameter is the label previewed before opening the combo.
			{
				for (auto& it : *shaderManager->getVertexShaderList())
				{
					if (ImGui::Selectable(it.first.c_str(), vShader == it.second)) {
						vShader = it.second;
						renderer->SetVertexShader(vShader);
					}
					if (pShader)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
				ImGui::EndCombo();
			}
		}
		ImGui::End();

		ImGui::Begin("Scene objects");
		for (auto& it : *list)
		{
			D3DRenderable* renderable = (D3DRenderable*)it;
			if (ImGui::Selectable(renderable->name.c_str()))
			{
				selected = renderable;
			}
		}
		ImGui::End();

		ImGui::Begin("Object properties");
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

			ImGui::Text("Position: x | y | z");
			ImGui::DragFloat3("##position", currentPosition, 0.1f);
			ImGui::Text("Rotation");
			ImGui::DragFloat3("##rotation", currentRotation, 0.1f);
			ImGui::Text("Scale: x | y | z");
			ImGui::DragFloat3("##scale", currentScale, 0.1f);

			selected->transform->setPosition(currentPosition[0], currentPosition[1], currentPosition[2]);
			selected->transform->setRotation(currentRotation[0], currentRotation[1], currentRotation[2]);
			selected->transform->setScale(currentScale[0], currentScale[1], currentScale[2]);

			ImGui::Spacing();

			for (auto& it : selected->renderableObjects)
			{
				if (ImGui::Selectable(it->name.c_str()))
				{
					selectedObject = it;
				}
			}
		}
		ImGui::End();

		ImGui::Begin("12312312312");
		for (auto& it : *TextureManager::instance()->getTexuresList())
		{
			ImGui::Text(it.first.c_str());
		}
		ImGui::End();

		ImGui::Begin("Material properties");
		if (selectedObject != nullptr)
		{
			if (selectedObject->material->pixelShader != nullptr) {
				if (ImGui::BeginCombo("##materialVertexShaderCombo", "Vertex shader")) // The second parameter is the label previewed before opening the combo.
				{
					for (auto& it : *shaderManager->getVertexShaderList())
					{
						if (ImGui::Selectable(it.first.c_str(), selectedObject->material->vertexShader == it.second)) {
							selectedObject->material->vertexShader = it.second;
						}
					}
					ImGui::EndCombo();
				}
			}

			if (selectedObject->material->vertexShader != nullptr) {
				if (ImGui::BeginCombo("##materialPixelShaderCombo", "Pixel shader")) // The second parameter is the label previewed before opening the combo.
				{
					for (auto& it : *shaderManager->getPixelShaderList())
					{
						if (ImGui::Selectable(it.first.c_str(), selectedObject->material->pixelShader == it.second)) {
							selectedObject->material->pixelShader = it.second;
						}
					}
					ImGui::EndCombo();
				}
			}
		}
		ImGui::End();
	}


	virtual void PostUpdate() override
	{
		// End docking viewport
		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
};