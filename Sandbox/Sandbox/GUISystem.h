#pragma once
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_opengl3.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11RenderApi.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Context.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Renderer/OpenGL/Win32/OGLWin32Window.h"
#include "Hollow/Graphics/Renderer/Base/Window.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Texture.h"
#include "Hollow/Graphics/Renderer/OpenGL/OGLTexture.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11RenderTarget.h"
#include "Hollow/Graphics/Renderer/OpenGL/OGLRenderTarget.h"
#include "Hollow/Graphics/TextureManager.h"
#include <Hollow/Events/IEventListener.h>
#include <Hollow/Events/EventSystem.h>
#include <Hollow/Core/ProjectSettings.h>

#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Systems/ForwardRenderSystem.h"
#include "Sandbox/Components/GLTFRenderable.h"
#include "Sandbox/Events.h"

using namespace Hollow;

enum DrawTypes {
	Debug = 0,
	Default = 1
};

class GUISystem : public Hollow::IEventListener
{
public:
	bool open = true;
	std::string filename = "";
	GameObject* selectedGameObject;
	ForwardRenderSystem* renderSystem;
	GLTFRenderableObject* selectedRenderable;
	Material* selectedMaterial;
	int drawMode = 0;

	GUISystem(Window* window, RenderApi* renderer)
	{
		Hollow::EventSystem::instance()->addEventListener(this, &GUISystem::onChangePickedMesh, ChangeSelectedEntity::getStaticEventId());


		bool result = true;
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		
#ifdef D3D11
		ImGui_ImplWin32_Init(*static_cast<D3D11Win32Window*>(window)->getHWND());
		D3D11Context& context = static_cast<D3D11RenderApi*>(renderer)->getContext();
		ImGui_ImplDX11_Init(context.getDevice(), context.getDeviceContext());
#endif
#ifdef OPENGL
		result = ImGui_ImplWin32_Init(*static_cast<OGLWin32Window*>(window)->getHWND());
		const char* glsl_version = "#version 460";
		result = ImGui_ImplOpenGL3_Init(glsl_version);
#endif
	}

