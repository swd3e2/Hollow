#pragma once
#include "Hollow/Platform.h"
#include "KeyCodes.h"
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include "DirectXMath.h"
#include "Hollow/Events/IEventListener.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/ButtonPressedEvent.h"
#include "Hollow/Events/ButtonReleasedEvent.h"
#include "Hollow/Events/WindowCreateEvent.h"
#include "Hollow/Core/CModule.h"
#include <vector>
#define FIELD_SIZE 10000

using namespace DirectX;

struct SimpleFieldVertex
{
	XMFLOAT4 first;
	XMFLOAT4 second;
	XMFLOAT4 third;
};

class  InputManager : IEventListener, public CModule<InputManager>
{
private:
	static bool KeyboardKeys[256];
	static bool MouseButton[5];
public:
	static long mx;
	static long my;

	static float mcx;
	static float mcy;

	static float pcx;
	static float pcy;

	static float pix;
	static float piy;
	static float piz;

	static XMVECTOR pickRayInWorldSpacePos;
	static XMVECTOR pickRayInWorldSpaceDir;
	static XMVECTOR pickRayInViewSpaceDir;
	static XMVECTOR pickRayInViewSpacePos;

	static std::vector<SimpleFieldVertex> field;
public:
	void startUp()
	{
		for (int i = 0; i < 256; i++) {
			KeyboardKeys[i] = false;
		}
		for (int i = 0; i < 5; i++) {
			MouseButton[i] = false;
		}

		field.push_back({ XMFLOAT4(-FIELD_SIZE, 0, -FIELD_SIZE, 0), XMFLOAT4(-1000, 0, 1000, 0), XMFLOAT4(1000, 0, 1000, 0) });
		field.push_back({ XMFLOAT4(FIELD_SIZE, 0, -FIELD_SIZE, 0), XMFLOAT4(1000, 0, 1000, 0), XMFLOAT4(-1000, 0, -1000, 0) });

		EventSystem::instance()->addEventListener(new EventDelegate<InputManager>(this, &InputManager::onButtonReleased, ButtonReleasedEvent::getStaticEventId()));
		EventSystem::instance()->addEventListener(new EventDelegate<InputManager>(this, &InputManager::onButtonPressed, ButtonPressedEvent::getStaticEventId()));

		setStartedUp();
	}

	void shutdown()
	{
		setShutdown();
	}

	void onButtonPressed(IEvent* ev)
	{
		ButtonPressedEvent* event = (ButtonPressedEvent*)ev;
		SetKeyboardKeyActive(event->button, true);
	}

	void onButtonReleased(IEvent* ev)
	{
		ButtonReleasedEvent* event = (ButtonReleasedEvent*)ev;
		SetKeyboardKeyActive(event->button, false);
	}

	void Clear() { mx = 0; my = 0; }

	static bool GetKeyboardKeyIsPressed(eKeyCodes keyCode) { return KeyboardKeys[keyCode]; }
	static void SetKeyboardKeyActive(eKeyCodes keyCode, bool active) { KeyboardKeys[keyCode] = active; }
	static bool GetMouseButtonIsPressed(eMouseKeyCodes mouseCode) { return MouseButton[mouseCode]; }
	static void SetMouseButtonActive(eMouseKeyCodes mouseCode, bool active) { MouseButton[mouseCode] = active; }

	static void GetMousePosition(XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
	{
		float xOffset = 0, yOffset = 0;
		int border_thickness;

		if (false)
		{
			float centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - 1600 / 2;
			float centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - 800 / 2;
			border_thickness = GetSystemMetrics(SM_CYCAPTION);

			xOffset = centerScreenX;
			yOffset = centerScreenY - 50;
		}

		XMFLOAT4X4 fView;
		XMStoreFloat4x4(&fView	, projectionMatrix);
		pcx = (((2 * mcx) / 1800.0f) - 1.0f) / fView._11;
		pcy = -(((2 * mcy) / 900.0f) - 1.0f) / fView._22;

		pickRayInViewSpaceDir = XMVectorSet(pcx, pcy, 1.0f, 0.0f);

		pickRayInViewSpaceDir = XMVector3Normalize(pickRayInViewSpaceDir);

		// Transform 3D Ray from View space to 3D ray in World space
		XMMATRIX pickRayToWorldSpaceMatrix;
		XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

		pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, viewMatrix);    //Inverse of View Space matrix is World space matrix

		pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
		pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
	}

