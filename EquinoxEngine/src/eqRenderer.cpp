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
		uint32_t raw_color = (color.alpha << 24) | (color.red << 16) | (color.green << 8) | (color.blue << 0);

		uint8_t* row = (uint8_t*)buffer.memory + x * s_BytesPerPixel + y * buffer.pitch;
		uint32_t* pixel = (uint32_t*)row;

		*pixel = BlendColor(*pixel, raw_color);//raw_color;


	}

	void Renderer::SetPixel(int x, int y, const uint32_t color)
	{
		BitmapBuffer& buffer = getActiveBuffer();

		if (x < 0 || x >= buffer.width || y < 0 || y >= buffer.height)
		{
			return;
		}

		uint8_t* row = (uint8_t*)buffer.memory + x * s_BytesPerPixel + y * buffer.pitch;
		uint32_t* pixel = (uint32_t*)row;

		*pixel = BlendColor(*pixel, color);//raw_color;
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

	void Renderer::DrawSprite(Sprite& sprite)
	{
		Math::Vector2 position = sprite.m_Position;
		if (sprite.m_CameraDependent)
			position += getInstance().m_Camera.get()->getPosition();

		for (unsigned int j = 0; j < sprite.m_Height; j++)
		{
			for (unsigned int i = 0; i < sprite.m_Width; i++)
			{
				SetPixel(position.x + i, position.y + j, sprite.getPixel(i, j));
			}
		}

	}

	void Renderer::draw(std::shared_ptr<Ellipse> ellipse)
	{
		if (ellipse.get()->getType() == DrawableType::ELLIPSE || ellipse.get()->getType() == DrawableType::CIRCLE)
			getActiveEllipses().push_back(ellipse);
	}

	void Renderer::draw(std::shared_ptr<Line> line)
	{
		if (line.get()->getType() == DrawableType::LINE)
			getActiveLines().push_back(line);
	}

	void Renderer::draw(std::shared_ptr<Rectangle> rectangle)
	{
		if (rectangle.get()->getType() == DrawableType::RECT)
			getActiveRectangles().push_back(rectangle);
	}

	void Renderer::draw(std::shared_ptr<Text> text)
	{
		if (text.get()->getType() == DrawableType::TEXT)
			getActiveText().push_back(text);
	}

	void Renderer::draw(std::shared_ptr<Sprite> sprite)
	{
		getActiveSprites().push_back(sprite);
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
			clearBuffers();
		}
	}

	void Renderer::clear()
	{
		BitmapBuffer& buffer = getActiveBuffer();

		FillRectangle(Rect(0, 0, float(buffer.width), float(buffer.height)), getInstance().m_ClearColor);
	}

	void Renderer::clearBuffers()
	{
		getActiveEllipses().clear();
		getActiveLines().clear();
		getActiveRectangles().clear();
		getActiveText().clear();
		getActiveSprites().clear();
	}

	void Renderer::RenderObjects()
	{
		RenderRectangles();
		RenderLines();
		RenderEllipses();
		RenderSprites();
	}

	void Renderer::RenderText(HDC deviceContext)
	{
		std::vector<std::shared_ptr<Text>> textBuffer = getInactiveText();

		for (unsigned int i = 0; i < textBuffer.size(); i++)
		{
			std::shared_ptr<Text> text = textBuffer[i];
			Color col = text.get()->getColor();
			int length = text.get()->getText().length();
			Math::Vector2 position = text.get()->getPosition();
			if (text.get()->isCameraDependent())
				position += getInstance().m_Camera.get()->getPosition();
			SetTextColor(deviceContext, RGB(col.red, col.green, col.blue));
			TextOut(deviceContext, std::floor(position.x + 0.5f), std::floor(position.y + 0.5f), text.get()->getText().c_str(), length);
		}
	}

	void Renderer::RenderRectangles()
	{
		std::vector<std::shared_ptr<Rectangle>> rectBuffer = getInactiveRectangles();

		for (unsigned int i = 0; i < rectBuffer.size(); i++)
		{
			std::shared_ptr<Rectangle> rect = rectBuffer[i];

			Math::Vector2 pos = rect.get()->getPosition();
			if (rect.get()->isCameraDependent())
				pos += getInstance().m_Camera.get()->getPosition();
			Math::Vector2 dim = rect.get()->getDimension();
			Rect r(pos.x, pos.y, dim.x, dim.y);
			DrawRectangle(r, rect.get()->getColor());
		}
	}

	void Renderer::RenderEllipses()
	{
		std::vector<std::shared_ptr<Ellipse>> ellipseBuffer = getInactiveEllipses();

		for (unsigned int i = 0; i < ellipseBuffer.size(); i++)
		{
			std::shared_ptr<Ellipse> ellipse = ellipseBuffer[i];

			Math::Vector2 pos = ellipse.get()->getPosition();
			if (ellipse.get()->isCameraDependent())
				pos += getInstance().m_Camera.get()->getPosition();
			if (ellipse.get()->getType() == DrawableType::ELLIPSE)
			{
				float r1, r2;
				ellipse.get()->getRadius(&r1, &r2);
				FillEllipse(pos.x, pos.y, r1, r2, ellipse.get()->getColor());
			}
			else
			{
				float r;
				ellipse.get()->getRadius(&r);
				DrawCircle(pos, r, ellipse.get()->getColor());
			}
		}
	}

	void Renderer::RenderLines()
	{
		std::vector<std::shared_ptr<Line>> lineBuffer = getInactiveLines();

		for (unsigned int i = 0; i < lineBuffer.size(); i++)
		{
			std::shared_ptr<Line> line = lineBuffer[i];

			Math::Vector2 pos1 = line.get()->getStartPos();
			Math::Vector2 pos2 = line.get()->getEndPos();
			if (line.get()->isCameraDependent())
			{
				pos1 += getInstance().m_Camera.get()->getPosition();
				pos2 += getInstance().m_Camera.get()->getPosition();
			}
			DrawLine(pos1, pos2, line.get()->getColor());
		}
	}

	void Renderer::RenderSprites()
	{
		std::vector<std::shared_ptr<Sprite>> sprites = getInactiveSprites();
		for (unsigned int i = 0; i < sprites.size(); i++)
		{
			DrawSprite(*sprites[i]);
		}
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

	uint32_t Renderer::BlendColor(uint32_t colorA, uint32_t colorB)
	{
		uint8_t alpha = colorB >> 24;
		uint32_t rb1 = ((0x100 - alpha) * (colorA & 0xFF00FF)) >> 8;
		uint32_t rb2 = (alpha * (colorB & 0xFF00FF)) >> 8;
		uint32_t g1 = ((0x100 - alpha) * (colorA & 0x00FF00)) >> 8;
		uint32_t g2 = (alpha * (colorB & 0x00FF00)) >> 8;
		return ((rb1 | rb2) & 0xFF00FF) + ((g1 | g2) & 0x00FF00);
	}
}