#pragma once
#include "Hollow/ECS/System.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include <vector>
#include "Hollow/Containers/vector.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Input/InputManager.h"
#include <DirectXMath.h>
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/Common/Log.h"

using namespace DirectX;
using namespace Hollow;

struct SimpleFieldVertex
{
	XMFLOAT4 first;
	XMFLOAT4 second;
	XMFLOAT4 third;
};

class MoveSystem : public Hollow::System<MoveSystem>
{
private:
	float pix;
	float piy;
	float piz;

	XMVECTOR pickRayInWorldSpacePos;
	XMVECTOR pickRayInWorldSpaceDir;
	XMVECTOR pickRayInViewSpaceDir;
	XMVECTOR pickRayInViewSpacePos;

	std::vector<SimpleFieldVertex> field;
public:
	Camera* camera;
	TransformComponent* posComponent;
	MoveComponent * moveComponent;
	SelectComponent* selectComponent;
public:
	MoveSystem(Camera* camera) :
		camera(camera)
	{
		field.push_back({ XMFLOAT4(-FIELD_SIZE, 0, -FIELD_SIZE, 0), XMFLOAT4(-FIELD_SIZE, 0, FIELD_SIZE, 0), XMFLOAT4(FIELD_SIZE, 0, FIELD_SIZE, 0) });
		field.push_back({ XMFLOAT4(FIELD_SIZE, 0, -FIELD_SIZE, 0), XMFLOAT4(FIELD_SIZE, 0, FIELD_SIZE, 0), XMFLOAT4(-FIELD_SIZE, 0, -FIELD_SIZE, 0) });
	}

	void GetMousePosition()
	{
		//float xOffset = 0, yOffset = 0;
		//int border_thickness;

		//if (true) {
		//	float centerScreenX = (float)GetSystemMetrics(SM_CXSCREEN) / 2.0f - 1920.0f / 2.0f;
		//	float centerScreenY = (float)GetSystemMetrics(SM_CYSCREEN) / 2.0f - 1080.0f / 2.0f;
		//	border_thickness = GetSystemMetrics(SM_CYCAPTION);

		//	xOffset = centerScreenX;
		//	yOffset = centerScreenY - 50;
		//}

		//// Normalized device coordinates
		//float pcx = (((2 * InputManager::mcx) / 1600.0f) - 1.0f) / camera->getProjectionMatrix().md[0][0];
		//float pcy = -(((2 * InputManager::mcy) / 900.0f) - 1.0f) / camera->getProjectionMatrix().md[1][1];

		//pickRayInViewSpaceDir = XMVectorSet(pcx, pcy, 1.0f, 0.0f);
		//pickRayInViewSpaceDir = XMVector3Normalize(pickRayInViewSpaceDir);

		//Matrix4 tempView = camera->getViewMatrix();
		//XMMATRIX viewMatrix(
		//	tempView.md[0][0], tempView.md[1][0], tempView.md[2][0], tempView.md[3][0],
		//	tempView.md[0][1], tempView.md[1][1], tempView.md[2][1], tempView.md[3][1],
		//	tempView.md[0][2], tempView.md[1][2], tempView.md[2][2], tempView.md[3][2],
		//	tempView.md[0][3], tempView.md[1][3], tempView.md[2][3], tempView.md[3][3]);
		//// Transform 3D Ray from View space to 3D ray in World space
		//XMMATRIX pickRayToWorldSpaceMatrix;
		//XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

		//pickRayToWorldSpaceMatrix = DirectX::XMMatrixInverse(&matInvDeter, viewMatrix); //Inverse of View Space matrix is World space matrix

		//pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
		//pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
	}

	void GetMousePosition2()
	{
		/*float x = (2.0f * InputManager::mcx) / 1600.0f - 1.0f;
		float y = 1.0f - (2.0f * InputManager::mcy) / 900.0f;
		float z = 1.0f;

		Hollow::Vector3 ray_nds(x, y, z);
		Vector4 ray_eye = ray_nds * Matrix4::inverse(camera->getProjectionMatrix());
		ray_eye.z = -1.0f;
		Vector4 ray_world = ray_eye * Matrix4::inverse(camera->getViewMatrix());
		ray_world = Vector4::normalize(ray_world);

		HW_DEBUG("{} {} {}", ray_world.x, ray_world.y, ray_world.z);*/
	}

