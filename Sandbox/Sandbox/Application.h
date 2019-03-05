#pragma once
#include "Entities/GameObject.h"
#include "Components/MoveComponent.h"
#include "Components/PositionComponent.h"
#include "Systems/MoveSystem.h"
#include "Hollow/Input/InputManager.h"
#include "DirectXMath.h"
#include "d3d11.h"
#include <random>
#include "Hollow/Resources/Sound.h"
#include "Hollow/Containers/vector.h"
#include "Components/SelectComponent.h"
#include "Sandbox/Components/CollisionComponent.h"
#include "Sandbox/Systems/CollisionSystem.h"
#include <thread>
#include "Hollow/Application.h"
#include "Sandbox/Components/MeshComponent.h"

using namespace DirectX;

class MyApp : public Application
{
private:
	HWND*					m_HWND;
	MoveSystem*				m_MoveSystem;
	CollisionSystem*		m_CollisionSystem;
private:
	void InitScene()
	{
		/*Hollow::Sound* music2 = m_ResourceManager->CreateSoundResource("Sandbox/Resources/Sounds/2.wav");
		music2->Play();*/
		
		GameObject * object = m_EntityManager->CreateEntity<GameObject>();
		object->AddComponent<MeshComponent, size_t>(m_ResourceManager->CreateMeshResource("Sandbox/Resources/Meshes/g.obj"));
		object->AddComponent<PositionComponent, DirectX::XMFLOAT3, DirectX::XMFLOAT3, DirectX::XMFLOAT3>({ 0.0f, -3.110f, 0.0f }, { 15.0f, 15.0f, 15.0f }, { 0.0f, 0.0f, 0.0f });
		object->AddComponent<SelectComponent, bool>(false);
	}
public:
	MyApp()
	{
		m_MoveSystem = new MoveSystem();
		m_CollisionSystem = new CollisionSystem();

		m_SystemManager->AddSystem(m_MoveSystem);
		m_SystemManager->AddSystem(m_CollisionSystem);

		this->InitScene();
	}
};