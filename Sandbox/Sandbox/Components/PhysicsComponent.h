#pragma once
#include <Hollow/ECS/Component.h>
#include "Hollow/Math/Vector3.h"
#include <btBulletDynamicsCommon.h>

class PhysicsComponent : public Hollow::Component<PhysicsComponent>
{
public:
	btCollisionShape* colShape;
	btBoxShape* boxShape;
	btDefaultMotionState* myMotionState;
	btRigidBody* body;
public:
	PhysicsComponent(const Hollow::Vector3& position, float mass_in);
};