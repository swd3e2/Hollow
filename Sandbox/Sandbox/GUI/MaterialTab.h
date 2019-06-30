#pragma once

#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Hollow/Graphics/TextureManager.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Texture.h"

using namespace Hollow;
namespace GUI {
	class MaterialTab
	{
	public:
		std::string filename;
	public:
		MaterialTab() {}

		void Draw(Hollow::Material* selectedMaterial)
		{
			ImGui::Begin("Material properties");
			if (selectedMaterial != nullptr) {
				ImGui::DragFloat4("Base color", (float*)& selectedMaterial->materialData.color, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Metallic", &selectedMaterial->materialData.metallicFactor, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Emissive", &selectedMaterial->materialData.emissiveFactor, 0.001f, 0.0f, 1.0f);
				ImGui::DragFloat("Roughness", &selectedMaterial->materialData.roughnessFactor, 0.001f, 0.0f, 1.0f);

				ImGui::Text("Diffuse texture");
				if (selectedMaterial->diffuseTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->diffuseTexture)->m_TextureShaderResource, ImVec2(100, 100));
					} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->diffuseTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##diffuse_texture")) {
					filename = Hollow::FileSystem::OpenFile("");
					if (filename.size()) {
						TextureManager::instance()->Remove(selectedMaterial->diffuseTexture);
						selectedMaterial->diffuseTexture = TextureManager::instance()->CreateTextureFromFile(filename, false);
					}
				}

				ImGui::Text("Normal texture");
				if (selectedMaterial->normalTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->normalTexture)->m_TextureShaderResource, ImVec2(100, 100));
					} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->normalTexture)->textureId, ImVec2(100, 100));
					}

					ImGui::SameLine();
				}

				if (ImGui::Button("Change##normal_texture")) {
					filename = Hollow::FileSystem::OpenFile("");
					if (filename.size()) {
						TextureManager::instance()->Remove(selectedMaterial->normalTexture);
						selectedMaterial->normalTexture = TextureManager::instance()->CreateTextureFromFile(filename, false);
					}
				}

				ImGui::Text("Specular texture");
				if (selectedMaterial->specularTexture != nullptr) {
					if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
						ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->specularTexture)->m_TextureShaderResource, ImVec2(100, 100));
					} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
						ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->specularTexture)->textureId, ImVec2(100, 100));
					}
					ImGui::SameLine();
				}

				if (ImGui::Button("Change##specular_texture")) {
					filename = Hollow::FileSystem::OpenFile("");
					if (filename.size()) {
						TextureManager::instance()->Remove(selectedMaterial->specularTexture);
						selectedMaterial->specularTexture = TextureManager::instance()->CreateTextureFromFile(filename, false);
					}
				}
			}
			ImGui::End();
		}
	};
}
