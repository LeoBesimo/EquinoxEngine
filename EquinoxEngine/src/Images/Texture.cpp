#include "Texture.hpp"

namespace eq
{
	uint32_t BitmapTexture::getPixel(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return 0;
		}

		return m_Buffer[x + y * m_Width];
	}

	void BitmapTexture::setPixel(unsigned int x, unsigned int y, uint32_t color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return;
		}

		m_Buffer[x + y * m_Width] = color;
	}

	bool BitmapTexture::save(const char* path) const
	{
		std::ofstream out;
		out.open(path, std::ios::out | std::ios::binary);

		if (!out.is_open())
			return false;

		unsigned char bmpPad[3] = { 0,0,0 };
		const int paddingAmmount = ((4 - (m_Width * 3) % 4) % 4);

		const int fileHeaderSize = 14;
		const int informationHeaderSize = 40;

		const int fileSize = fileHeaderSize + informationHeaderSize + m_Width * m_Height * 3 + paddingAmmount * m_Height;

		// fileHeader

		unsigned char fileHeader[fileHeaderSize];

		fileHeader[0] = 'B';
		fileHeader[1] = 'M';

		fileHeader[2] = fileSize;
		fileHeader[3] = fileSize >> 8;
		fileHeader[4] = fileSize >> 16;
		fileHeader[5] = fileSize >> 24;

		fileHeader[6] = 0;
		fileHeader[7] = 0;
		fileHeader[8] = 0;
		fileHeader[9] = 0;

		fileHeader[10] = fileHeaderSize + informationHeaderSize;
		fileHeader[11] = 0;
		fileHeader[12] = 0;
		fileHeader[13] = 0;

		//informationHeader
		
		unsigned char informationHeader[informationHeaderSize];

		informationHeader[0] = informationHeaderSize;
		informationHeader[1] = 0;
		informationHeader[2] = 0;
		informationHeader[3] = 0;

		informationHeader[4] = m_Width;
		informationHeader[5] = m_Width >> 8;
		informationHeader[6] = m_Width >> 16;
		informationHeader[7] = m_Width >> 24;
		
		informationHeader[8] = m_Height;
		informationHeader[9] = m_Height >> 8;
		informationHeader[10] = m_Height >> 16;
		informationHeader[11] = m_Height >> 24;
		
		// Planes
		informationHeader[12] = 1;
		informationHeader[13] = 0;

		//Bits per pixel(RGB)
		informationHeader[14] = 24;
		informationHeader[15] = 0;

		for (unsigned int i = 16; i < informationHeaderSize; i++)
		{
			informationHeader[i] = 0;
		}
		/*
		// Compression
		informationHeader[16] = 0;
		informationHeader[17] = 0;
		informationHeader[18] = 0;
		informationHeader[19] = 0;

		// Image size
		informationHeader[20] = 0;
		informationHeader[21] = 0;
		informationHeader[22] = 0;
		informationHeader[23] = 0;

		// x pixels per meter
		informationHeader[24] = 0;
		informationHeader[25] = 0;
		informationHeader[26] = 0;
		informationHeader[27] = 0;

		// y pixels per meter
		informationHeader[28] = 0;
		informationHeader[29] = 0;
		informationHeader[30] = 0;
		informationHeader[31] = 0;

		// total colors;
		informationHeader[32] = 0;
		informationHeader[33] = 0;
		informationHeader[34] = 0;
		informationHeader[35] = 0;

		// important colors
		informationHeader[36] = 0;
		informationHeader[37] = 0;
		informationHeader[38] = 0;
		informationHeader[39] = 0;
		*/


		out.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
		out.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

		wchar_t* textBuffer[128];


		for (unsigned int i = 0; i < m_Height; i++)
		{
			for (unsigned int j = 0; j < m_Width; j++)
			{
				uint32_t color = m_Buffer[j + i * m_Width];
				uint8_t red = (uint8_t)(color >> 16);
				uint8_t green = (uint8_t)(color >> 8);
				uint8_t blue = (uint8_t)(color >> 0);
				unsigned char r = static_cast<unsigned char>(red);
				unsigned char g = static_cast<unsigned char>(green);
				unsigned char b = static_cast<unsigned char>(blue);

				//swprintf(textBuffer, 128, L"%d : %d : %d", (unsigned int) red, (unsigned int)green, (unsigned int)blue);
				//OutputDebugString(textBuffer);
				std::wstringstream ss;
				//ss << red << " " << green << " " << blue << "\n";
				//LPCWSTR sw = ss.str().c_str();
				//OutputDebugString(sw);

				unsigned char col[] = { b, g, r };

				out.write(reinterpret_cast<char*>(col),3);
			}
			out.write(reinterpret_cast<char*>(bmpPad), paddingAmmount);
		}

		out.close();

		return true;
	}
}