	static bool calculate()
	{
		for (auto& object : field) {
			//Temporary 3d floats for each vertex
			XMFLOAT4 tV1 = object.first;
			XMFLOAT4 tV2 = object.second;
			XMFLOAT4 tV3 = object.third;

			XMVECTOR tri1V1 = XMVectorSet(tV1.x, tV1.y, tV1.z, 0.0f);
			XMVECTOR tri1V2 = XMVectorSet(tV2.x, tV2.y, tV2.z, 0.0f);
			XMVECTOR tri1V3 = XMVectorSet(tV3.x, tV3.y, tV3.z, 0.0f);

			XMVECTOR faceNormal = { 0, 1, 0, 0 };

			//Calculate a point on the triangle for the plane equation
			XMVECTOR triPoint = tri1V1;

			//Get plane equation ("Ax + By + Cz + D = 0") Variables
			float tri1A = XMVectorGetX(faceNormal);
			float tri1B = XMVectorGetY(faceNormal);
			float tri1C = XMVectorGetZ(faceNormal);
			float tri1D = (-tri1A * XMVectorGetX(triPoint) - tri1B * XMVectorGetY(triPoint) - tri1C * XMVectorGetZ(triPoint));

			//Now we find where (on the ray) the ray intersects with the triangles plane
			float ep1, ep2, t = 0.0f;
			float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;
			XMVECTOR pointInPlane = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			ep1 = (XMVectorGetX(pickRayInWorldSpacePos) * tri1A) + (XMVectorGetY(pickRayInWorldSpacePos) * tri1B) + (XMVectorGetZ(pickRayInWorldSpacePos) * tri1C);
			ep2 = (XMVectorGetX(pickRayInWorldSpaceDir) * tri1A) + (XMVectorGetY(pickRayInWorldSpaceDir) * tri1B) + (XMVectorGetZ(pickRayInWorldSpaceDir) * tri1C);

			//Make sure there are no divide-by-zeros
			if (ep2 != 0.0f)
				t = -(ep1 + tri1D) / (ep2);

			if (t > 0.0f)    //Make sure you don't pick objects behind the camera
			{
				//Get the point on the plane
				planeIntersectX = XMVectorGetX(pickRayInWorldSpacePos) + XMVectorGetX(pickRayInWorldSpaceDir) * t;
				planeIntersectY = XMVectorGetY(pickRayInWorldSpacePos) + XMVectorGetY(pickRayInWorldSpaceDir) * t;
				planeIntersectZ = XMVectorGetZ(pickRayInWorldSpacePos) + XMVectorGetZ(pickRayInWorldSpaceDir) * t;

				pointInPlane = XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectZ, 0.0f);

				if (PointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
				{
					//Return the distance to the hit, so you can check all the other pickable objects in your scene
					//and choose whichever object is closest to the camera

					pix = planeIntersectX;
					piy = planeIntersectY;
					piz = planeIntersectZ;

					return false;
				}
			}
		}
	}

	static bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point)
	{
		//To find out if the point is inside the triangle, we will check to see if the point
		//is on the correct side of each of the triangles edges.

		XMVECTOR cp1 = XMVector3Cross((triV3 - triV2), (point - triV2));
		XMVECTOR cp2 = XMVector3Cross((triV3 - triV2), (triV1 - triV2));
		if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
		{
			cp1 = XMVector3Cross((triV3 - triV1), (point - triV1));
			cp2 = XMVector3Cross((triV3 - triV1), (triV2 - triV1));
			if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
			{
				cp1 = XMVector3Cross((triV2 - triV1), (point - triV1));
				cp2 = XMVector3Cross((triV2 - triV1), (triV3 - triV1));
				if (XMVectorGetX(XMVector3Dot(cp1, cp2)) >= 0)
				{
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
		return false;
	}
};
