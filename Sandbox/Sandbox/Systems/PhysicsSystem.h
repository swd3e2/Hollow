#pragma once
#include <Hollow/ECS/System.h>
#include <btBulletDynamicsCommon.h>
#include "Hollow/Core/CModule.h"
#include "Sandbox/Components/PhysicsComponent.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include <Hollow/ECS/EntityManager.h>
#include "Sandbox/Profiler.h"

class PhysicsSystem : public Hollow::System<PhysicsSystem>, public Hollow::CModule<PhysicsSystem>
{
public:
	Hollow::s_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	Hollow::s_ptr<btSequentialImpulseConstraintSolver> solver;
	Hollow::s_ptr<btBroadphaseInterface> overlappingPairCache;
	Hollow::s_ptr<btCollisionDispatcher> dispatcher;
	Hollow::s_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
public:
	PhysicsSystem()
	{
		collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = std::make_shared<btCollisionDispatcher>(collisionConfiguration.get());
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = std::make_shared<btDbvtBroadphase>();
		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = std::make_shared<btSequentialImpulseConstraintSolver>();

		dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());
		dynamicsWorld->setGravity(btVector3(0, -10.0f, 0));
	}

	virtual void Update(double dt) override
	{
		Profiler::begin("PhysicsSystem update(): ");
		dynamicsWorld->stepSimulation(dt / 1000.0);

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<TransformComponent>() && entity.hasComponent<PhysicsComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				PhysicsComponent* physics = entity.getComponent<PhysicsComponent>();
				
				btTransform tr;

				if (physics->isDynamic) {
					physics->body->getMotionState()->getWorldTransform(tr);
				} else {
					tr = physics->body->getWorldTransform();
				}
				
				btVector3& pos = tr.getOrigin();

				transform->position.x = pos.getX();
				transform->position.y = pos.getY();
				transform->position.z = pos.getZ();

				btQuaternion rot = physics->body->getCenterOfMassTransform().getRotation();

				if (physics->applyRotation) {
					rot.getEulerZYX(transform->rotation.x, transform->rotation.y, transform->rotation.z);
				}
			}
		}
		Profiler::end();
	}
};