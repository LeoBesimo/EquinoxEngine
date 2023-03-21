#pragma once

#include "Texture.hpp"

namespace eq
{
	class Sprite
	{
	private:
		std::vector<uint32_t> m_Buffer;
		std::vector<uint32_t> m_OriginalBuffer;
		unsigned int m_Width;
		unsigned int m_Height;

		unsigned int m_ScaledWidth;
		unsigned int m_ScaledHeight;

		float m_Angle = 0;
		Math::Vector2 m_OriginalScale;
		Math::Vector2 m_Scale;

		friend class Renderer;
		friend class BitmapTexture;

		Math::Vector2 m_Position;

		bool m_CameraDependent = true;
		bool m_PreTransformed = false;
		bool m_Changed = false;


	public:
		Sprite() :
			m_Width(0), m_Height(0), m_Position(0, 0), m_Scale(1, 1), m_OriginalScale(1, 1) {}
		Sprite(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height), m_Buffer(width* height), m_Position(0, 0), m_Scale(1, 1), m_OriginalScale(1, 1) {}

		Sprite(BitmapTexture& texture);
		Sprite(BitmapTexture& texture, unsigned int width, unsigned int height);
		Sprite(BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height);

		void setPosition(Math::Vector2 position);
		Math::Vector2 getPosition();
		void move(Math::Vector2 direction);

		void rotate(float angle);
		void scale(float scaleX, float scaleY);

		uint32_t getPixel(unsigned int x, unsigned int y);

		bool isCameraDependent() { return m_CameraDependent; }
		void setCameraDependent(bool dependent) { m_CameraDependent = dependent; }

		void preprocessSprite();

		Math::Vector2 getScaledSize() { return Math::Vector2(m_ScaledWidth, m_ScaledHeight); }
		Math::Vector2 getSize() { return Math::Vector2(m_Width, m_Height); }

	private:
		Math::Vector2 getScale() { return m_Scale; }
		void setScale(Math::Vector2 scale) { m_Scale = scale; }

		uint32_t getTransformedPixel(unsigned int x, unsigned int y);
		void applyScaling();
		void applyRotation();
		uint32_t billinearInterpolation(float x, float y);
	};
}