#pragma once
#include "Hollow/Application.h"
#include "Hollow/Graphics/PointLight.h"

using namespace DirectX;

class MyApp : public Application
{
private:
	/*MoveSystem*				m_MoveSystem;
	CollisionSystem*		m_CollisionSystem;*/
public:
	MyApp()
	{
		/*m_MoveSystem = new MoveSystem();
		m_CollisionSystem = new CollisionSystem();
		systemManager.AddSystem(m_MoveSystem);
		systemManager.AddSystem(m_CollisionSystem);*/
		
		m_Renderer->pointLight = new PointLight(m_Renderer->getDevice());

		sceneManager.CreateSceneObject(
			((D3DRenderer*)m_Renderer)->getDevice(),
			meshManager.CreateMesh("Sandbox/Resources/Meshes/gwen.obj", true),
			new Transform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f })
		);
	}
};