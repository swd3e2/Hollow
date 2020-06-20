#pragma once

#include "Hollow/ECS/System.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Hollow/ECS/EntityManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Hollow/Math/Matrix4.h"

class NodeUpdateSystem : public Hollow::System<NodeUpdateSystem>
{
public:
	virtual void PreUpdate(double dt) override
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<RenderableComponent>()) {
				RenderableComponent* renderableComponent = entity.getComponent<RenderableComponent>();
				updateTransform(renderableComponent->rootNode, renderableComponent, Hollow::Matrix4::identity());
			}
		}
	}

	void updateTransform(const int nodeId, RenderableComponent* renderable, const Hollow::Matrix4& parentTransform)
	{
		if (renderable->nodes.size() <= nodeId) return;

		const Hollow::s_ptr<RenderableComponent::Node>& node = renderable->nodes[nodeId];

		if (node->transform.hasChanged == true) {
			Hollow::Matrix4 localTransform = Hollow::Matrix4::translation(node->transform.translation) 
				* Hollow::Matrix4::rotation(node->transform.rotation) 
				* Hollow::Matrix4::scaling(node->transform.scale);
			node->transform.worldTransform = parentTransform * localTransform;
			node->transform.hasChanged = false;
		}

		for (auto& it : node->childs) {
			updateTransform(it, renderable, node->transform.worldTransform);
		}
	}

	virtual void Update(double dt) override
	{}

	virtual void PostUpdate(double dt) override
	{}
};