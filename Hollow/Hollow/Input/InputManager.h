#pragma once
#include "Hollow/Platform.h"
#include "KeyCodes.h"
#include <windows.h>
#include "DirectXMath.h"

namespace Hollow {

	using namespace DirectX;

	class HOLLOW_API InputManager
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

		static XMVECTOR pickRayInWorldSpacePos;
		static XMVECTOR pickRayInWorldSpaceDir;
		static XMVECTOR pickRayInViewSpaceDir;
		static XMVECTOR pickRayInViewSpacePos;
	public:
		InputManager()
		{
			for (int i = 0; i < 256; i++) {
				KeyboardKeys[i] = false;
			}
			for (int i = 0; i < 5; i++) {
				MouseButton[i] = false;
			}
		}

		static bool GetKeyboardKeyIsPressed(eKeyCodes keyCode) { return KeyboardKeys[keyCode]; }
		static void SetKeyboardKeyActive(eKeyCodes keyCode, bool active) { KeyboardKeys[keyCode] = active; }
		static bool GetMouseButtonIsPressed(eMouseKeyCodes mouseCode) { return MouseButton[mouseCode]; }
		static void SetMouseButtonActive(eMouseKeyCodes mouseCode, bool active) { MouseButton[mouseCode] = active; }

		static void GetMousePosition(XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
		{
			float x, y, xOffset = 0, yOffset = 0;
			int border_thickness;

			if (false)
			{
				int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - 1600 / 2;
				int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - 800 / 2;
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

			//Uncomment this line if you want to use the center of the screen (client area)
			//to be the point that creates the picking ray (eg. first person shooter)
			//pickRayInViewSpaceDir = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

			// Transform 3D Ray from View space to 3D ray in World space
			XMMATRIX pickRayToWorldSpaceMatrix;
			XMVECTOR matInvDeter;    //We don't use this, but the xna matrix inverse function requires the first parameter to not be null

			pickRayToWorldSpaceMatrix = XMMatrixInverse(&matInvDeter, viewMatrix);    //Inverse of View Space matrix is World space matrix

			pickRayInWorldSpacePos = XMVector3TransformCoord(pickRayInViewSpacePos, pickRayToWorldSpaceMatrix);
			pickRayInWorldSpaceDir = XMVector3TransformNormal(pickRayInViewSpaceDir, pickRayToWorldSpaceMatrix);
		}

		static void Clear() { mx = 0; my = 0; }
	};
}