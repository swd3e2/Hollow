#pragma once
#include <Hollow/ECS/Component.h>
#include "Hollow/Math/Vector3.h"
#include <btBulletDynamicsCommon.h>
#include "Hollow/Import/Mesh.h"
#include "Hollow/Platform.h"

enum PhysicsShapeType
{
	PST_BOX,
	PST_SPHERE,
	PST_PLANE
};

class PhysicsComponent : public Hollow::Component<PhysicsComponent>
{
public:
	Hollow::s_ptr<btCollisionShape> shape;
	Hollow::s_ptr<btDefaultMotionState> myMotionState;
	Hollow::s_ptr<btRigidBody> body;
	bool applyRotation = true;
public:
	PhysicsComponent() = default;
	~PhysicsComponent();

	void addBoxShape(const Hollow::Vector3& size, const Hollow::Vector3& position, float mass);
	void addSphereShape(float radius, const Hollow::Vector3& position, float mass);
	void addPlaneShape(const Hollow::Vector3& planeNormal, float size, const Hollow::Vector3& position, float mass);
	void addCapsuleShape(float height, float radius, const Hollow::Vector3& position, float mass);
	void load(const Hollow::s_ptr<Hollow::Import::Model>& model, const Hollow::Vector3& position, float mass);
private:
	void initialize(const Hollow::Vector3& position, float mass);
};