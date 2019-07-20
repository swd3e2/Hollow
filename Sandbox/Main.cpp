#pragma once
#include <Hollow/Core.h>
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Systems/ForwardRenderSystem.h"
#include "Sandbox/GUI/GUISystem.h"
#include "Sandbox/Systems/MoveSystem.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TerrainData.h"
#include "Hollow/Graphics/Base/InputLayout.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	
	Hollow::Core core(Hollow::RendererType::OpenGL);

	Hollow::Window* window = core.windowManager.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, Hollow::WindowType::Bordered);
	Hollow::RenderApi* renderer = core.renderApiManager.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	ProjectSettings::startUp<ProjectSettings>(Hollow::RendererType::OpenGL);

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->SetProjectionValues(80.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

	ForwardRenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	MoveSystem moveSystem(camera);

	SystemManager::instance()->AddSystem(&renderPass);
	SystemManager::instance()->AddSystem(&moveSystem);

	GUISystem* gui = new GUISystem(window, renderer);
	gui->rendererTab.renderSystem = &renderPass;

	Terrain* terrain = Hollow::EntityManager::instance()->create<Terrain>();
	terrain->addComponent<TerrainData>();

	ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");

	//Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
	//pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/test.glsl"), "main" });
	//pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/test.glsl"), "main" });
	//PipelineState* pipeline = Hollow::PipelineState::create(pipelineDesc);

	//std::vector<Hollow::Vertex> vertices;
	//vertices.push_back(Vertex(1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	//vertices.push_back(Vertex(1.0f, -1.0f, 0.0f, 1.0f, 1.0f));
	//vertices.push_back(Vertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	//vertices.push_back(Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f));
	//VertexBuffer* vb = VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Vertex) });

	//Hollow::INPUT_LAYOUT_DESC layoutDesc = {
	//	{ Hollow::INPUT_DATA_TYPE::Float3, "POSITION" }, // pos
	//	{ Hollow::INPUT_DATA_TYPE::Float2, "TEXCOORD" }, // texcoord
	//	{ Hollow::INPUT_DATA_TYPE::Float3, "NORMAL" }, // normal
	//	{ Hollow::INPUT_DATA_TYPE::Float3, "TANGENT" }, // tangent
	//	{ Hollow::INPUT_DATA_TYPE::Float3, "BITANGENT" }, // bitangent 
	//	{ Hollow::INPUT_DATA_TYPE::Int4, "BONE" },
	//	{ Hollow::INPUT_DATA_TYPE::Float4, "WEIGHT" }
	//};

	//InputLayout* layout = InputLayout::create(layoutDesc);
	//renderer->setInputLayout(layout);

	//unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
	//IndexBuffer* ib = IndexBuffer::create({ indices, 3, INDEX_FORMAT::UINT });

	//renderer->setViewport(0, 0, 1920, 1080);

	//HWND hwnd = *static_cast<OGLWin32Window*>(Window::instance())->getHWND();

	//float clearColor[] = { 1.0f, 1.0f ,0.0f ,1.0f };

	while (!window->isClosed()) {
		core.PreUpdate();

		window->ProcessMessage();
		camera->Update(core.dt);
		core.Update();

		/*{
			glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderer->setPipelineState(pipeline);

			renderer->setVertexBuffer(vb);
			renderer->setIndexBuffer(ib);
			renderer->drawIndexed(6);
			SwapBuffers(GetDC(hwnd));
		}*/

		gui->update(core.dt);

		core.PostUpdate();
	}
	
	return 0;
}