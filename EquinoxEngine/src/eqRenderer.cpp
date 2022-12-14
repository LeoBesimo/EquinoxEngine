#include "eqRenderer.hpp"

#include <cstdio>

namespace eq
{
	float Renderer::alphaScaler;

	void Renderer::SetPixel(int x, int y, const Color& color)
	{
		BitmapBuffer& buffer = getActiveBuffer();

		if (x < 0 || x >= buffer.width || y < 0 || y >= buffer.height)
		{
			return;
		}
		uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue << 0);

		uint8_t* row = (uint8_t*)buffer.memory + x * s_BytesPerPixel + y * buffer.pitch;
		uint32_t* pixel = (uint32_t*)row;

		*pixel = raw_color;

	}

	void Renderer::FillRectangle(const Rect& rect, const Color& color)
	{
		BitmapBuffer& buffer = getActiveBuffer();

		int minX = rect.x;
		int minY = rect.y;
		int maxX = rect.x + rect.width;
		int maxY = rect.y + rect.height;

		if (minX < 0) minX = 0;
		if (minY < 0) minY = 0;
		if (maxX > buffer.width) maxX = buffer.width;
		if (maxY > buffer.height) maxY = buffer.height;

		uint32_t rawColor = (color.red << 16) | (color.green << 8) | (color.blue << 0);

		uint8_t* row = (uint8_t*)buffer.memory + minX * s_BytesPerPixel + minY * buffer.pitch;
		for (int y = minY; y < maxY; y++)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = minX; x < maxX; x++)
			{
				//Color pixelColor = ColorFromUInt(*pixel);
				//uint32_t rawColor = BlendColor(color, *pixel);
				*pixel++ = rawColor;
			}
			row += buffer.pitch;
		}
	}

	void Renderer::DrawRectangle(const Rect& rect, const Color& color)
	{
		BitmapBuffer& buffer = getActiveBuffer();

		int minX = rect.x;
		int minY = rect.y;
		int maxX = rect.x + rect.width;
		int maxY = rect.y + rect.height;

		if (minX < 0) minX = 0;
		if (minY < 0) minY = 0;
		if (maxX > buffer.width) maxX = buffer.width;
		if (maxY > buffer.height) maxY = buffer.height;

		for (int x = minX; x <= maxX; x++) {
			SetPixel(x, minY, color);
			SetPixel(x, maxY, color);
		}

		for (int y = minY; y <= maxY; y++) {
			SetPixel(minX, y, color);
			SetPixel(maxX, y, color);
		}
	}

	void Renderer::DrawLine(int x0, int y0, int x1, int y1, const Color& color)
	{
		if (abs(y1 - y0) < abs(x1 - x0))
		{
			if (x0 > x1)
				plotLineLow(x1, y1, x0, y0, color);
			else
				plotLineLow(x0, y0, x1, y1, color);
		}
		else
		{
			if (y0 > y1)
				plotLineHigh(x1, y1, x0, y0, color);
			else
				plotLineHigh(x0, y0, x1, y1, color);
		}
	}

	void Renderer::DrawLine(Math::Vector2 a, Math::Vector2 b, const Color& color)
	{
		DrawLine(int(a.x + 0.5f), int(a.y + 0.5f), int(b.x + 0.5f), int(b.y + 0.5f), color);
	}

	void Renderer::FillCircle(int originX, int originY, int radius, const Color& color)
	{
		for (int y = -radius; y <= radius; y++)
			for (int x = -radius; x <= radius; x++)
				if (x * x + y * y < radius * radius + radius)
					SetPixel(originX + x, originY + y, color);
	}

	void Renderer::FillEllipse(int originX, int originY, int radiusX, int radiusY, const Color& color)
	{
		for (int y = -radiusY; y <= radiusY; y++)
			for (int x = -radiusX; x <= radiusX; x++)
				if (x * x * radiusY * radiusY + y * y * radiusX * radiusX <= radiusY * radiusY * radiusX * radiusX + radiusX)
					SetPixel(originX + x, originY + y, color);
	}

	void Renderer::DrawCircle(int originX, int originY, int radius, const Color& color)
	{
		for (int y = -radius; y <= radius; y++)
			for (int x = -radius; x <= radius; x++)
				if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
					SetPixel(originX + x, originY + y, color);
	}

	void Renderer::DrawCircle(Math::Vector2 position, int radius, const Color& color)
	{
		DrawCircle(int(position.x + 0.5), int(position.y + 0.5), radius, color);
	}

	void Renderer::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color& color)
	{
		DrawLine(x0, y0, x1, y1, color);
		DrawLine(x0, y0, x2, y2, color);
		DrawLine(x1, y1, x2, y2, color);
	}

	void Renderer::draw(std::shared_ptr<Text> text)
	{
		if (text.get()->getType() == DrawableType::TEXT)
			getActiveText().push_back(text);
	}

	void Renderer::draw(std::shared_ptr<Drawable> drawable)
	{
		if (drawable.get()->getType() == DrawableType::TEXT)
		{
			//OutputDebugString(L"Adding Text object\n");
			//getActiveText().push_back(std::static_pointer_cast<Text>(drawable));
		}
		else
		{
			//OutputDebugString(L"Adding Drawable object\n");

			getActiveShapes().push_back(drawable);
		}
	}

	void Renderer::getWindowDimenstions(int* outWidth, int* outHeight)
	{
		RECT clientRect;
		GetClientRect(getInstance().m_WindowHandle, &clientRect);

		*outWidth = clientRect.right - clientRect.left;
		*outHeight = clientRect.bottom - clientRect.top;
	}

	void Renderer::resizeFrameBuffer(int width, int height)
	{
		for (unsigned int i = 0; i < 2; i++)
		{
			BitmapBuffer& buffer = getInstance().m_BitmapBuffer[i];
			if (buffer.memory)
			{
				VirtualFree(buffer.memory, 0, MEM_RELEASE);
			}

			buffer.width = width;
			buffer.height = height;

			buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
			buffer.info.bmiHeader.biWidth = buffer.width;
			buffer.info.bmiHeader.biHeight = -buffer.height;  //negative to invert buffer y direction
			buffer.info.bmiHeader.biPlanes = 1;
			buffer.info.bmiHeader.biBitCount = 32;
			buffer.info.bmiHeader.biCompression = BI_RGB;

			int bufferMemorySize = (buffer.width) * (buffer.height) * s_BytesPerPixel;
			buffer.memory = VirtualAlloc(0, bufferMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

			buffer.pitch = buffer.width * s_BytesPerPixel;
		}
	}

	void Renderer::copyBufferToWindow(HDC deviceContext, int m_WindowWidth, int m_WindowHeight)
	{
		BitmapBuffer& buffer = getInactiveBuffer();

		StretchDIBits(
			deviceContext,
			0, 0, m_WindowWidth, m_WindowHeight,
			0, 0, buffer.width, buffer.height,
			buffer.memory,
			&(buffer.info),
			DIB_RGB_COLORS,
			SRCCOPY
		);
		SetBkMode(deviceContext, TRANSPARENT);

		if (buffersSwapped() == true) {
			getInstance().m_SwappedBuffers = false;
			getActiveShapes().clear();
			getActiveText().clear();
		}
	}

	void Renderer::RenderShapes()
	{
		
	}

	void Renderer::RenderText(HDC deviceContext)
	{
		std::vector<std::shared_ptr<Text>> textBuffer = getInactiveText();

		wchar_t tb[128];
		swprintf(tb, 128, L"Size: %d\n", textBuffer.size());
		//OutputDebugString(tb);

		for (unsigned int i = 0; i < textBuffer.size(); i++)
		{	
			//OutputDebugString(L"DOING STUFF\n");
			std::shared_ptr<Text> text = textBuffer[i];//std::static_pointer_cast<Text>(textBuffer[i]);
			Color col = text.get()->getColor();
			int length = text.get()->getText().length();
			OutputDebugString(text.get()->getText().c_str());
			Math::Vector2 position = text.get()->getPosition();
			//OutputDebugString(text->getText().c_str());
			SetTextColor(deviceContext, RGB(col.red, col.green, col.blue));
			TextOut(deviceContext, std::floor(position.x), std::floor(position.y), text.get()->getText().c_str(), length);
		}
	}

	void Renderer::clear()
	{
		BitmapBuffer& buffer = getActiveBuffer();

		FillRectangle(Rect(0, 0, float(buffer.width), float(buffer.height)), getInstance().m_ClearColor);
	}

	void Renderer::plotLineLow(int x0, int y0, int x1, int y1, const Color& color)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int yi = 1;
		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		int D = (2 * dy) - dx;
		int y = y0;

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(x, y, color);
			if (D > 0)
			{
				y = y + yi;
				D = D + (2 * (dy - dx));
			}
			else
			{
				D = D + 2 * dy;
			}
		}
	}

	void Renderer::plotLineHigh(int x0, int y0, int x1, int y1, const Color& color)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int xi = 1;
		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}
		int D = (2 * dx) - dy;
		int x = x0;

		for (int y = y0; y <= y1; y++)
		{
			SetPixel(x, y, color);
			if (D > 0)
			{
				x = x + xi;
				D = D + (2 * (dx - dy));
			}
			else
			{
				D = D + 2 * dx;
			}
		}
	}

	Color Renderer::ColorFromUInt(uint32_t color)
	{
		uint8_t r = (uint8_t)color >> 16;
		uint8_t g = (uint8_t)color >> 8;
		uint8_t b = (uint8_t)color >> 0;
		return Color(r, g, b);
	}

	uint32_t Renderer::UIntFromColor(Color color)
	{
		return (color.red << 16) | (color.green << 8) | color.blue;
	}

	uint32_t Renderer::BlendColor(Color original, Color background)
	{
		float opacity = original.alpha;
		uint8_t newRed = opacity * original.red + (1 - opacity) * background.red;
		uint8_t newGreen = opacity * original.green + (1 - opacity) * background.green;
		uint8_t newBlue = opacity * original.blue + (1 - opacity) * background.blue;

		return (newRed << 16) | (newGreen << 8) | newBlue;
	}

	uint32_t Renderer::BlendColor(Color original, uint32_t background)
	{
		float opacity = original.alpha;
		/*uint8_t newRed = (opacity * original.red + (1 - opacity) * (uint8_t) (background >> 16));
		uint8_t newGreen = (opacity * original.green + (1 - opacity) * (uint8_t) (background >> 8));
		uint8_t newBlue = opacity * original.blue + (1 - opacity) * (uint8_t) (background);*/

		return ((uint8_t)(opacity * original.red + (1 - opacity) * (uint8_t)(background >> 16)) << 16) |
			((uint8_t)(opacity * original.green + (1 - opacity) * (uint8_t)(background >> 8)) << 8) |
			((uint8_t)(opacity * original.blue + (1 - opacity) * (uint8_t)(background)));
	}
	uint32_t Renderer::BlendColor(uint32_t colorA, uint32_t colorB)
	{
		uint32_t rb = (colorA & 0x00FF00FF) + (((colorB >> 24) * colorB & 0x00FF00FF) >> 8);
		uint32_t g = (colorA & 0x0000FF00) + (((colorB >> 24) * colorB & 0x0000FF00) >> 8);
		return (rb & 0x00FF00FF); +(255 & 0xFF000000) + (g & 0x0000FF00);
	}
}