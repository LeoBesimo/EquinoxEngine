#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <stdint.h>

#include <eqMath.hpp>

#define EQ_MAX_KEYS 52


#define EQ_A			0
#define EQ_B			1
#define EQ_C			2
#define EQ_D			3
#define EQ_E			4
#define EQ_F			5
#define EQ_G			6
#define EQ_H			7
#define EQ_I			8
#define EQ_J			9
#define EQ_K			10
#define EQ_L			11
#define EQ_M			12
#define EQ_N			13
#define EQ_O			14
#define EQ_P			15
#define EQ_Q			16
#define EQ_R			17
#define EQ_S			18
#define EQ_T			19
#define EQ_U			20
#define EQ_V			21
#define EQ_W			22
#define EQ_X			23
#define EQ_Y			24
#define EQ_Z			25

#define EQ_UP			26
#define EQ_DOWN			27
#define EQ_LEFT			28
#define EQ_RIGHT		29

#define EQ_0			30
#define EQ_1			31
#define EQ_2			32
#define EQ_3			33
#define EQ_4			34
#define EQ_5			35
#define EQ_6			36
#define EQ_7			37
#define EQ_8			38
#define EQ_9			39
#define EQ_MINUS		40
#define EQ_PLUS			41

#define EQ_SHIFT		42
#define EQ_CONTROL		43
#define EQ_ALT			44
#define EQ_SPACE		45
#define EQ_ESCAPE		46
#define EQ_CAPSLOCK		47
#define EQ_TAB			48
#define EQ_ENTER		49
#define EQ_BACKSPACE	50
#define EQ_TILDE		51

#define EQ_MAX_MOUSE_BUTTONS 5

#define EQ_MOUSE_LEFT 0
#define EQ_MOUSE_RIGHT 1
#define EQ_MOUSE_MIDDLE 2
#define EQ_MOUSE_X1 3
#define EQ_MOUSE_X2 4

namespace eq
{
	class Input
	{
		friend LRESULT CALLBACK WindowCallback(
			HWND m_WindowHandle,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	public:
		struct KeyState
		{
			bool wasDown;
			bool isDown;
		};

		struct ButtonState
		{
			bool wasDown;
			bool isDown;
		};

		struct Position
		{
			int x;
			int y;
		};

		struct MouseInputMap
		{
			ButtonState buttons[EQ_MAX_MOUSE_BUTTONS];
			Position position;
		};

		struct KeyboardInputMap
		{
			KeyState keys[EQ_MAX_KEYS];
		};

	private:
		static KeyboardInputMap keyboard;
		static MouseInputMap mouse;

	public:
		static KeyState GetKeyState(uint32_t keycode);
		static bool IsKeyPressed(uint32_t keycode);
		static bool IsKeyReleased(uint32_t keycode);
		static bool WasKeyHit(uint32_t keycode);

		static Position GetMousePosition(Math::Vector2* position = nullptr);
		static bool IsMouseButtonPressed(unsigned int buttonCode);
		static bool IsMouseButtonReleased(unsigned int buttonCode);
		static bool WasMouseButtonHit(unsigned int buttonCode);

	private:
		static void processKeyboardInput(uint32_t keycode, bool wasDown, bool isDown);

		static void processMouseInput(WPARAM wParam, LPARAM lParam);

		static void updateMousePosition(LPARAM lParam);
	};
}