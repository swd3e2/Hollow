#pragma once
#include "Hollow/ECS/Entities/GameObject.h"
#include "Components/MoveComponent.h"
#include "Hollow/ECS/Components/PositionComponent.h"
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
#include "Hollow/Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Common/Log.h"

using namespace DirectX;

class MyApp : public Application
{
private:
	HWND*					m_HWND;
	MoveSystem*				m_MoveSystem;
	CollisionSystem*		m_CollisionSystem;
public:
	MyApp()
	{
		m_MoveSystem = new MoveSystem();
		m_CollisionSystem = new CollisionSystem();
		m_SystemManager->AddSystem(m_MoveSystem);
		m_SystemManager->AddSystem(m_CollisionSystem);
		
		sceneManager.CreateSceneObject(
			((D3DRenderer*)m_Renderer)->getDevice(),
			m_ResourceManager->CreateMeshResource("Sandbox/Resources/Meshes/gwen.obj"),
			new Transform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f })
		);
	}
};