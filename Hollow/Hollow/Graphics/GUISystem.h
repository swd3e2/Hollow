#pragma once
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_opengl3.h"
#include "Renderer/DirectX/D3D11RenderApi.h"
#include "Renderer/DirectX/D3D11Context.h"
#include "Renderer/DirectX/D3D11Win32Window.h"
#include "Renderer/OpenGL/Win32/OGLWin32Window.h"
#include "Renderer/Base/Window.h"

class GUISystem
{
public:
	GUISystem(Window* window, RenderApi* renderer)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		
#ifdef D3D11
		ImGui_ImplWin32_Init(static_cast<D3D11Win32Window*>(window)->getHWND());
		D3D11Context* context = static_cast<D3D11RenderApi*>(renderer)->getContext();
		ImGui_ImplDX11_Init(context->getDevice(), context->getDeviceContext());
#endif
#ifdef OPENGL
		ImGui_ImplWin32_Init(static_cast<OGLWin32Window*>(window)->getHWND());
		const char* glsl_version = "#version 460";
		ImGui_ImplOpenGL3_Init(glsl_version);
#endif
	}

	void update(double dt)
	{
#ifdef OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#endif
#ifdef D3D11
		ImGui_ImplDX11_NewFrame();
#endif
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
};