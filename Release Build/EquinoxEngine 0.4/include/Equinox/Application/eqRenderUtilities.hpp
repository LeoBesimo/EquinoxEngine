#pragma once
#include <Windows.h>
#include <stdint.h>

namespace eq
{
	struct Color
	{
		uint8_t red = 0, green = 0, blue = 0, alpha = 255;

		Color() {}
		Color(uint8_t red, uint8_t green, uint8_t blue) :
			red(red), green(green), blue(blue)
		{}

		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) :
			red(red), green(green), blue(blue), alpha(alpha)
		{}

		Color(uint32_t color) :
			red(color >> 16), green(color >> 8), blue(color), alpha(color >> 24)
		{}
	};

	struct Rect
	{
		int x, y, width, height;

		Rect(float x, float y, float width, float height) :
			x(int(x + 0.5f)), y(int(y + 0.5f)), width(int(width + 0.5f)), height(int(height + 0.5f))
		{}

	};

	struct BitmapBuffer
	{
		int width, height;
		BITMAPINFO info;
		void* memory;
		int pitch; // in bytes;
	};
}