	bool calculate()
	{
		return false;
		//for (auto& object : field) {
		//	//Temporary 3d floats for each vertex
		//	XMFLOAT4 tV1 = object.first;
		//	XMFLOAT4 tV2 = object.second;
		//	XMFLOAT4 tV3 = object.third;

		//	XMVECTOR tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
		//	XMVECTOR tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
		//	XMVECTOR tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

		//	XMVECTOR faceNormal = { 0, 1, 0, 0 };

		//	//Calculate a point on the triangle for the plane equation
		//	XMVECTOR triPoint = tri1V1;

		//	//Get plane equation ("Ax + By + Cz + D = 0") Variables
		//	float tri1A = XMVectorGetX(faceNormal);
		//	float tri1B = XMVectorGetY(faceNormal);
		//	float tri1C = XMVectorGetZ(faceNormal);
		//	float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

		//	//Now we find where (on the ray) the ray intersects with the triangles plane
		//	float ep1, ep2, t = 0.0f;
		//	float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
		//	XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//	ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
		//	ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

		//	//Make sure there are no divide-by-zeros
		//	if (ep2 != 0.0f) {
		//		t = -(ep1 + tri1D) / (ep2);
		//	}

		//	//Make sure you don't pick objects behind the camera
		//	if (t > 0.0f) {
		//		//Get the point on the plane
		//		planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
		//		planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
		//		planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

		//		pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

		//		if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane)) {
		//			//Return the distance to the hit, so you can check all the other pickable objects in your scene
		//			//and choose whichever object is closest to the camera

		//			pix = planeIntersectX;
		//			piy = planeIntersectY;
		//			piz = planeIntersectZ;

		//			return false;
		//		}
		//	}
		//}
	}

	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
	{
		////To find out if the point is inside the triangle, we will check to see if the point
		////is on the correct side of each of the triangles edges.

		//XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
		//XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
		//if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0) {
		//	cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
		//	cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
		//	if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0) {
		//		cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
		//		cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
		//		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0) {
		//			return true;
		//		} else {
		//			return false;
		//		}	
		//	} else {
		//		return false;
		//	}
		//}
		return false;
	}

	virtual void Update(double dt) override
	{
		/*for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			posComponent = entity.getComponent<TransformComponent>();
			moveComponent = entity.getComponent<MoveComponent>();
			selectComponent = entity.getComponent<SelectComponent>();

			if (posComponent == nullptr || moveComponent == nullptr || selectComponent == nullptr) continue;

			GetMousePosition2();
			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				GetMousePosition();
				calculate();
				moveComponent->elapsed = 0.0f;
				XMFLOAT3 tempFloat3(posComponent->position.x, posComponent->position.y, posComponent->position.z);
				moveComponent->originalPosition = XMLoadFloat3(&tempFloat3);
				DirectX::XMFLOAT3 temp(pix, 0, piz);
				moveComponent->destination = XMLoadFloat3(&temp);

				moveComponent->direction = moveComponent->originalPosition - moveComponent->destination;
				XMVECTOR length = XMVector3Length(moveComponent->direction);
				moveComponent->distance = 0.0f;
				XMStoreFloat(&moveComponent->distance, length);
				moveComponent->direction = XMVector4Normalize(moveComponent->direction);
				moveComponent->move = true;
			}

			if (moveComponent->move == true) {
				posComponent->position.x -= XMVectorGetX(moveComponent->direction) * moveComponent->speed;
				posComponent->position.z -= XMVectorGetZ(moveComponent->direction) * moveComponent->speed;

				XMFLOAT3 tempFloat3(posComponent->position.x, posComponent->position.y, posComponent->position.z);
				XMVECTOR passed = moveComponent->originalPosition - XMLoadFloat3(&tempFloat3);
				XMVECTOR length = XMVector3Length(passed);

				float distance = 0.0f;
				XMStoreFloat(&distance, length);

				if (distance >= moveComponent->distance)
				{
					posComponent->position = {pix, 0, piz};
					moveComponent->move = false;
				}
			}
			moveComponent->elapsed += dt;
		}*/
	}
};
