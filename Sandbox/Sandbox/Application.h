#pragma once
#include "Sandbox/MyWindow.h"
#include "Entities/GameObject.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include <Hollow/Engine.h>
#include "Hollow/Components/MeshComponent.h"
#include "Components/MoveComponent.h"
#include "Components/PositionComponent.h"
#include "Systems/InterfaceSystem/InterfaceSystem.h"
#include "Systems/MoveSystem.h"
#include "Systems/RenderSystem/RenderSystem.h"
#include "Hollow/Input/InputManager.h"
#include "DirectXMath.h"
#include "d3d11.h"
#include <random>
#include "Hollow/Resources/Sound.h"
#include "Hollow/Containers/vector.h"
#include <thread>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

using namespace DirectX;

class Application
{
private:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };

	MyWindow				m_Window;
	Hollow::Engine			engine;
	HWND*					m_HWND;
	RenderSystem*           m_RenderSystem;
	MoveSystem*				m_MoveSystem;
	InterfaceSystem*		m_InterfaceSystem;

	std::vector<GameObject*> gameObjects;
private:
	void InitScene()
	{
		Hollow::Sound* music2 = engine.GetReosourceManager()->CreateSoundResource("Sandbox/Resources/Sounds/2.wav");
		//music2->Play();

		std::vector<SimpleVertex> vertices;
		vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });

		vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) });

		std::vector<unsigned int> vindices;
		unsigned int * indices = new unsigned int[36]{
				2, 1, 0,
				2, 0, 3,
				2, 5, 1,
				5, 7, 1,
				4, 2, 3,
				4, 5, 2,
				4, 3, 0,
				4, 0, 6,
				1, 6, 0,
				1, 7, 6,
				5, 6, 7,
				5, 4, 6
		};

		for (int i = 0; i < 36; i++)
			vindices.push_back(indices[i]);

		for (int i = 0; i < 100; i++) {
			GameObject * object = engine.GetEntityManager()->CreateEntity<GameObject>();

		}
		/*GameObject * object = engine.GetEntityManager()->CreateEntity<GameObject>();
		object->AddComponent<MeshComponent, Hollow::Mesh*>(
			this->engine.GetReosourceManager()->CreateMeshResource(
				this->m_RenderSystem->GetDevice(),
				this->m_RenderSystem->GetDeviceContext(),
				"Sandbox/Resources/Meshes/sponza.obj", 
				"Sandbox/Resources/Meshes/")
			);

		object->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({1.0f, -3.0f, 1.0f}, { 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 0.0f });
		gameObjects.push_back(object);

		GameObject * object2 = engine.GetEntityManager()->CreateEntity<GameObject>();
		object2->AddComponent<MeshComponent, Hollow::Mesh*>(
			this->engine.GetReosourceManager()->CreateMeshResource(
				this->m_RenderSystem->GetDevice(),
				this->m_RenderSystem->GetDeviceContext(),
				"Sandbox/Resources/Meshes/sponza.obj",
				"Sandbox/Resources/Meshes/")
			);

		object2->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ 10000.0f, -3.0f, 1.0f }, { 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 0.0f });
		gameObjects.push_back(object2);*/
	}
public:
	Application(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		this->m_HWND = this->m_Window.getHWND();
		this->m_RenderSystem = new RenderSystem(this->m_HWND, SCREEN_WIDTH, SCREEN_HEIGHT);
		this->m_InterfaceSystem = new InterfaceSystem(this->m_HWND, this->m_RenderSystem->GetDevice(), this->m_RenderSystem->GetDeviceContext(), engine.GetEntityManager(), engine.GetComponentManager());
		this->m_MoveSystem = new MoveSystem();
		this->InitScene();
	}

	void Run() {
		while (m_Window.ProcessMessage())
		{
			engine.GetTimer()->Tick(DELTA_TIME_STEP);
			this->m_RenderSystem->PreUpdate(DELTA_TIME_STEP);
			this->m_MoveSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_RenderSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_InterfaceSystem->Update(DELTA_TIME_STEP);
			this->m_RenderSystem->PostUpdate(DELTA_TIME_STEP);

			engine.GetEventHandler()->DispatchEvents();
			Hollow::InputManager::Clear();
		}
	}
};