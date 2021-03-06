#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Systems/RenderSystem.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11RenderTarget.h"
#include "Hollow/Common/Helper.h"
#include "Sandbox/Profiler.h"


namespace GUI {
	class RendererTab
	{
	public:
		RenderSystem* renderSystem;
		int r = -1000, l = 1000, t = -1000, b = 1000, n = -1000, f = 2000;
	public:
		void Draw(double dt)
		{
			ImGui::Begin("Renderer", NULL);

			if (ImGui::Button("somebutton")) {
				std::vector<Hollow::Vertex> vertices;
				vertices.push_back(Hollow::Vertex(0.0f, 100.0f, 0.0f, 0.0f, 0.0f));
				vertices.push_back(Hollow::Vertex(0.0f, -100.0f, 0.0f, 0.0f, 1.0f));

				renderSystem->lineVB->update(vertices.data(), sizeof(Hollow::Vertex) * 2);
			}

			ImGui::Text(("Physical memory usage (Kb): " + std::to_string(Profiler::getMemoryUsage() / 1024)).c_str());

			ImGui::Checkbox("Debug physics draw", &renderSystem->enableDebugPhysicsDraw);

			ImGui::Text("GBuffer pass");
			ImGui::SameLine();
			ImGui::Text(std::to_string(renderSystem->gbufferTime).c_str());

			ImGui::Text("Picker pass");
			ImGui::SameLine();
			ImGui::Text(std::to_string(renderSystem->pickerTime).c_str());

			ImGui::Text("Shadow pass");
			ImGui::SameLine();
			ImGui::Text(std::to_string(renderSystem->shadowTime).c_str());

			ImGui::Text("Light pass");
			ImGui::SameLine();
			ImGui::Text(std::to_string(renderSystem->lightTime).c_str());

			ImGui::Text(std::to_string(dt).c_str());
			ImGui::Text(std::to_string(renderSystem->culled).c_str());
			//ImGui::DragFloat("##Maincameraspeed", &renderSystem->m_Camera->cameraMoveSpeed, 0.001f, 0.0f, 100.0f);

			if (ImGui::CollapsingHeader("Shadow orthographic projection settings")) {
				ImGui::Text("Right");
				ImGui::DragInt("##right", &r);
				ImGui::Text("Left");
				ImGui::DragInt("##left", &l);

				ImGui::Text("Top");
				ImGui::DragInt("##top", &t);
				ImGui::Text("Bottom");
				ImGui::DragInt("##bottom", &b);

				ImGui::Text("Near");
				ImGui::DragInt("##near", &n);
				ImGui::Text("Far");
				ImGui::DragInt("##far", &f);

				ImGui::Text("Bias");
				ImGui::DragFloat("##bias", &renderSystem->shadow.bias, 0.0000001f, 0.0f, 0.0001f);

				renderSystem->shadow.shadowCamera->setOrthoValues(r, l, t, b, n, f);
			}

			for (auto& it : Profiler::getValues()) {
				ImGui::Text((it.first + ": " + std::to_string(it.second)).c_str());
			}
			ImGui::End();

			ImGui::Begin("Scene");
			if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
				if (ImGui::CollapsingHeader("G buffer")) {
					ImGui::Image(std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->gBuffer)->getDepthStencilResource(), ImVec2(200, 200));
					ImGui::Image(std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->gBuffer)->getShaderResourceView()[0], ImVec2(200, 200));
					ImGui::Image(std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->gBuffer)->getShaderResourceView()[1], ImVec2(200, 200));
					ImGui::Image(std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->gBuffer)->getShaderResourceView()[2], ImVec2(200, 200));
				}
				if (ImGui::CollapsingHeader("Shadow map")) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->shadow.renderTarget)->getShaderResourceView()[0], ImVec2(200, 200));
					ImGui::Image((void*)std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->shadow.renderTarget)->getDepthStencilResource(), ImVec2(200, 200));
				}
				if (ImGui::CollapsingHeader("Picker map")) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::D3D11RenderTarget>(renderSystem->picker)->getShaderResourceView()[0], ImVec2(200, 200));
				}
			} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
				if (ImGui::CollapsingHeader("G buffer")) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->gBuffer)->depth, ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->gBuffer)->texture[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->gBuffer)->texture[1], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->gBuffer)->texture[2], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				}
				if (ImGui::CollapsingHeader("Shadow map")) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->shadow.renderTarget)->texture[0], ImVec2(200, 200));
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->shadow.renderTarget)->depth, ImVec2(200, 200));
				}
				if (ImGui::CollapsingHeader("Picker map")) {
					ImGui::Image((void*)std::static_pointer_cast<Hollow::OGLRenderTarget>(renderSystem->picker)->texture[0], ImVec2(200, 200), ImVec2(0, 1), ImVec2(1, 0));
				}
			}
			ImGui::End();
		}
	};
}