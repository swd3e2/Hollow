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

		int r = -1000, l = 1000, t = -1000, b = 1000, n = -1000, f = 2000;
	public:
		RendererTab() {}

		void Draw()
		{
			ImGui::Begin("Renderer", NULL);
			ImGui::Text("Main camera speed");
			ImGui::DragFloat("##Maincameraspeed", &renderSystem->m_Camera->cameraMoveSpeed, 0.001f, 0.0f, 100.0f);

			if (ImGui::CollapsingHeader("Shadow orthographic projection settings")) {
				ImGui::DragInt("##right", &r);
				ImGui::DragInt("##left", &l);

				ImGui::DragInt("##top", &t);
				ImGui::DragInt("##bottom", &b);

				ImGui::DragInt("##near", &n);
				ImGui::DragInt("##far", &f);
				renderSystem->shadow.shadowCamera->SetOrthoValues(r, l, t, b, n, f);
			}


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

			ImGui::Begin("Scene");
			if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
				if (ImGui::CollapsingHeader("G buffer")) {
					ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetDepthStencilResource(), ImVec2(200, 200));
					ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[0], ImVec2(200, 200));
					ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[1], ImVec2(200, 200));
					ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->gBuffer)->GetShaderResourceView()[2], ImVec2(200, 200));
				}
				if (ImGui::CollapsingHeader("Shadow map")) {
					ImGui::Image((void*)static_cast<D3D11RenderTarget*>(renderSystem->shadow.renderTarget)->GetShaderResourceView()[0], ImVec2(200, 200));
					ImGui::Image((void*)static_cast<D3D11RenderTarget*>(renderSystem->shadow.renderTarget)->GetDepthStencilResource(), ImVec2(200, 200));
				}
			} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
				if (ImGui::CollapsingHeader("G buffer")) {
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->depth, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[1], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->gBuffer)->texture[2], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				}
				if (ImGui::CollapsingHeader("Shadow map")) {
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->shadow.renderTarget)->texture[0], ImVec2(200, 200));
					ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->shadow.renderTarget)->depth, ImVec2(200, 200));
				}
			}
			ImGui::End();
		}
	};
}