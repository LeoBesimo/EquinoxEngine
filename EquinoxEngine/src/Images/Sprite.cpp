#include "Sprite.hpp"

namespace eq
{
	Sprite::Sprite(BitmapTexture& texture) :
		m_Position(0, 0), m_Scale(1, 1)
	{
		m_Width = texture.m_Width;
		m_Height = texture.m_Height;

		m_OriginalBuffer = std::vector<uint32_t>(m_Width * m_Height);

		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{

				unsigned int index = i + j * m_Width;
				m_OriginalBuffer[index] = texture.m_Buffer[index];
			}
		}
	}

	Sprite::Sprite(BitmapTexture& texture, unsigned int width, unsigned int height) :
		m_Width(width), m_Height(height), m_OriginalBuffer(width* height), m_Position(0, 0), m_Scale(1, 1)
	{
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				m_OriginalBuffer[index] = texture.getPixel(i, j);
			}
		}
	}

	Sprite::Sprite(BitmapTexture& texture, unsigned int xOffset, unsigned int yOffset, unsigned int width, unsigned int height) :
		m_Width(width), m_Height(height), m_OriginalBuffer(width* height), m_Position(0, 0), m_Scale(1, 1)
	{
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				m_OriginalBuffer[index] = texture.getPixel(i + xOffset, j + yOffset);
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

	void Sprite::rotate(float angle)
	{
		m_Angle = angle;
		m_Changed = true;
	}

	void Sprite::scale(float scaleX, float scaleY)
	{
		m_Scale.x = scaleX;
		m_Scale.y = scaleY;
		m_OriginalScale = m_Scale;
		m_Changed = true;
	}

	uint32_t Sprite::getPixel(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return 0;
		}

		uint32_t col = m_OriginalBuffer[x + y * m_Width];
		//uint8_t red = (uint8_t)(col >> 16);
		//uint8_t green = (uint8_t)(col >> 8);
		//uint8_t blue = (uint8_t)(col >> 0);

		return col;//(m_Alpha << 24) | (red << 16) | (green << 8) | blue;
	}

	void Sprite::preprocessSprite()
	{
		m_Changed = false;
		m_PreTransformed = true;
		applyScaling();
		applyRotation();
	}

	uint32_t Sprite::getTransformedPixel(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= m_ScaledWidth || y < 0 || y >= m_ScaledHeight || m_Buffer.size() == 0)
		{
			return 0;
		}

		return m_Buffer[x + y * m_ScaledWidth];
	}

	void Sprite::applyScaling()
	{
		float newWidth = floor(m_Width * m_Scale.x + 0.5f);
		float newHeight = floor(m_Height * m_Scale.y + 0.5f);

		std::vector<uint32_t> tempBuffer(newWidth * newHeight);

		for (unsigned int y = 0; y < newHeight; y++)
		{
			for (unsigned int x = 0; x < newWidth; x++)
			{
				unsigned int nearestX = (int)floor(x / m_Scale.x);
				unsigned int nearestY = (int)floor(y / m_Scale.y);
				unsigned int nearestIndex = nearestX + nearestY * m_Width;

				unsigned int pixelIndex = x + y * newWidth;
				uint32_t color = m_OriginalBuffer[nearestIndex];
				tempBuffer[pixelIndex] = color;
			}
		}

		m_ScaledWidth = newWidth;
		m_ScaledHeight = newHeight;

		m_Buffer = tempBuffer;
	}

	void Sprite::applyRotation()
	{
		Math::Matrix2x2 rotation(-m_Angle);

		std::vector<uint32_t> tempBuffer(m_Buffer.size());
		float xOff = m_ScaledWidth / 2;
		float yOff = m_ScaledHeight / 2;
		Math::Vector2 offset(xOff, yOff);

		for (unsigned int i = 0; i < m_ScaledWidth; i++)
		{
			for (unsigned int j = 0; j < m_ScaledHeight; j++)
			{
				Math::Vector2 original(i - xOff, j - yOff);
				original = rotation * original;
				original += offset;
				tempBuffer[i + j * m_ScaledWidth] = billinearInterpolation(original.x, original.y);
			}
		}
		m_Buffer = tempBuffer;
	}

	uint32_t Sprite::billinearInterpolation(float x, float y)
	{
		if (x < 0 || x >= m_ScaledWidth || y < 0 || y >= m_ScaledHeight)
			return 0;

		int x1 = floor(x);
		int x2 = ceil(x);
		int y1 = floor(y);
		int y2 = ceil(y);

		float dx1 = x - x1;
		float dx2 = x - x2;
		float dy1 = y - y1;
		float dy2 = y - y2;

		uint32_t value = m_Buffer[x1 + y1 * m_ScaledWidth];/*m_Buffer[x1 + y1 * m_Width] * dx2 * dy2 +
			m_Buffer[x2 + y1 * m_Width] * dx1 * dy2 +
			m_Buffer[x1 + y2 * m_Width] * dx2 * dy1 +
			m_Buffer[x2 + y2 * m_Width] * dx1 + dy1;*/

		return value;

	}
}