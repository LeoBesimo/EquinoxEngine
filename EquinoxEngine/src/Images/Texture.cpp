#include "Texture.hpp"

namespace eq
{
	uint32_t BitmapTexture::getPixel(unsigned int x, unsigned int y)
	{
		if (!0 <= x < m_Width || !(0 <= y < m_Height))
		{
			return 0;
		}

		return m_Buffer[x + y * m_Width];
	}

	void BitmapTexture::setPixel(unsigned int x, unsigned int y, uint32_t color)
	{
		if (!0 <= x < m_Width || !(0 <= y < m_Height))
		{
			return;
		}

		m_Buffer[x + y * m_Width] = color;
	}
}