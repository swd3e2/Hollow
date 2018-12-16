#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "d3d11.h"
#include "Engine/ImGui/imgui.h"
#include "Engine/ImGui/imgui_impl_win32.h"
#include "Engine/ImGui/imgui_impl_dx11.h"
#include "Engine/Common/Log.h"
#include "Engine/Systems/System.h"
#include "Engine/EntityManager.h"
#include "Engine/ComponentManager.h"

namespace Hollow {

	class InterfaceSystem : public System<InterfaceSystem>
	{
	private:
		static ID3D11Device*				m_Device;
		static ID3D11DeviceContext*			m_DeviceContext;
		EntityManager* entityManager;
		ComponentManager* componentManager;
	public:
		InterfaceSystem(HWND* hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, EntityManager* entityManager, ComponentManager* componentManager)
			: entityManager(entityManager), componentManager(componentManager)
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

		virtual void PreUpdate(float_t dt) override
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		virtual void Update(float_t dt) override
		{
			PreUpdate(dt);
			ImGui::Begin("Info");
			entityManager->GetMemoryUsed();
			ImGui::Text("Entity manager");

			for (size_t it : entityManager->GetMemoryUsed()) {
				float result = (float)it / 1024 / 1024;
				std::string first;
				first += std::to_string(result).c_str();
				first += " mb.";
				ImGui::Text(first.c_str());
				ImGui::SameLine();
				ImGui::Text("128 mb.");
			}
			ImGui::Text("Component manager");
			for (size_t it : componentManager->GetMemoryUsed()) {
				float result = (float)it / 1024 / 1024;
				std::string first;
				first += std::to_string(result).c_str();
				first += " mb.";
				ImGui::Text(first.c_str());
				ImGui::SameLine();
				ImGui::Text("128 mb.");
			}

			ImGui::End();
			PostUpdate(dt);
		}

		virtual void PostUpdate(float_t dt) override
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	};
	
}

#endif