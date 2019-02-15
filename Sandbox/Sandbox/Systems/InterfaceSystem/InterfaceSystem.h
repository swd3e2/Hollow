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

class InterfaceSystem : public Hollow::System<InterfaceSystem>
{
private:
	Hollow::Core::Graphics::DirectXRenderer*	renderer;
	Hollow::EntityManager*				entityManager;
	Hollow::ComponentManager*			componentManager;

	int currentPixelShader = 0;
	int currentVertexShader = 0;
	std::vector<const char*> pixelShadersList;
	std::vector<const char*> vertexShadersList;
public:
	InterfaceSystem(HWND* hwnd, Hollow::EntityManager* entityManager, Hollow::ComponentManager* componentManager)
		: entityManager(entityManager), componentManager(componentManager)
	{
		bool result = true;
		this->renderer = (Hollow::Core::Graphics::DirectXRenderer*)Hollow::Renderer::Get();

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
		ImGui::Combo("Current pixel shader", &currentPixelShader, pixelShadersList.data(), pixelShadersList.size());
		ImGui::Combo("Current pixel shader", &currentVertexShader, vertexShadersList.data(), vertexShadersList.size());
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
