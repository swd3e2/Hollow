#pragma once

#ifndef HW_INPUT_MANAGER_H
#define HW_INPUT_MANAGER_H

#include "Hollow/Platform.h"
#include "KeyCodes.h"
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include "DirectXMath.h"
#include "Hollow/Events/IEventListener.h"
#include "Hollow/Events/EventSystem.h"
#include "Hollow/Events/DefaultEvents.h"
#include "Hollow/Core/CModule.h"
#include <vector>

using namespace DirectX;

namespace Hollow {
	class InputManager : IEventListener, public CModule<InputManager>
	{
	private:
		static bool KeyboardKeys[256];
		static bool MouseButton[5];
	public:
		// Mouse moving delta
		static long mx;
		static long my;
		// Actual position of cursor on screen
		static float mcx;
		static float mcy;
		static long lastX;
		static long lastY;
	public:
		virtual void onStartUp() override
		{
			for (int i = 0; i < 256; i++) {
				KeyboardKeys[i] = false;
			}
			for (int i = 0; i < 5; i++) {
				MouseButton[i] = false;
			}

			EventSystem::instance()->addEventListener(this, &InputManager::onButtonReleased, ButtonReleasedEvent::staticGetId());
			EventSystem::instance()->addEventListener(this, &InputManager::onButtonPressed, ButtonPressedEvent::staticGetId());
			EventSystem::instance()->addEventListener(this, &InputManager::onMouseMove, MouseMoveEvent::staticGetId());
		}

		void onMouseMove(IEvent* ev)
		{
			MouseMoveEvent* event = (MouseMoveEvent*)ev;
			lastX = mcx - event->x;
			lastY = mcy - event->y;

			mcx = event->x;
			mcy = event->y;
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

		void clear() { mx = 0; my = 0; lastX = 0; lastY = 0; }

		static bool GetKeyboardKeyIsPressed(eKeyCodes keyCode) { return KeyboardKeys[keyCode]; }
		static void SetKeyboardKeyActive(eKeyCodes keyCode, bool active) { KeyboardKeys[keyCode] = active; }
		static bool GetMouseButtonIsPressed(eMouseKeyCodes mouseCode) { return MouseButton[mouseCode]; }
		static void SetMouseButtonActive(eMouseKeyCodes mouseCode, bool active) { MouseButton[mouseCode] = active; }
	};
}

#endif