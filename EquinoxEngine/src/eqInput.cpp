#include "eqInput.hpp"

namespace eq
{
	Input::KeyboardInputMap Input::keyboard;
	Input::MouseInputMap Input::mouse;

	Input::KeyState Input::getKeyState(uint32_t keycode)
	{
		return keyboard.keys[keycode];
	}

	bool Input::isKeyPressed(uint32_t keycode)
	{
		return keyboard.keys[keycode].isDown;
	}

	bool  Input::isKeyReleased(uint32_t keycode)
	{
		return !keyboard.keys[keycode].isDown;
	}

	bool  Input::wasKeyHit(uint32_t keycode)
	{
		bool keyHit = ((!keyboard.keys[keycode].wasDown) && keyboard.keys[keycode].isDown);
		keyboard.keys[keycode].wasDown = 0;
		keyboard.keys[keycode].isDown = 0;
		return keyHit;
	}

	void Input::processKeyboardInput(uint32_t VKCode, bool wasDown, bool isDown)
	{
		if (wasDown != isDown)
		{
			if (VKCode >= 'A' && VKCode <= 'Z')
			{
				uint32_t eqKeycode = VKCode - 'A';
				keyboard.keys[eqKeycode].isDown = isDown;
				keyboard.keys[eqKeycode].wasDown = wasDown;
			}

			else if (VKCode >= '0' && VKCode <= '9')
			{
				uint32_t eq_keycode = VKCode - '0' + EQ_0;
				keyboard.keys[eq_keycode].isDown = isDown;
				keyboard.keys[eq_keycode].wasDown = wasDown;
			}

			else
			{
				switch (VKCode)
				{
				case VK_UP:
				{
					keyboard.keys[EQ_UP].isDown = isDown;
					keyboard.keys[EQ_UP].wasDown = wasDown;
				}break;

				case VK_DOWN:
				{
					keyboard.keys[EQ_DOWN].isDown = isDown;
					keyboard.keys[EQ_DOWN].wasDown = wasDown;
				}break;

				case VK_LEFT:
				{
					keyboard.keys[EQ_LEFT].isDown = isDown;
					keyboard.keys[EQ_LEFT].wasDown = wasDown;
				}break;

				case VK_RIGHT:
				{
					keyboard.keys[EQ_RIGHT].isDown = isDown;
					keyboard.keys[EQ_RIGHT].wasDown = wasDown;
				}break;

				case VK_OEM_MINUS:
				{
					keyboard.keys[EQ_MINUS].isDown = isDown;
					keyboard.keys[EQ_MINUS].wasDown = wasDown;
				}break;

				case VK_OEM_PLUS:
				{
					keyboard.keys[EQ_PLUS].isDown = isDown;
					keyboard.keys[EQ_PLUS].wasDown = wasDown;
				}break;

				case VK_SHIFT:
				{
					keyboard.keys[EQ_SHIFT].isDown = isDown;
					keyboard.keys[EQ_SHIFT].wasDown = wasDown;
				}break;

				case VK_CONTROL:
				{
					keyboard.keys[EQ_CONTROL].isDown = isDown;
					keyboard.keys[EQ_CONTROL].wasDown = wasDown;
				}break;

				case VK_MENU:
				{
					keyboard.keys[EQ_ALT].isDown = isDown;
					keyboard.keys[EQ_ALT].wasDown = wasDown;
				}break;

				case VK_SPACE:
				{
					keyboard.keys[EQ_SPACE].isDown = isDown;
					keyboard.keys[EQ_SPACE].wasDown = wasDown;
				}break;

				case VK_ESCAPE:
				{
					keyboard.keys[EQ_ESCAPE].isDown = isDown;
					keyboard.keys[EQ_ESCAPE].wasDown = wasDown;
				}break;

				case VK_CAPITAL:
				{
					keyboard.keys[EQ_CAPSLOCK].isDown = isDown;
					keyboard.keys[EQ_CAPSLOCK].wasDown = wasDown;
				}break;

				case VK_TAB:
				{
					keyboard.keys[EQ_TAB].isDown = isDown;
					keyboard.keys[EQ_TAB].wasDown = wasDown;
				}break;

				case VK_RETURN:
				{
					keyboard.keys[EQ_ENTER].isDown = isDown;
					keyboard.keys[EQ_ENTER].wasDown = wasDown;
				}break;

				case VK_BACK:
				{
					keyboard.keys[EQ_BACKSPACE].isDown = isDown;
					keyboard.keys[EQ_BACKSPACE].wasDown = wasDown;
				}break;

				case VK_OEM_3:
				{
					keyboard.keys[EQ_TILDE].isDown = isDown;
					keyboard.keys[EQ_TILDE].wasDown = wasDown;
				}break;
				}
			}
		}
	}

	Input::Position Input::getMousePosition(Math::Vector2* position)
	{
		if (position)
		{
			position->x = mouse.position.x;
			position->y = mouse.position.y;
		}

		return mouse.position;
	}

	bool Input::isMouseButtonPressed(unsigned int buttonCode)
	{
		return mouse.buttons[buttonCode].isDown;
	}

	bool Input::isMouseButtonReleased(unsigned int buttonCode)
	{
		return mouse.buttons[buttonCode].wasDown;

	}

	bool Input::wasMouseButtonHit(unsigned int buttonCode)
	{
		bool hit = ((!mouse.buttons[buttonCode].wasDown) && mouse.buttons[buttonCode].isDown);
		mouse.buttons[buttonCode].wasDown = 0;
		mouse.buttons[buttonCode].isDown = 0;
		return hit;
	}

	void Input::processMouseInput(WPARAM wParam, LPARAM lParam)
	{
		mouse.buttons[EQ_MOUSE_LEFT].wasDown = mouse.buttons[EQ_MOUSE_LEFT].isDown;
		mouse.buttons[EQ_MOUSE_RIGHT].wasDown = mouse.buttons[EQ_MOUSE_RIGHT].isDown;
		mouse.buttons[EQ_MOUSE_MIDDLE].wasDown = mouse.buttons[EQ_MOUSE_MIDDLE].isDown;
		mouse.buttons[EQ_MOUSE_X1].wasDown = mouse.buttons[EQ_MOUSE_X1].isDown;
		mouse.buttons[EQ_MOUSE_X2].wasDown = mouse.buttons[EQ_MOUSE_X2].isDown;

		mouse.buttons[EQ_MOUSE_LEFT].isDown = wParam & MK_LBUTTON;
		mouse.buttons[EQ_MOUSE_RIGHT].isDown = wParam & MK_RBUTTON;
		mouse.buttons[EQ_MOUSE_MIDDLE].isDown = wParam & MK_MBUTTON;
		mouse.buttons[EQ_MOUSE_X1].isDown = wParam & MK_XBUTTON1;
		mouse.buttons[EQ_MOUSE_X2].isDown = wParam & MK_XBUTTON2;

		updateMousePosition(lParam);
	}

	void Input::updateMousePosition(LPARAM lParam)
	{
		mouse.position.x = GET_X_LPARAM(lParam);
		mouse.position.y = GET_Y_LPARAM(lParam);
	}
}