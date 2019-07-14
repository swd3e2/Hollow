#pragma once
#include "Hollow/Graphics/GUI/ImGui/imgui.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_win32.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_dx11.h"
#include "Hollow/Graphics/GUI/ImGui/imgui_impl_opengl3.h"
#include "Hollow/Graphics/Base/Window.h"
#include <Hollow/Events/IEventListener.h>
#include <Hollow/Events/EventSystem.h>
#include "Sandbox/ProjectSettings.h"
#include "LogTab.h"
#include "HierarchyTab.h"
#include "InspectorTab.h"
#include "MaterialTab.h"
#include "RendererTab.h"
#include "Hollow/Common/Logger.h"
#include "Hollow/Core/DelayedTaskManager.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Context.h"

using namespace Hollow;

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

	GUI::LogTab logTab;
	GUI::HierarchyTab hierarchyTab;
	GUI::InspectorTab inspectorTab;
	GUI::MaterialTab materialTab;
	GUI::RendererTab rendererTab;

	GUISystem(Window* window, RenderApi* renderer)
	{
		Hollow::EventSystem::instance()->addEventListener(this, &GUISystem::onChangePickedMesh, ChangeSelectedEntity::getStaticEventId());

		bool result = true;
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\dev\\Hollow Engine\\Sandbox\\Sandbox\\Resources\\Roboto-Medium.ttf", 16.0f);

		ImGuiStyle* style = &ImGui::GetStyle();

		style->Colors[ImGuiCol_Text]					= ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled]			= ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg]				= ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg]			= ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style->Colors[ImGuiCol_PopupBg]					= ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style->Colors[ImGuiCol_Border]					= ImVec4(0.30f, 0.30f, 0.30f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow]			= ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg]					= ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive]			= ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg]					= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive]			= ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg]				= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg]				= ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.83f, 0.83f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive]		= ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark]				= ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab]				= ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button]					= ImVec4(0.78f, 0.48f, 0.18f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered]			= ImVec4(0.72f, 0.38f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive]			= ImVec4(0.72f, 0.38f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_Header]					= ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered]			= ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive]			= ImVec4(0.06f, 0.05f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines]				= ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered]		= ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram]			= ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening]	= ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
		style->Colors[ImGuiCol_Tab]						= ImVec4(0.98f, 0.58f, 0.22f, 0.73f);
		style->Colors[ImGuiCol_TabActive]				= ImVec4(0.98f, 0.58f, 0.22f, 0.73f);
		style->Colors[ImGuiCol_TabUnfocused]			= ImVec4(0.98f, 0.58f, 0.22f, 0.73f);
		style->Colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.98f, 0.58f, 0.22f, 0.73f);
		style->Colors[ImGuiCol_TabHovered]				= ImVec4(0.98f, 0.58f, 0.22f, 0.73f);
		style->Colors[ImGuiCol_Separator]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_SeparatorActive]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

		if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplWin32_Init(*static_cast<D3D11Win32Window*>(window)->getHWND());
			D3D11Context& context = static_cast<D3D11RenderApi*>(renderer)->getContext();
			ImGui_ImplDX11_Init(context.getDevice(), context.getDeviceContext());
		} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			result = ImGui_ImplWin32_Init(*static_cast<OGLWin32Window*>(window)->getHWND());
			const char* glsl_version = "#version 460";
			result = ImGui_ImplOpenGL3_Init(glsl_version);
		}
		
		strcpy_s(projectFolder, 4, "C:/");
		strcpy_s(projectName, 8, "Project");
	}

	~GUISystem()
	{
		if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_Shutdown();
		} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
			ImGui_ImplOpenGL3_Shutdown();
		}
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void begin()
	{
		if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_NewFrame();
		} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
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

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	}

	void update(double dt)
	{
		begin();
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Load")) {
					filename = Hollow::FileSystem::OpenFile("");
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
				std::string tempString = Hollow::FileSystem::OpenFolder().c_str();
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

		if (ProjectSettings::instance()->isProjectLoaded) {
			rendererTab.Draw();
			hierarchyTab.Draw();
			inspectorTab.Draw(hierarchyTab.selectedGameObject);
			materialTab.Draw(inspectorTab.selectedMaterial);
			logTab.Draw();
		}
		
		end();
	}

	void end()
	{
		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::Render();
		if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::DirectX) {
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		} else if (ProjectSettings::instance()->getRendererType() == Hollow::RendererType::OpenGL) {
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
		/*ChangeSelectedEntity* changeEvent = reinterpret_cast<ChangeSelectedEntity*>(event);
		if (selectedGameObject != nullptr && selectedGameObject->hasComponent<RenderableComponent>()) {
			RenderableComponent* renderable = selectedGameObject->getComponent<RenderableComponent>();
			std::vector<RenderableObject>& renderables = renderable->renderables;
			if (changeEvent->pickedId < renderables.size()) {
				selectedMaterial = &renderable->materials[renderables[changeEvent->pickedId].id];
			}
		}*/
	}
};