#include "PhysicsComponent.h"
#include "Sandbox/Systems/PhysicsSystem.h"

PhysicsComponent::~PhysicsComponent()
{
	PhysicsSystem::instance()->dynamicsWorld->removeRigidBody(body.get());
}

void PhysicsComponent::addBoxShape(const Hollow::Vector3& size, const Hollow::Vector3& position, float mass)
{
	shape = std::make_shared<btBoxShape>(btVector3(size.x, size.y, size.z));
	initialize(position, mass);
}

void PhysicsComponent::addSphereShape(float radius, const Hollow::Vector3& position, float mass)
{
	shape = std::make_shared<btSphereShape>(radius);
	initialize(position, mass);
}

void PhysicsComponent::addPlaneShape(const Hollow::Vector3& planeNormal, float size, const Hollow::Vector3& position, float mass)
{
	shape = std::make_shared<btStaticPlaneShape>(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), size);
	initialize(position, mass);
}

void PhysicsComponent::addCapsuleShape(float height, float radius, const Hollow::Vector3& position, float mass)
{
	shape = std::make_shared<btCapsuleShape>(radius, height);
	initialize(position, mass);
}

void PhysicsComponent::load(const Hollow::s_ptr<Hollow::Import::Model>& model, const Hollow::Vector3& position, float mass)
{
	btConvexHullShape* convex = new btConvexHullShape();

	for (int i = 0; i < model->meshes.size(); i++) {
		for (int j = 0; j < model->meshes[i]->indices.size(); j++) {
			const Hollow::Vertex& vertex = model->meshes[i]->vertices[model->meshes[i]->indices[j]];
			convex->addPoint(btVector3(vertex.pos.x, vertex.pos.y, vertex.pos.z), true);
		}
	}
	
	/*btCompoundShape* compound = new btCompoundShape();
	compound->addChildShape(convex);*/

	shape = Hollow::s_ptr<btCollisionShape>(convex);

	btVector3 localInertia(0, 0, 0);

	bool isDynamic = (mass != 0.f);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	myMotionState = std::make_shared<btDefaultMotionState>(trans);

	body = std::make_shared<btRigidBody>(mass, myMotionState.get(), shape.get(), localInertia);

	PhysicsSystem::instance()->dynamicsWorld->addRigidBody(body.get());
}

void PhysicsComponent::initialize(const Hollow::Vector3& position, float mass)
{
	btTransform startTransform;
	startTransform.setIdentity();

	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(position.x, position.y, position.z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	myMotionState = std::make_shared<btDefaultMotionState>(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState.get(), shape.get(), localInertia);
	body = std::make_shared<btRigidBody>(rbInfo);
	
	body->setFriction(200.3f);
	body->setRollingFriction(200.3f);
	
	PhysicsSystem::instance()->dynamicsWorld->addRigidBody(body.get());
}
