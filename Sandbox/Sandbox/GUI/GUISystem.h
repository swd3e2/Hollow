#pragma once
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_opengl3.h"
#include "Hollow/Graphics/Window.h"
#include <Hollow/Events/IEventListener.h>
#include <Hollow/Events/EventSystem.h>
#include "Sandbox/ProjectSettings.h"
#include "HierarchyTab.h"
#include "RendererTab.h"
#include "Hollow/Core/DelayedTaskManager.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Context.h"

enum DrawTypes {
	Debug = 0,
	Default = 1
};

class GUISystem : public Hollow::IEventListener
{
public:
	bool open = NULL;
	std::string filename = "";
	int drawMode = 0;
	bool openProjectCreationPopup = false;

	char* projectFolder = new char[100]{ 0 };
	char* projectName = new char[100]{ 0 };

	GUI::HierarchyTab hierarchyTab;
	GUI::RendererTab rendererTab;

	GUISystem(Hollow::Window* window, Hollow::RenderApi* renderer)
	{
		Hollow::EventSystem::instance()->addEventListener(this, &GUISystem::onChangePickedMesh, ChangeSelectedEntity::staticGetId());

		bool result = true;
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("E:\\Hollow\\Sandbox\\Sandbox\\Resources\\Roboto-Medium.ttf", 16.0f);

		if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplWin32_Init(static_cast<Hollow::D3D11Win32Window*>(window)->getHWND());
			Hollow::D3D11Context& context = static_cast<Hollow::D3D11RenderApi*>(renderer)->getContext();
			ImGui_ImplDX11_Init(context.getDevice(), context.getDeviceContext());
		} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			result = ImGui_ImplWin32_Init(static_cast<Hollow::OGLWin32Window*>(window)->getHWND());
			const char* glsl_version = "#version 460";
			result = ImGui_ImplOpenGL3_Init(glsl_version);
		}

		strcpy_s(projectFolder, 4, "C:/");
		strcpy_s(projectName, 8, "Project");
	}

	~GUISystem()
	{
		
	}

	void shutdown()
	{
		if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_Shutdown();
		}
		else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			ImGui_ImplOpenGL3_Shutdown();
		}
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void begin()
	{
		if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_NewFrame();
		} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			ImGui_ImplOpenGL3_NewFrame();
		}
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

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0.0f);
	}

	void update(double dt)
	{
		begin();
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Load")) {
					filename = Hollow::FileSystem::openFile("");
					if (filename.size()) {
						ProjectSettings::instance()->load(filename);
					}
				}
				if (ImGui::MenuItem("Create")) {
					openProjectCreationPopup = true;
				}
				if (ImGui::MenuItem("Save")) {
					ProjectSettings::instance()->save();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Exit")) {
				Hollow::Window::instance()->setIsClosed(true);
			}
			ImGui::EndMenuBar();
		}
		if (openProjectCreationPopup) {
			ImGui::OpenPopup("Project creation");
		}

		if (ImGui::BeginPopupModal("Project creation"))
		{
			ImGui::InputText("##project_name", projectName, 100);
			ImGui::InputText("##project_path", projectFolder, 100);
			if (ImGui::Button("Select folder")) {
				std::string tempString = Hollow::FileSystem::openFolder().c_str();
				strcpy_s(projectFolder, tempString.size() + 1, tempString.c_str());
			}
			if (ImGui::Button("Create")) {
				ProjectSettings::instance()->create(projectFolder, projectName);
				ImGui::CloseCurrentPopup();
				openProjectCreationPopup = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
				openProjectCreationPopup = false;
			}
			ImGui::EndPopup();
		}

		//if (ProjectSettings::instance()->isProjectLoaded) {
			rendererTab.Draw(dt);
			hierarchyTab.Draw();
		//}
		
		end();
	}

	void end()
	{
		ImGui::PopStyleVar(2);
		ImGui::End();

		ImGui::Render();
		if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		} else if (Hollow::RenderApi::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void onChangePickedMesh(Hollow::IEvent* event)
	{
		ChangeSelectedEntity* changeEvent = reinterpret_cast<ChangeSelectedEntity*>(event);
		hierarchyTab.selectedGameObject = (GameObject*)Hollow::EntityManager::instance()->get(changeEvent->pickedId);
		hierarchyTab.selectedLight = nullptr;
		hierarchyTab.selectedTerrain = nullptr;
	}
};