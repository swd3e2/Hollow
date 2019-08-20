#include "PhysicsComponent.h"
#include "Sandbox/Systems/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(const Hollow::Vector3& position, float mass_in)
{
	colShape = new btSphereShape(btScalar(1.));
	boxShape = new btBoxShape(btVector3(10, 1, 10));
	new btTriangleMeshShape();
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(mass_in);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		boxShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, boxShape, localInertia);
	body = new btRigidBody(rbInfo);

	PhysicsSystem::instance()->dynamicsWorld->addRigidBody(body);
}