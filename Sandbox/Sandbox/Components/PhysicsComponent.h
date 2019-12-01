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
	PST_PLANE,
	PST_CAPSULE,
	PST_AABB
};

class PhysicsComponent : public Hollow::Component<PhysicsComponent>
{
public:
	Hollow::s_ptr<btCollisionShape> shape;
	Hollow::s_ptr<btDefaultMotionState> myMotionState;
	Hollow::s_ptr<btRigidBody> body;
	PhysicsShapeType type;
	Hollow::Vector3 originPosition;
	Hollow::Vector3 boxSize;
	Hollow::Vector3 planeNormal;
	Hollow::Vector3 localScale = Hollow::Vector3(1.0f, 1.0f, 1.0f);
	Hollow::Vector3 angularFactor = Hollow::Vector3(1.0f, 1.0f, 1.0f);
	float contactingThreshold;
	float capsuleHeight;
	float capsuleRadius;
	float planeSize;
	float sphereRadius;
	bool applyRotation = true;
	bool isDynamic = false;
	float mass = 0.0f;
public:
	PhysicsComponent(const Hollow::Vector3& position = Hollow::Vector3(), float mass = 0.0f);
	~PhysicsComponent();

	void addBoxShape(const Hollow::Vector3& size);
	void addAABBShape(const Hollow::s_ptr<Hollow::Import::Model>& model);
	void addSphereShape(float radius);
	void addPlaneShape(const Hollow::Vector3& planeNormal, float size);
	void addCapsuleShape(float height, float radius);
	void setPosition(const Hollow::Vector3& position);
	void setRotation(const Hollow::Vector3 rotation);
	void setLocalScale(const Hollow::Vector3& scale);
	void setAngularFactor(const Hollow::Vector3& factor);
	inline void setOriginPosition(const Hollow::Vector3& position) { this->originPosition = position; }
	inline void setMass(const float mass) { this->mass = mass; }
	void init();
};