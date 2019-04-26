#pragma once
#include "Hollow/Application.h"
#include "Hollow/Graphics/PointLight.h"
#include "Hollow/ECS/GameObject.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/PointLightComponent.h"
#include "Hollow/ECS/Light.h"
#include "Hollow/Math/Vector3.h"

using namespace DirectX;

class MyApp : public Application
{
private:
public:
	void onStartUp()
	{
		/*Light* light = entityManager.createEntity<Light>();
		light->addComponent<PointLightComponent>(m_Renderer->getDevice());
		light->addComponent<TransformComponent, Vector3&&, Vector3 &&, Vector3 &&>
			({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

		Light* light2 = entityManager.createEntity<Light>();
		light2->addComponent<PointLightComponent>(m_Renderer->getDevice());
		light2->addComponent<TransformComponent, Vector3&&, Vector3 &&, Vector3 &&>
			({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

		light2->getComponent<PointLightComponent>()->light.data.color[2] = 1.0f;
		light2->getComponent<PointLightComponent>()->light.data.range = 5.f;*/
	}
};