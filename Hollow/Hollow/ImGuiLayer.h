#pragma once
#include <vector>
#include "Layer.h"
#include "Hollow/Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/Window/Win32/Win32Window.h"
#include "Hollow/Graphics/DirectX/D3DRenderable.h"
#include "Hollow/Resources/TextureManager.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Resources/ShaderManager.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Graphics/ForwardRenderPass.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/PointLightComponent.h"
#include "Hollow/Common/SaveHelper.h"
#include "Hollow/Platform.h"
#include <fstream>

enum MenuAction
{
	MENU_ITEM_FILESYSTEM
};

class ImGuiLayer : public Layer
{
public:
	IEntity* selected;
	RenderableObject* selectedObject;
	float currentPosition[3] = {};
	float currentRotation[3] = {};
	float currentScale[3] = {};
	float ambientLightColor[3] = {};
	float ambientLightDirection[3] = {};

	float pointLightColor[3] = {};
	float pointLightPosition[3] = {};
	float pointLightStrenght = 0;
	float pointLightSpecularPower = 0;

	float shadowMapPosition[3] = {};

	bool vSync = true;

	int menuItemSelected = -1;

	D3DRenderer* renderer;
	ShaderManager* shaderManager;
	
	// File system stuff
	Hollow::FileSystem fs;
	std::vector<std::string>* v = nullptr;

	std::string currentFilePath = "C:\\";
	std::string filePath;
	std::string selectedFile;

	bool* p_open;

	std::vector<IRenderable*>* list;
	const char* current_item = NULL;
	Win32Window* window;
	MyCamera* mainCamera;
	ShadowMap* shadowMap;
	ForwardRenderPass* renderPass;

public:
	ImGuiLayer(D3DRenderer* renderer, ForwardRenderPass* renderPass, std::vector<IRenderable*>* list, MyCamera* mainCamera) :
		renderer(renderer), renderPass(renderPass), list(list), mainCamera(mainCamera), shadowMap(shadowMap)
	{
		bool result = true;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		result = ImGui_ImplWin32_Init(*((Win32Window*)Win32Window::instance())->getHWND());
		if (!result) HW_ERROR("Can't init imgui_win32");
		result = ImGui_ImplDX11_Init(renderer->getDevice(), renderer->getDeviceContext());
		if (!result) HW_ERROR("Can't init imgui_dx11");
		ImGui::StyleColorsDark();
		p_open = new bool;
		*p_open = false;

		shaderManager = ShaderManager::instance();
		v = fs.read_next_directory("C");
	}

