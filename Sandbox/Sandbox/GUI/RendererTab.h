#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Systems/ForwardRenderSystem.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11RenderTarget.h"

using namespace Hollow;

namespace GUI {
	class RendererTab
	{
	public:
		ForwardRenderSystem* renderSystem;
	public:
		RendererTab() {}

		void Draw()
		{
			ImGui::Begin("Renderer", NULL);
			ImGui::Text("Main camera speed");
			ImGui::DragFloat("##Maincameraspeed", &renderSystem->m_Camera->cameraMoveSpeed, 0.001f, 0.0f, 100.0f);
			ImGui::Text("Shadow camera speed");
			ImGui::DragFloat("##Shadowcameraspeed", &renderSystem->shadow.shadowCamera->cameraMoveSpeed, 0.001f, 0.0f, 100.0f);
			if (ImGui::Button("Toggle camera")) {
				if (renderSystem->m_Camera->mainCamera) {
					renderSystem->m_Camera->mainCamera = false;
					renderSystem->shadow.shadowCamera->mainCamera = true;
				} else {
					renderSystem->m_Camera->mainCamera = true;
					renderSystem->shadow.shadowCamera->mainCamera = false;
				}
			}
			if (ImGui::TreeNode("Shaders")) {
				auto& shaders = ShaderManager::instance()->shaders;
				for (auto& it : shaders) {
					ImGui::Text(it.first.c_str());
					ImGui::SameLine();
					std::string buttonName = "Reload##" + it.first;
					if (ImGui::Button(buttonName.c_str())) {
						ShaderManager::instance()->reloadShader(it.second);
					}
				}
				ImGui::TreePop();
			}
			ImGui::End();

			ImGui::Begin("Main");
			if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
				ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->main)->GetShaderResourceView()[0], ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
			} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->main)->texture[0], ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 16), ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();

			ImGui::Begin("Scene");
			if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
				ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetDepthStencilResource(), ImVec2(200, 200));
				ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[0], ImVec2(200, 200));
				ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[1], ImVec2(200, 200));
				ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[2], ImVec2(200, 200));
				ImGui::Image((void*)static_cast<D3D11RenderTarget*>(renderSystem->shadow.renderTarget)->GetShaderResourceView()[0], ImVec2(200, 200));
				ImGui::Image((void*)static_cast<D3D11RenderTarget*>(renderSystem->shadow.renderTarget)->GetDepthStencilResource(), ImVec2(200, 200));
			} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->depth, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[1], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[2], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->shadow.renderTarget)->texture[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->shadow.renderTarget)->depth, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
			}
			ImGui::End();
		}
	};
}