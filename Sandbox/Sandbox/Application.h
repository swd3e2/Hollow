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
#include "Hollow/Graphics/Renderer.h"
#include "Components/SelectComponent.h"
#include "Sandbox/Components/CollisionComponent.h"
#include "Sandbox/Systems/CollisionSystem.h"
#include <thread>
#define SCREEN_WIDTH 1800
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
	CollisionSystem*		m_CollisionSystem;
private:
	void InitScene()
	{
		Hollow::Sound* music2 = engine.GetReosourceManager()->CreateSoundResource("Sandbox/Resources/Sounds/2.wav");
		//music2->Play();
		
		GameObject * object = engine.GetEntityManager()->CreateEntity<GameObject>();
		object->AddComponent<MeshComponent, Hollow::Mesh*>(
			this->engine.GetReosourceManager()->CreateMeshResource(
			((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(),
				((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDeviceContext(),
				"Sandbox/Resources/Meshes/g.obj",
				"Sandbox/Resources/Meshes/",
				false)
			);

		object->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });
		object->AddComponent<SelectComponent, bool>(false);
		object->AddComponent<CollisionComponent>();
		object->AddComponent<MoveComponent>();

		object = engine.GetEntityManager()->CreateEntity<GameObject>();
		object->AddComponent<MeshComponent, Hollow::Mesh*>(
			this->engine.GetReosourceManager()->CreateMeshResource(
			((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(),
				((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDeviceContext(),
				"Sandbox/Resources/Meshes/hill.obj",
				"Sandbox/Resources/Meshes/",
				false)
			);

		object->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ 0.0f, -3.110f, 0.0f }, { 15.0f, 15.0f, 15.0f }, { 0.0f, 0.0f, 0.0f });
		object->AddComponent<SelectComponent, bool>(false);
	}

	void InitResources()
	{
		D3D11_INPUT_ELEMENT_DESC bxlayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,							  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		UINT numElements = ARRAYSIZE(bxlayout);

		ResourceManager::Get()->CreatePixelShader(((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(), "Sandbox/Resources/Shaders/ps.hlsl", "simple");
		ResourceManager::Get()->CreatePixelShader(((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(), "Sandbox/Resources/Shaders/pickerPS.hlsl", "mouse");
		ResourceManager::Get()->CreatePixelShader(((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(), "Sandbox/Resources/Shaders/normalMapPs.hlsl", "normal");
		ResourceManager::Get()->CreateVertexShader(((Hollow::DirectXRenderer*)Hollow::Renderer::Get())->GetDevice(), "Sandbox/Resources/Shaders/vs.hlsl", "simple", bxlayout, numElements);
	}
public:
	Application(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		this->m_HWND = this->m_Window.getHWND();
		Renderer::InitializeRenderer(RendererType::DirectX, this->m_HWND, SCREEN_WIDTH, SCREEN_HEIGHT);

		this->InitResources();

		this->m_RenderSystem = new RenderSystem(SCREEN_WIDTH, SCREEN_HEIGHT);
		this->m_InterfaceSystem = new InterfaceSystem(this->m_HWND);
		this->m_MoveSystem = new MoveSystem();
		this->m_CollisionSystem = new CollisionSystem();
		this->InitScene();
	}

	void Run() {
		while (m_Window.ProcessMessage())
		{
			InputManager::GetMousePosition(m_RenderSystem->GetCamera()->GetProjectionMatrix(), m_RenderSystem->GetCamera()->GetViewMatrix());
			engine.GetTimer()->Tick(DELTA_TIME_STEP);
			this->m_RenderSystem->PreUpdate(DELTA_TIME_STEP);
			this->m_MoveSystem->Update(DELTA_TIME_STEP);
			this->m_RenderSystem->Update(DELTA_TIME_STEP);
			this->m_InterfaceSystem->Update(DELTA_TIME_STEP);
			this->m_CollisionSystem->Update(DELTA_TIME_STEP);
			this->m_RenderSystem->PostUpdate(DELTA_TIME_STEP);

			engine.GetEventHandler()->DispatchEvents();
			Hollow::InputManager::Clear();
		}
	}
};