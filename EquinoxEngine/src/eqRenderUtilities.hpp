#pragma once
#include <Windows.h>
#include <stdint.h>

namespace eq
{
	struct Color
	{
		uint8_t red = 0, green = 0, blue = 0, alpha;

		Color() {}

		Color(uint8_t red, uint8_t green, uint8_t blue) :
			red(red), green(green), blue(blue), alpha(255)
		{}

		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
			red(red), green(green), blue(blue), alpha(alpha)
		{}

	};

	struct Rect
	{
		int x, y, width, height;

		Rect(float x, float y, float width, float height) :
			x(int(x + 0.5f)), y(int(y + 0.5f)), width(int(width + 0.5f)), height(int(height + 0.5f))
		{}

	};

	struct Text
	{
		int x, y;
		int length;
		const wchar_t* text;
		Color color;
	};

	struct BitmapBuffer
	{
		int width, height;
		BITMAPINFO info;
		void* memory;
		int pitch; // in bytes;
	};
}