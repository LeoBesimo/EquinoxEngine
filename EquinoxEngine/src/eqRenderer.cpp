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
		Math::Matrix2x2 rotation(0);

		Math::Vector2 position = sprite.m_Position;
		Math::Vector2 scale = sprite.m_Scale;
		if (sprite.m_CameraDependent)
		{
			Math::Matrix2x2 original = sprite.m_Scale;
			if(!sprite.m_PreTransformed || sprite.m_Changed) sprite.m_Scale = (getInstance().m_Camera->getTransform() * sprite.m_OriginalScale);
			position = WorldToScreenspace(position); //+= getInstance().m_Camera.get()->getPosition();
		}

		if (!sprite.m_PreTransformed || sprite.m_Changed)
		{
			sprite.applyScaling();
			sprite.applyRotation();
			sprite.m_Changed = false;
		}

		for (unsigned int j = 0; j < sprite.m_ScaledHeight; j++)
		{
			for (unsigned int i = 0; i < sprite.m_ScaledWidth; i++)
			{
				Math::Vector2 targetPos = Math::Vector2(i, j);
				targetPos = rotation * targetPos;
				targetPos += position;
				SetPixel(targetPos.x + 0.5, targetPos.y + 0.5, sprite.getTransformedPixel(i, j));
			}
		}

	}

	void Renderer::draw(Ellipse ellipse)
	{
		if (ellipse.getType() == DrawableType::ELLIPSE || ellipse.getType() == DrawableType::CIRCLE)
			getActiveEllipses().push_back(ellipse);
	}

	void Renderer::draw(Line line)
	{
		if (line.getType() == DrawableType::LINE)
			getActiveLines().push_back(line);
	}

	void Renderer::draw(Rectangle rectangle)
	{
		if (rectangle.getType() == DrawableType::RECT)
			getActiveRectangles().push_back(rectangle);
	}

	void Renderer::draw(Text text)
	{
		if (text.getType() == DrawableType::TEXT)
			getActiveText().push_back(text);
	}

	void Renderer::draw(Sprite sprite)
	{
		getActiveSprites().push_back(sprite);
	}

	void Renderer::draw(Physics::PhysicsWorld& world)
	{
		std::vector<Physics::Shape*> bodies = world.getBodies();
		for (Physics::Shape* body : bodies)
		{
			switch (body->getShapeType())
			{
			case Physics::ShapeType::Box:
			{
				Physics::BoxShape* box = static_cast<Physics::BoxShape*>(body);
				std::vector<Math::Vector2> points = box->getCorners();
				for (unsigned int i = 0; i < points.size(); i++)
				{
					unsigned int index = (i + 1) % points.size();
					Line line(points[i], points[index]);
					draw(line);
				}

				break;
			}
			case Physics::ShapeType::Circle:
			{
				Physics::CircleShape* circle = static_cast<Physics::CircleShape*>(body);
				Ellipse shape(circle->getPosition(), circle->getRadius());
				Math::Vector2 p2(circle->getRadius() * cos(circle->getAngle()), circle->getRadius() * sin(circle->getAngle()));
				Line line(circle->getPosition(), p2 + circle->getPosition());
				draw(line);
				draw(shape);
				break;
			}
			case Physics::ShapeType::Polygon:
			{
				Physics::PolygonShape* polygon = static_cast<Physics::PolygonShape*>(body);
				std::vector<Math::Vector2> points = polygon->getCorners();
				for (unsigned int i = 0; i < points.size(); i++)
				{
					unsigned int index = (i + 1) % points.size();
					Line line(points[i], points[index]);
					draw(line);
				}
				break;
			}
			}
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

		if (BuffersSwapped() == true) {
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
		std::vector<Text> textBuffer = getInactiveText();

		for (unsigned int i = 0; i < textBuffer.size(); i++)
		{
			Text text = textBuffer[i];
			Color col = text.getColor();
			int length = text.getText().length();
			Math::Vector2 position = text.getPosition();
			if (text.isCameraDependent())
				position = WorldToScreenspace(position);//+= getInstance().m_Camera.get()->getPosition();
			SetTextColor(deviceContext, RGB(col.red, col.green, col.blue));
			TextOut(deviceContext, std::floor(position.x + 0.5f), std::floor(position.y + 0.5f), text.getText().c_str(), length);
		}
	}

	void Renderer::RenderRectangles()
	{
		std::vector<Rectangle> rectBuffer = getInactiveRectangles();

		for (unsigned int i = 0; i < rectBuffer.size(); i++)
		{
			Rectangle rect = rectBuffer[i];

			Math::Vector2 pos = rect.getPosition();
			Math::Vector2 dim = rect.getDimension();

			if (rect.isCameraDependent())
			{
				pos = WorldToScreenspace(pos);
				dim = ApplyCameraTransform(dim);

			}
			Rect r(pos.x, pos.y, dim.x, dim.y);
			DrawRectangle(r, rect.getColor());
		}
	}

	void Renderer::RenderEllipses()
	{
		std::vector<Ellipse> ellipseBuffer = getInactiveEllipses();

		for (unsigned int i = 0; i < ellipseBuffer.size(); i++)
		{
			Ellipse ellipse = ellipseBuffer[i];

			Math::Vector2 pos = ellipse.getPosition();
			if (ellipse.isCameraDependent())
				pos = WorldToScreenspace(pos);
			if (ellipse.getType() == DrawableType::ELLIPSE)
			{
				Math::Vector2 r;
				ellipse.getRadius(&r.x, &r.y);
				if (ellipse.isCameraDependent()) r = ApplyCameraTransform(r);
				FillEllipse(pos.x, pos.y, r.x, r.y, ellipse.getColor());
			}
			else
			{
				float r;
				ellipse.getRadius(&r);
				Math::Vector2 rad(r, r);
				if (ellipse.isCameraDependent()) rad = ApplyCameraTransform(rad);
				DrawCircle(pos, rad.x, ellipse.getColor());
			}
		}
	}

	void Renderer::RenderLines()
	{
		std::vector<Line> lineBuffer = getInactiveLines();

		for (unsigned int i = 0; i < lineBuffer.size(); i++)
		{
			Line line = lineBuffer[i];

			Math::Vector2 pos1 = line.getStartPos();
			Math::Vector2 pos2 = line.getEndPos();
			if (line.isCameraDependent())
			{
				pos1 = WorldToScreenspace(pos1);//+= getInstance().m_Camera.get()->getPosition();
				pos2 = WorldToScreenspace(pos2);//+= getInstance().m_Camera.get()->getPosition();
			}
			DrawLine(pos1, pos2, line.getColor());
		}
	}

	void Renderer::RenderSprites()
	{
		std::vector<Sprite> sprites = getInactiveSprites();
		for (unsigned int i = 0; i < sprites.size(); i++)
		{
			DrawSprite(sprites[i]);
		}
	}

	Math::Vector2 Renderer::WorldToScreenspace(Math::Vector2 p)
	{
		p.y = -p.y;
		p = ApplyCameraTransform(p);
		p = ApplyCameraPosition(p);
		return p;
	}

	Math::Vector2 Renderer::ScreenToWorldspace(Math::Vector2 p)
	{
		//p.y = -p.y;
		p -= getInstance().m_Camera->getPosition();

		Math::Matrix2x2 transform = getInstance().m_Camera->getTransform();
		Math::Matrix2x2 invTransform = transform.inverse();
		p = invTransform * p;
		p.y = -p.y;
		return p;
	}

	Math::Vector2 Renderer::ApplyCameraPosition(Math::Vector2 p)
	{
		p += getInstance().m_Camera->getPosition();
		return p;
	}

	Math::Vector2 Renderer::ApplyCameraTransform(Math::Vector2 p)
	{
		return getInstance().m_Camera->getTransform() * p;
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