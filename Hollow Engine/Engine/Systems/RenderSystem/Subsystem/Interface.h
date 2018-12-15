#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "d3d11.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/imgui_impl_win32.h"
#include "Engine/ImGui/imgui_impl_dx11.h"
#include "Engine/Common/Log.h"
#include "Engine/Input/InputManager.h"

namespace Hollow {

	class Interface
	{
	private:
		static ID3D11Device*				m_Device;
		static ID3D11DeviceContext*			m_DeviceContext;
	public:
		static void Init(HWND* hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
		{
			bool result = true;
			m_Device = device;
			m_DeviceContext = deviceContext;

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			result = ImGui_ImplWin32_Init(*hwnd);
			if (!result) Log::GetCoreLogger()->critical("Can't init imgui_win32");
			result = ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
			if (!result) Log::GetCoreLogger()->critical("Can't init imgui_dx11");
			ImGui::StyleColorsDark();
		}
		
		static void PreUpdate()
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		static void Render()
		{
			ImGui::Begin("Info");
			for (short i = 0; i < 5; i++) {
				std::string tempString = "Button ";
				tempString += static_cast<eMouseKeyCodes>(i);
				tempString += " status ";
				tempString.push_back(InputManager::GetMouseButtonIsPressed(static_cast<eMouseKeyCodes>(i)));
				ImGui::Text(tempString.c_str());
			}
			
			ImGui::End();
		}

		static void PostUpdate()
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		static void Update()
		{
			PreUpdate();
			Render();
			PostUpdate();
		}
	};
}

#endif