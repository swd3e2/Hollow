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

	/*ForwardRenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;*/

	//MoveSystem moveSystem(camera);

	//SystemManager::instance()->AddSystem(&renderPass);
	//SystemManager::instance()->AddSystem(&moveSystem);

	//GUISystem* gui = new GUISystem(window, renderer);
	//gui->rendererTab.renderSystem = &renderPass;

	/*Terrain* terrain = Hollow::EntityManager::instance()->create<Terrain>();
	terrain->addComponent<TerrainData>();*/

	//ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");

	Hollow::PIPELINE_STATE_DESC pipelineDesc = { 0 };
	pipelineDesc.vertexShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::VERTEX, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/vertex/test.glsl"), "main" });
	pipelineDesc.pixelShader = Hollow::Shader::create({ Hollow::SHADER_TYPE::PIXEL, Hollow::FileSystem::getFileContent("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/OGL/pixel/test.glsl"), "main" });
	PipelineState* pipeline = Hollow::PipelineState::create(pipelineDesc);

	std::vector<Hollow::Vertex> vertices;
	vertices.push_back({ -1.0f, -1.0f, 0.0f });
	vertices.push_back({ 1.0f, -1.0f, 0.0f });
	vertices.push_back({ 0.0f,  1.0f, 0.0f });
	VertexBuffer* vb = VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Vertex) });

	unsigned int indices[] = { 0, 1, 2 };
	IndexBuffer* ib = IndexBuffer::create({ indices, 3, INDEX_FORMAT::UINT });

	//glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, 1920, 1080);

	bool res = false;
	HWND hwnd = *static_cast<OGLWin32Window*>(Window::instance())->getHWND();

	float clearColor[] = { 1.0f, 1.0f ,0.0f ,1.0f };

	while (!window->isClosed()) {
		core.PreUpdate();

		window->ProcessMessage();
		camera->Update(core.dt);
		core.Update();

		{
			glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderer->SetPipelineState(pipeline);

			renderer->SetVertexBuffer(vb);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glEnableVertexAttribArray(6);
			renderer->SetIndexBuffer(ib);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44);
			glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (const GLvoid*)56);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)72);

			renderer->DrawIndexed(3);

			SwapBuffers(GetDC(hwnd));
		}

		//gui->update(core.dt);

		core.PostUpdate();
	}
	
	return 0;
}