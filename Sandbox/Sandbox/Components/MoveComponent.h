#pragma once
#include <Hollow/ECS/Component.h>
#include <DirectXMath.h>

using namespace DirectX;

class MoveComponent : public Hollow::Component<MoveComponent>
{
public:
	MoveComponent() : move(true) {}
	bool move;
	float speed = 1.0f;
	float elapsed = 0.0f;
	float distance = 0.0f;
	float distancePassed = 0.0f;
	XMVECTOR destination;
	XMVECTOR direction;
	XMVECTOR originalPosition;
};
