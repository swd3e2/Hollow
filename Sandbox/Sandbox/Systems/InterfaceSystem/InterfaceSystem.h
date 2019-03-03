#pragma once
#include "d3d11.h"
#include "Sandbox/ImGui/imgui.h"
#include "Sandbox/ImGui/imgui_impl_win32.h"
#include "Sandbox/ImGui/imgui_impl_dx11.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Graphics/IRenderer.h"
#include "Hollow/ECS/Systems/System.h"

class InterfaceSystem : public Hollow::System<InterfaceSystem>
{
public:
	InterfaceSystem()
	{
		D3DRenderer* renderer = static_cast<D3DRenderer*>(IRenderer::instance());
		bool result = true;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		result = ImGui_ImplWin32_Init(GetModuleHandle(NULL));
		if (!result) Hollow::Log::GetCoreLogger()->critical("Can't init imgui_win32");
		result = ImGui_ImplDX11_Init(renderer->getDevice(), renderer->getDeviceContext());
		if (!result) Hollow::Log::GetCoreLogger()->critical("Can't init imgui_dx11");
		ImGui::StyleColorsDark();
	}

	virtual void PreUpdate(float_t dt) override
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	virtual void Update(float_t dt) override
	{
		ImGui::Begin("Resource manager"); 
		ImGui::End();
	}

	virtual void PostUpdate(float_t dt) override
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
};