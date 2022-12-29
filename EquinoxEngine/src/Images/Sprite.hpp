#pragma once

#include "Texture.hpp"

namespace eq
{
	class Sprite
	{
	private:
		std::vector<uint32_t> m_Buffer;
		std::vector<uint32_t> m_OriginalBuffer;
		unsigned int m_OriginalWidth;
		unsigned int m_OriginalHeight;
		unsigned int m_Width;
		unsigned int m_Height;
		float m_Angle;

		Math::Vector2 m_Scale;

		friend class Renderer;

		Math::Vector2 m_Position;

		bool m_CameraDependent = true;


	public:
		Sprite() :
			m_Width(0), m_Height(0), m_Position(0, 0), m_Scale(1, 1) {}
		Sprite(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height), m_Buffer(width* height), m_Position(0, 0), m_Scale(1, 1) {}

		Sprite(BitmapTexture& texture);
		Sprite(BitmapTexture& texture, unsigned int width, unsigned int height);
		Sprite(BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);

		void setPosition(Math::Vector2 position);
		Math::Vector2 getPosition();
		void move(Math::Vector2 direction);

		void rotate(float ang);

		void scale(float scaleX, float scaleY);

		uint32_t getPixel(unsigned int x, unsigned int y);

	private:
		uint32_t billinearInterpolation(float x, float y);
	};
}