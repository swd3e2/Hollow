#pragma once
#include <Hollow/ECS/Component.h>
#include "Hollow/Math/Vector3.h"
#include <btBulletDynamicsCommon.h>
#include "Hollow/Importer/Mesh.h"

class PhysicsComponent : public Hollow::Component<PhysicsComponent>
{
public:
	btCollisionShape* colShape;
	btBoxShape* boxShape;
	btDefaultMotionState* myMotionState;
	btRigidBody* body;
public:
	PhysicsComponent() = default;
	PhysicsComponent(const Hollow::Vector3& position, float mass_in);

	void load(Hollow::Import::Model* model);
};