	~ImGuiLayer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	virtual void PreUpdate(float dt) override
	{
		ImGui_ImplDX11_NewFrame();
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
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruDockspace;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) {
					SaveHelper::instance()->save();
				}
				if (ImGui::MenuItem("Load")) {
					SaveHelper::instance()->load();
				}
				if (ImGui::MenuItem("Exit")) {
					window->setIsClosed(true);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::MenuItem("Load texture")) {
					menuItemSelected = MenuAction::MENU_ITEM_FILESYSTEM;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (menuItemSelected == MenuAction::MENU_ITEM_FILESYSTEM)
		{
			ImGui::OpenPopup("Filesystem");
		}

		if (ImGui::BeginPopupModal("Filesystem"))
		{
			ImGui::BeginChild("Child1", ImVec2(400, 360), false, window_flags);
			for (auto& it : *v) {
				if (it.find(".png") != -1 || it.find(".jpeg") != -1 || it.find(".jpg") != -1 || it.find(".dds") != -1 || it.find(".tga") != -1) {
					if (ImGui::Selectable(it.c_str(), it.c_str() == selectedFile.c_str())) {
						selectedFile = it;
						filePath = fs.get_current_file_path();
						filePath.append(it);
					}
				} else {
					if (ImGui::Selectable(it.c_str(), false)) {
						v = fs.read_next_directory(it);
						currentFilePath = fs.get_current_file_path();
						break;
					}
				}
			}
			ImGui::EndChild();

			if (ImGui::Button("Add texture"))
			{
				TextureManager::instance()->CreateTexture(filePath, false);
				menuItemSelected = -1;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	virtual void Update(float dt) override
	{
		ImGui::Begin("Renderer");
		ImGui::Text("Past frame time %f", dt);
		if (ImGui::Checkbox("VSync", &vSync)) {
			renderer->toggleVSync();
		}
		static float fov = 75.0f;
		if (ImGui::DragFloat("FOV", &fov)) {
			renderer->getCamera()->SetProjectionValues(fov, static_cast<float>(1920) / static_cast<float>(1080), 0.1f, 10000.0f);
		}
		if (ImGui::Checkbox("Toggle camera", p_open)) {
			if (mainCamera->mainCamera == true) {
				renderPass->shadowMap->camera.mainCamera = true;
				mainCamera->mainCamera = false;
			}
			else {
				renderPass->shadowMap->camera.mainCamera = false;
				mainCamera->mainCamera = true;
			}
		}
		ImGui::DragFloat("Bias", &renderPass->shadowMap->bias, 0.001f, 0.0f, 1.0f);
		ImGui::Image(renderPass->m_ShadowDepthStencil->GetDepthStencilResource(), ImVec2(200, 200));
		ImGui::DragFloat3("Position", (float*)&mainCamera->position, 0.01f);
		ImGui::DragFloat3("Rotation", (float*)& mainCamera->rotation, 0.01f);
		ImGui::DragFloat3("Target", (float*)& mainCamera->camTarget, 0.01f);
		ImGui::Spacing();
		ImGui::Text("My look at");

		Matrix4 transposeView = mainCamera->GetViewMatrix();
		for (int i = 0; i < 16; i++)
		{
			ImGui::Text(std::to_string(transposeView.m[i]).c_str());
			if (i == 0 || (i + 1) % 4 != 0) {
				ImGui::SameLine();
			}
		}
		XMVECTOR EyePosition{ mainCamera->position.x, mainCamera->position.y, mainCamera->position.z, mainCamera->position.w };
		XMVECTOR FocusPosition{ mainCamera->camTarget.x, mainCamera->camTarget.y, mainCamera->camTarget.z, mainCamera->camTarget.w };
		XMVECTOR EyeDirection = XMVectorSubtract(FocusPosition, EyePosition);

		XMMATRIX m = XMMatrixLookAtLH(
			EyePosition,
			FocusPosition,
			{ 0.0f, 1.0f, 0.0f, 0.0f }
		);
		ImGui::Spacing();
		ImGui::Text("Directx look at");
		for (int i = 0; i < 16; i++)
		{
			ImGui::Text(std::to_string(*((float*)& m + i)).c_str());
			if (i == 0 || (i + 1) % 4 != 0) {
				ImGui::SameLine();
			}
		}
		ImGui::End();

		ImGui::Begin("Lights");
		if (EntityManager::instance()->getContainer<Light>()->entityList.getSize()) {
			Light* lightEntity = EntityManager::instance()->getContainer<Light>()->entityList[0];
			if (lightEntity->hasComponent<PointLightComponent>()) {
				PointLightComponent* lightComponent = lightEntity->getComponent<PointLightComponent>();
				ImGui::Text("Ambient");
				ImGui::Spacing();

				ImGui::Text("Ambient Light color");
				ImGui::ColorEdit3("###", ambientLightColor);
				ImGui::Spacing();

				ImGui::Text("Ambient Light direction");
				ImGui::DragFloat3("###", ambientLightDirection, 0.01f, -1.0f, 1.0f);
				ImGui::Spacing();

				ImGui::Text("Point");
				ImGui::ColorEdit3("Point Light color", pointLightColor);
				ImGui::DragFloat3("Point Light position", pointLightPosition, 0.1f, -30.0f, 30.0f);
				ImGui::DragFloat3("Point Light attenuation", lightComponent->light.data.attenuation, 0.01f, -30.0f, 30.0f);
				ImGui::DragFloat("Range", &lightComponent->light.data.range, 0.1f, -30.0f, 30.0f);

				lightComponent->light.setColor(pointLightColor);
				lightComponent->light.setPosition(pointLightPosition);
			}
		}
		
		ImGui::End();

		ImGui::Begin("Scene objects");
		for (auto& it : EntityManager::instance()->getContainer<GameObject>()->entityList)
		{
			std::string	name = "Entity";
			name += std::to_string(it.getId());
			if (ImGui::Selectable(name.c_str()))
			{
				selected = &it;
			}
		}
		ImGui::End();

		ImGui::Begin("Object properties");
		if (selected != nullptr)
		{
			if (selected->hasComponent<TransformComponent>())
			{
				TransformComponent* transform = selected->getComponent<TransformComponent>();
				currentPosition[0] = transform->position.x;
				currentPosition[1] = transform->position.y;
				currentPosition[2] = transform->position.z;

				currentRotation[0] = transform->rotation.x;
				currentRotation[1] = transform->rotation.y;
				currentRotation[2] = transform->rotation.z;

				currentScale[0] = transform->scale.x;
				currentScale[1] = transform->scale.y;
				currentScale[2] = transform->scale.z;

				ImGui::Text("Position: x | y | z");
				ImGui::DragFloat3("##position", currentPosition, 0.1f);
				ImGui::Text("Rotation");
				ImGui::DragFloat3("##rotation", currentRotation, 0.1f);
				ImGui::Text("Scale: x | y | z");
				ImGui::DragFloat3("##scale", currentScale, 0.1f);

				transform->setPosition(currentPosition[0], currentPosition[1], currentPosition[2]);
				transform->setRotation(currentRotation[0], currentRotation[1], currentRotation[2]);
				transform->setScale(currentScale[0], currentScale[1], currentScale[2]);

				ImGui::Spacing();
			}

			if (selected->hasComponent<RenderableComponent>()) {
				RenderableComponent* renderable = selected->getComponent<RenderableComponent>();
				for (auto& it : renderable->renderable.renderableObjects)
				{
					if (ImGui::Selectable(it->name.c_str()))
					{
						selectedObject = it;
					}
				}
			}
		}
		ImGui::End();

		ImGui::Begin("Resources");
		if (ImGui::CollapsingHeader("Textures")) {
			for (auto& it : *TextureManager::instance()->getTexuresList())
			{
				ImGui::Image(it.second->m_TextureShaderResource, ImVec2(100, 100));
				ImGui::Text(it.first.c_str());
			}
		}
		ImGui::End();

		ImGui::Begin("Material properties");
		if (selectedObject != nullptr)
		{
			ImGui::Text("Material properties\n\n");

			ImGui::DragFloat("Ns", &selectedObject->material->materialData.Ns, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat3("Kd", selectedObject->material->materialData.Ka, 0.01, -1.0f, 1.0f);
			ImGui::DragFloat3("Kd", selectedObject->material->materialData.Kd, 0.01, -1.0f, 1.0f);
			ImGui::DragFloat3("Ks", selectedObject->material->materialData.Ks, 0.01, -1.0f, 1.0f);
			ImGui::DragFloat3("Ke", selectedObject->material->materialData.Ke, 0.01, -1.0f, 1.0f);

			ImGui::Text("Vertex shader");
			if (ImGui::BeginCombo("##materialVertexShaderCombo", "")) // The second parameter is the label previewed before opening the combo.
			{
				for (auto& it : *shaderManager->getVertexShaderList())
				{
					if (ImGui::Selectable(it.first.c_str(), selectedObject->material->vertexShader == it.second)) {
						selectedObject->material->vertexShader = it.second;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Pixel shader");
			if (ImGui::BeginCombo("##materialPixelShaderCombo", "")) 
			{
				for (auto& it : *shaderManager->getPixelShaderList())
				{
					if (ImGui::Selectable(it.first.c_str(), selectedObject->material->pixelShader == it.second)) {
						selectedObject->material->pixelShader = it.second;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Diffuese texture");
			if (ImGui::BeginCombo("##materialDiffuesTextureCombo", ""))
			{
				for (auto& it : *TextureManager::instance()->getTexuresList())
				{
					if (ImGui::Selectable(it.first.c_str(), &(*selectedObject->material->diffuseTexture) == it.second)) {
						selectedObject->material->diffuseTexture = std::make_shared<D3DTexture>(*it.second);
						selectedObject->material->materialData.hasDiffuseTexture = true;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Normal texture");
			if (ImGui::BeginCombo("##materialNormalTextureCombo", ""))
			{
				for (auto& it : *TextureManager::instance()->getTexuresList())
				{
					if (ImGui::Selectable(it.first.c_str(), &(*selectedObject->material->normalTexture) == it.second)) {
						selectedObject->material->normalTexture = std::make_shared<D3DTexture>(*it.second);
						selectedObject->material->materialData.hasNormalMap = true;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Text("Specular texture");
			if (ImGui::BeginCombo("##materialSpecularTextureCombo", ""))
			{
				for (auto& it : *TextureManager::instance()->getTexuresList())
				{
					if (ImGui::Selectable(it.first.c_str(), &(*selectedObject->material->specularTexture) == it.second)) {
						selectedObject->material->specularTexture = std::make_shared<D3DTexture>(*it.second);
						selectedObject->material->materialData.hasSpecularMap = true;
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}


	virtual void PostUpdate(float dt) override
	{
		// End docking viewport
		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
};