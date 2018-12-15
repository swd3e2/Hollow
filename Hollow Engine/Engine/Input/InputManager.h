#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include "KeyCodes.h"

namespace Hollow {
	class InputManager
	{
	private:
		static bool KeyboardKeys[256];
		static bool MouseButton[5];
	public:
		static long x;
		static long y;
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
	};
}

#endif