	~GUISystem()
	{
#ifdef OPENGL
		ImGui_ImplOpenGL3_Shutdown();
#endif
#ifdef D3D11
		ImGui_ImplDX11_Shutdown();
#endif
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void begin()
	{
#ifdef OPENGL
		ImGui_ImplOpenGL3_NewFrame();
#endif
#ifdef D3D11
		ImGui_ImplDX11_NewFrame();
#endif
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
		ImGui::Begin("DockSpace Demo", &open, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	void update(double dt)
	{
		begin();

		if (!Hollow::ProjectSettings::instance()->isProjectLoaded) {
			drawMainWindow();
		} else {
			drawProjectLoaderGui();
		}
		
		end();
	}

	void drawProjectLoaderGui()
	{
		ImGui::Begin("ProjectLoader");
		ImGui::Text("You need to load project");
		if (ImGui::Button("Select project file")) {
			filename = Hollow::FileSystem::OpenFile("");
			if (filename.size()) {

			}
		}
		ImGui::End();
	}

	void drawMainWindow()
	{
		ImGui::Begin("Main");
		if (ImGui::Button("Clear")) {
			Hollow::EntityManager::instance()->clear();
			Hollow::ComponentManager::instance()->clear();
		}
		if (ImGui::TreeNode("Shaders")) {
			auto& shaders = ShaderManager::instance()->shaders;
			for (auto& it : shaders) {
				if (ImGui::TreeNode(it.first.c_str())) {
					std::string buttonName = "Reload##" + it.first;
					if (ImGui::Button(buttonName.c_str())) {
						ShaderManager::instance()->reloadShader(it.second);
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::Button("Delete all textures")) {
			TextureManager::instance()->RemoveAll();
		}

		int counter = 0;
		if (ImGui::Button("Add enitity")) {
			GameObject* entity = EntityManager::instance()->createEntity<GameObject>();
			entity->addComponent<TransformComponent, Vector3&&, Vector3&&, Vector3&&>
				(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		}

		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			if (ImGui::Selectable(("Entity" + std::to_string(counter++)).c_str())) {
				selectedGameObject = &entity;
			}
		}
		ImGui::End();

		ImGui::Begin("Inspector");
		ImGui::Text("Renderable component");
		if (selectedGameObject != nullptr) {
			if (selectedGameObject->hasComponent<GLTFRenderable>()) {
				GLTFRenderable* renderableComponent = selectedGameObject->getComponent<GLTFRenderable>();
				std::vector<GLTFRenderableObject*>& renderables = selectedGameObject->getComponent<GLTFRenderable>()->renderables;
				drawNodes(renderableComponent->rootNode, renderables);
			}
		}

		ImGui::Text("Transform component");
		if (selectedGameObject != nullptr) {
			if (selectedGameObject->hasComponent<TransformComponent>()) {
				TransformComponent* component = selectedGameObject->getComponent<TransformComponent>();
				ImGui::DragFloat3("Position", (float*)& component->position, 0.1f, -100.0f, 100.0f);
				ImGui::DragFloat3("Rotation", (float*)& component->rotation, 0.1f, -100.0f, 100.0f);
				ImGui::DragFloat3("Scale", (float*)& component->scale, 0.1f, -100.0f, 100.0f);
			}
		}
		ImGui::End();

		ImGui::Begin("Material properties");
		if (selectedMaterial != nullptr) {
			ImGui::DragFloat4("Base color", (float*)& selectedMaterial->materialData.color, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &selectedMaterial->materialData.metallicFactor, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("Emissive", &selectedMaterial->materialData.emissiveFactor, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("Roughness", &selectedMaterial->materialData.roughnessFactor, 0.001f, 0.0f, 1.0f);

			ImGui::Text("Diffuse texture");
			if (selectedMaterial->diffuseTexture != nullptr) {
#ifdef OPENGL
				ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->diffuseTexture)->textureId, ImVec2(100, 100));
#endif
#ifdef D3D11
				ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->diffuseTexture)->m_TextureShaderResource, ImVec2(100, 100));
#endif
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
#ifdef OPENGL
				ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->normalTexture)->textureId, ImVec2(100, 100));
#endif
#ifdef D3D11
				ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->normalTexture)->m_TextureShaderResource, ImVec2(100, 100));
#endif
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
#ifdef OPENGL
				ImGui::Image((void*)static_cast<OGLTexture*>(selectedMaterial->specularTexture)->textureId, ImVec2(100, 100));
#endif
#ifdef D3D11
				ImGui::Image(static_cast<D3D11Texture*>(selectedMaterial->specularTexture)->m_TextureShaderResource, ImVec2(100, 100));
#endif
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
		ImGui::Begin("Scene");
#ifdef OPENGL
		ImGui::Image((void*)static_cast<OGLRenderTarget*>(renderSystem->debug)->texture, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()), ImVec2(0, 1), ImVec2(1, 0));
#endif
#ifdef D3D11
		ImGui::Image(static_cast<D3D11RenderTarget*>(renderSystem->debug)->GetShaderResourceView(), ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
#endif
		ImGui::End();
	}

	void end()
	{
		ImGui::End();

		ImGui::Render();
#ifdef OPENGL
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
#ifdef D3D11 
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void drawNodes(Hollow::GLTF::Node* node, std::vector<GLTFRenderableObject*>& renderables) {
		if (ImGui::TreeNode(node->name.c_str())) {
			if (node->mesh >= 0) {
				if (ImGui::Selectable(renderables[node->mesh]->name.c_str())) {
					selectedMaterial = renderables[node->mesh]->material;
				}
			}
			
			for (auto& it : node->childrens) {
				drawNodes(it, renderables);
			}
			ImGui::TreePop();
		}
	}

	void onChangePickedMesh(Hollow::IEvent* event)
	{
		ChangeSelectedEntity* changeEvent = reinterpret_cast<ChangeSelectedEntity*>(event);
		if (selectedGameObject != nullptr && selectedGameObject->hasComponent<GLTFRenderable>()) {
			GLTFRenderable* renderable = selectedGameObject->getComponent<GLTFRenderable>();
			std::vector<GLTFRenderableObject*>& renderables = renderable->renderables;
			if (changeEvent->pickedId < renderables.size()) {
				selectedMaterial = renderables[changeEvent->pickedId]->material;
			}
		}
	}
};