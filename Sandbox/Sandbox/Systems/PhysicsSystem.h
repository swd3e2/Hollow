#pragma once
#include <Hollow/ECS/System.h>
#include <btBulletDynamicsCommon.h>
#include "Hollow/Core/CModule.h"
#include "Sandbox/Components/PhysicsComponent.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include <Hollow/ECS/EntityManager.h>

class PhysicsSystem : public Hollow::System<PhysicsSystem>, public Hollow::CModule<PhysicsSystem>
{
public:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btSequentialImpulseConstraintSolver* solver;
	btBroadphaseInterface* overlappingPairCache;
	btCollisionDispatcher* dispatcher;
	btDefaultCollisionConfiguration* collisionConfiguration;
public:
	PhysicsSystem()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld->setGravity(btVector3(0, -1.1f, 0));
	}

	virtual void Update(double dt) override
	{
		dynamicsWorld->stepSimulation(dt, 10);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<PhysicsComponent>()) {
				/*TransformComponent* transform = entity.getComponent<TransformComponent>();
				PhysicsComponent* physics = entity.getComponent<PhysicsComponent>();
				btTransform tr;
				physics->body->getMotionState()->getWorldTransform(tr);
				btVector3& pos = tr.getOrigin();

				transform->position.x = pos.getX();
				transform->position.y = pos.getY();
				transform->position.z = pos.getZ();*/
			}
		}
	}
};