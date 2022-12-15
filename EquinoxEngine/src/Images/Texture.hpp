#pragma once

#include <eqMath.hpp>
#include <vector>
#include <iostream>

namespace eq
{
	class BitmapTexture
	{
	private:

		unsigned int m_Width;
		unsigned int m_Height;
		std::vector<uint32_t> m_Buffer;

	public:
		BitmapTexture() :
			m_Width(0), m_Height(0) {}
		BitmapTexture(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height) {}

		uint32_t getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, uint32_t color);

		void save(const char* filePath);
	};
}