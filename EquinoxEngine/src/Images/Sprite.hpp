#pragma once

#include "Texture.hpp"

namespace eq
{
	class Sprite
	{
	private:
		std::vector<uint32_t> m_Buffer;
		unsigned int m_Width;
		unsigned int m_Height;

		friend class Renderer;

		Math::Vector2 m_Position;

		bool m_CameraDependent = true;

		uint8_t m_Alpha = 255;

	public:
		Sprite() :
			m_Width(0), m_Height(0), m_Position(0, 0) {}
		Sprite(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height), m_Buffer(width* height), m_Position(0, 0) {}

		Sprite(BitmapTexture& texture);
		Sprite(BitmapTexture& texture, unsigned int width, unsigned int height);
		Sprite(BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);
		
		void setPosition(Math::Vector2 position);
		Math::Vector2 getPosition();
		void move(Math::Vector2 direction);

		void scale(float scaleX, float scaleY);
		void setAlpha(uint8_t alpha);
		uint8_t getAlpha();

		uint32_t getPixel(unsigned int x, unsigned int y);
	};
}