#include "Sprite.hpp"

namespace eq
{
	Sprite::Sprite(BitmapTexture& texture) :
		m_Position(0, 0)
	{
		m_Width = texture.m_Width;
		m_Height = texture.m_Height;

		m_Buffer = std::vector<uint32_t>(m_Width * m_Height);

		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{

				unsigned int index = i + j * m_Width;
				m_Buffer[index] = texture.m_Buffer[index];
			}
		}
	}

	Sprite::Sprite(BitmapTexture& texture, unsigned int width, unsigned int height) :
		m_Width(width), m_Height(height), m_Buffer(width* height), m_Position(0, 0)
	{
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				m_Buffer[index] = texture.getPixel(i, j);
			}
		}
	}

	Sprite::Sprite(BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height) :
		m_Width(width), m_Height(height), m_Buffer(width* height), m_Position(0, 0)
	{
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				m_Buffer[index] = texture.getPixel(i + xOffset, j + yOffset);
			}
		}
	}

	void Sprite::setPosition(Math::Vector2 position)
	{
		m_Position = position;
	}

	Math::Vector2 Sprite::getPosition()
	{
		return m_Position;
	}

	void Sprite::move(Math::Vector2 direction)
	{
		m_Position += direction;
	}

	void Sprite::scale(float scaleX, float scaleY)
	{
		float newWidth = m_Width * scaleX;
		float newHeight = m_Height * scaleY;

		std::vector<uint32_t> tempBuffer(newWidth * newHeight);
		;

		for (unsigned int y = 0; y < newHeight; y++)
		{
			for (unsigned int x = 0; x < newWidth; x++)
			{
				unsigned int nearestX = (int)floor(x / scaleX);
				unsigned int nearestY = (int)floor(y / scaleY);
				unsigned int nearestIndex = nearestX + nearestY * m_Width;

				unsigned int pixelIndex = x + y * newWidth;
				uint32_t color = m_Buffer[nearestIndex];
				tempBuffer[pixelIndex] = color;
			}
		}

		m_Width = newWidth;
		m_Height = newHeight;

		m_Buffer = tempBuffer;
	}
	void Sprite::setAlpha(uint8_t alpha)
	{
		m_Alpha = alpha;
	}

	uint8_t Sprite::getAlpha()
	{
		return m_Alpha;
	}
	uint32_t Sprite::getPixel(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return 0;
		}

		uint32_t col = m_Buffer[x + y * m_Width];
		uint8_t red = (uint8_t)(col >> 16);
		uint8_t green = (uint8_t)(col >> 8);
		uint8_t blue = (uint8_t)(col >> 0);

		return col;//(m_Alpha << 24) | (red << 16) | (green << 8) | blue;
	}
}