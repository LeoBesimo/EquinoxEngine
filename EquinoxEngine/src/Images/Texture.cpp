#include "Texture.hpp"

namespace eq
{
	uint32_t BitmapTexture::getPixel(unsigned int x, unsigned int y)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return 0;
		}

		return (255 << 24) |  (m_Buffer[x + y * m_Width] & 0xFFFFFF);
	}

	void BitmapTexture::setPixel(unsigned int x, unsigned int y, uint32_t color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
		{
			return;
		}

		m_Buffer[x + y * m_Width] = color;
	}

	void BitmapTexture::invertY()
	{
		std::vector<uint32_t> temp(m_Buffer.size());
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				unsigned int indexReversed = i + (m_Height - 1 - j) * m_Width;
				temp[indexReversed] = m_Buffer[index];
			}
		}

		m_Buffer = temp;
	}

	void BitmapTexture::invertX()
	{
		std::vector<uint32_t> temp(m_Buffer.size());
		for (unsigned int j = 0; j < m_Height; j++)
		{
			for (unsigned int i = 0; i < m_Width; i++)
			{
				unsigned int index = i + j * m_Width;
				unsigned int indexReversed = (m_Width - 1 - i) + j * m_Width;
				temp[indexReversed] = m_Buffer[index];
			}
		}

		m_Buffer = temp;
	}

	bool BitmapTexture::read(const char* path)
	{
		std::ifstream in;
		in.open(path, std::ios::in | std::ios::binary);

		if (!in.is_open())
			return false;
		
		in.read(reinterpret_cast<char*>(&m_FileHeader), sizeof(BitmapFileHeader));
		in.read(reinterpret_cast<char*>(&m_InfoHeader), sizeof(BITMAPINFOHEADER));

		if(m_FileHeader.signature)

		if (m_InfoHeader.bitsPerPixel <= 3)
		{
			return false;
		}

		// Determine the size of the pixel data
		const int width = m_InfoHeader.width;
		const int height = m_InfoHeader.height;
		const int numPixels = width * height;

		m_Width = width;
		m_Height = height;

		// Seek to the start of the pixel data in the file
		in.seekg(m_FileHeader.offsetToPixelData, std::ios::beg);

		// Create an std::vector to store the pixel data
		std::vector<uint32_t> pixels(numPixels);
		m_Buffer.resize(numPixels);
		// Read the pixel data from the file into the vector
		in.read(reinterpret_cast<char*>(m_Buffer.data()), numPixels * sizeof(uint32_t));

		// Close the file
		in.close();

		// You can now access the pixel data stored in the vector using the [] operator,
		// e.g. pixels[0] is the first pixel in the image, pixels[1] is the second pixel, etc.

		return 0;
	}

	bool BitmapTexture::save(const char* path) const
	{
		std::ofstream out;
		out.open(path, std::ios::out | std::ios::binary);

		if (!out.is_open())
			return false;

		// Create variables to store the file header and info header
		BitmapFileHeader fileHeader;
		BitmapInfoHeader infoHeader;

		// Fill the file header and info header with the appropriate values
		fileHeader.signature[0] = 'B';
		fileHeader.signature[1] = 'M';
		fileHeader.fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + m_Buffer.size() * sizeof(uint32_t);
		fileHeader.reserved1 = 0;
		fileHeader.reserved2 = 0;
		fileHeader.offsetToPixelData = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

		infoHeader.headerSize = sizeof(BitmapInfoHeader);
		infoHeader.width = m_Width;
		infoHeader.height = m_Height;
		infoHeader.planes = 1;
		infoHeader.bitsPerPixel = 32;
		infoHeader.compression = 0;
		infoHeader.imageSize = m_Buffer.size() * sizeof(uint32_t);
		infoHeader.xResolution = 0;
		infoHeader.yResolution = 0;
		infoHeader.colorsInPalette = 0;
		infoHeader.importantColors = 0;

		// Write the file header and info header to the file
		out.write(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));
		out.write(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeader));

		//std::reverse(m_Buffer.begin(), m_Buffer.end());
		// Write the pixel data to the file
		out.write(reinterpret_cast<char*>(const_cast<uint32_t*>(m_Buffer.data())), m_Buffer.size() * sizeof(uint32_t));

		// Close the file
		out.close();

		return true;
	}
}