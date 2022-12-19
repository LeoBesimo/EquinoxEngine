#pragma once

#include <eqMath.hpp>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace eq
{
#pragma pack(push, 1) // This ensures that the struct is packed tightly, with no padding bytes between fields
	struct BitmapFileHeader {
		char signature[2]; // The file signature, "BM"
		unsigned int fileSize; // The size of the file in bytes
		unsigned short reserved1; // The reserved field, set to zero
		unsigned short reserved2; // The reserved field, set to zero
		unsigned int offsetToPixelData; // The offset, in bytes, to the start of the pixel data
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct BitmapInfoHeader {
		unsigned int headerSize; // The size of the header, in bytes
		int width; // The width of the bitmap, in pixels
		int height; // The height of the bitmap, in pixels
		unsigned short planes; // The number of planes
		unsigned short bitsPerPixel; // The number of bits per pixel
		unsigned int compression; // The compression method used to encode the pixel data
		unsigned int imageSize; // The size of the image data, in bytes
		int xResolution; // The horizontal resolution, in pixels per meter
		int yResolution; // The vertical resolution, in pixels per meter
		unsigned int colorsInPalette; // The number of colors in the color palette
		unsigned int importantColors; // The number of important colors in the image
	};
#pragma pack(pop)


	class BitmapTexture
	{
	private:

		BitmapFileHeader m_FileHeader;
		BitmapInfoHeader m_InfoHeader;

		unsigned int m_Width = 0;
		unsigned int m_Height = 0;
		std::vector<uint32_t> m_Buffer;

		friend class Sprite;

	public:
		BitmapTexture() :
			m_Width(0), m_Height(0) {}
		BitmapTexture(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height), m_Buffer(width* height) {}

		uint32_t getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, uint32_t color);

		void invertY();
		void invertX();

		bool read(const char* filePaht);
		bool save(const char* filePath) const;
	};
}