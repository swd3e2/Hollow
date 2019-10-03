#include "PhysicsComponent.h"
#include "Sandbox/Systems/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(const Hollow::Vector3& position, float mass_in)
{
	colShape = new btSphereShape(btScalar(1.));
	boxShape = new btBoxShape(btVector3(10, 1, 10));
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

void PhysicsComponent::load(Hollow::Import::Model* model)
{
	btTriangleMesh* tetraMesh = new btTriangleMesh();
	for (int i = 0; i < model->meshes.size(); i++) {
		for (int j = 0; j < model->meshes[i]->indices.size() / 3; j++) {
			const Hollow::Vertex& first	 = model->meshes[i]->vertices[model->meshes[i]->indices[j * 3]];
			const Hollow::Vertex& second = model->meshes[i]->vertices[model->meshes[i]->indices[j * 3 + 1]];
			const Hollow::Vertex& third	 = model->meshes[i]->vertices[model->meshes[i]->indices[j * 3 + 2]];

			tetraMesh->addTriangle(
				btVector3(first.pos.x, first.pos.y, first.pos.z), 
				btVector3(second.pos.x, second.pos.y, second.pos.z),
				btVector3(third.pos.x, third.pos.y, third.pos.z), 
				true
			);
		}
	}
	btBvhTriangleMeshShape* tetraShape = new btBvhTriangleMeshShape(tetraMesh, false);

	btVector3 localInertia(0, 0, 0);
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 7, 0));
	btDefaultMotionState* motionState = new btDefaultMotionState(trans);
	btRigidBody* body = new btRigidBody(1, motionState, tetraShape, localInertia);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setCcdMotionThreshold(.5);
	body->setCcdSweptSphereRadius(0);
	PhysicsSystem::instance()->dynamicsWorld->addRigidBody(body);
}