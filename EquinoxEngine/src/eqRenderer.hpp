#pragma once

#pragma once

#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <memory>
#include <wchar.h>

#include <eqMath.hpp>
#include "Shapes/eqShapes.hpp"
#include "eqRenderUtilities.hpp"
#include "eqCamera.hpp"
#include "Images/eqImages.hpp"
#include <eqPhysics.hpp>

//#include "Shapes/Drawable.h"

namespace eq
{
	class Renderer
	{
		friend LRESULT CALLBACK WindowCallback(
			HWND m_WindowHandle,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		friend class Application;

	private:
		static const int s_BytesPerPixel = 4;

		HWND m_WindowHandle = 0;
		BitmapBuffer m_BitmapBuffer[2];
		std::vector<Rectangle> m_RectangleBuffer[2];
		std::vector<Ellipse> m_EllipseBuffer[2];
		std::vector<Line> m_LineBuffer[2];
		std::vector<Text> m_TextBuffer[2];
		std::vector<Sprite> m_SpriteBuffer[2];
		Color m_ClearColor;
		bool m_Buffer2 = false;
		bool m_SwappedBuffers = false;

		std::shared_ptr<Camera> m_Camera;

		static float alphaScaler;

	public:
		static void SetClearColor(const Color& color) { getInstance().m_ClearColor = color; }
		static void SetPixel(int x, int y, const Color& color);
		static void SetPixel(int x, int y, const uint32_t color);
		static void FillRectangle(const Rect& rect, const Color& color);
		static void DrawRectangle(const Rect& rect, const Color& color);
		static void DrawLine(int x0, int y0, int x1, int y1, const Color& color);
		static void DrawLine(Math::Vector2 a, Math::Vector2 b, const Color& color);
		static void FillCircle(int originX, int originY, int radius, const Color& color);
		static void FillEllipse(int originX, int originY, int radiusX, int radiusY, const Color& color);
		static void DrawCircle(int originX, int originY, int radius, const Color& color);
		static void DrawCircle(Math::Vector2 position, int radius, const Color& color);
		static void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color& color);
		static void DrawSprite(Sprite& sprite);

		static void draw(Ellipse ellipse);
		static void draw(Line line);
		static void draw(Rectangle rectangle);
		static void draw(Text text);
		static void draw(Sprite sprite);
		static void draw(Physics::PhysicsWorld& world);

		static void swapBuffers()
		{
			getInstance().m_Buffer2 = !getInstance().m_Buffer2;
			getInstance().m_SwappedBuffers = true;
		}

		static bool BuffersSwapped() { return getInstance().m_SwappedBuffers; }

		static void SetCamera(std::shared_ptr<Camera> camera) { getInstance().m_Camera = camera; }

		static bool WaitForSwap() { return !getInstance().m_SwappedBuffers; }

	private:
		Renderer() { m_BitmapBuffer[0] = {}; m_BitmapBuffer[1] = {}; m_ClearColor = Color(255, 255, 255, 255); alphaScaler = 1 / 255; }

		Renderer(const Renderer&) = delete;
		Renderer& operator= (const Renderer&) = delete;

		~Renderer() {}

		inline static Renderer& getInstance()
		{
			static Renderer renderer;
			return renderer;
		}

	private:
		static BitmapBuffer& getActiveBuffer() { return getInstance().m_BitmapBuffer[getInstance().m_Buffer2]; }
		static BitmapBuffer& getInactiveBuffer() { return getInstance().m_BitmapBuffer[!getInstance().m_Buffer2]; }

		static std::vector<Rectangle>& getActiveRectangles() { return getInstance().m_RectangleBuffer[getInstance().m_Buffer2]; }
		static std::vector<Rectangle>& getInactiveRectangles() { return getInstance().m_RectangleBuffer[!getInstance().m_Buffer2]; }

		static std::vector<Ellipse>& getActiveEllipses() { return getInstance().m_EllipseBuffer[getInstance().m_Buffer2]; }
		static std::vector<Ellipse>& getInactiveEllipses() { return getInstance().m_EllipseBuffer[!getInstance().m_Buffer2]; }

		static std::vector<Line>& getActiveLines() { return getInstance().m_LineBuffer[getInstance().m_Buffer2]; }
		static std::vector<Line>& getInactiveLines() { return getInstance().m_LineBuffer[!getInstance().m_Buffer2]; }

		static std::vector<Text>& getActiveText() { return getInstance().m_TextBuffer[getInstance().m_Buffer2]; }
		static std::vector<Text>& getInactiveText() { return getInstance().m_TextBuffer[!getInstance().m_Buffer2]; }

		static std::vector<Sprite>& getActiveSprites() { return getInstance().m_SpriteBuffer[getInstance().m_Buffer2]; }
		static std::vector<Sprite>& getInactiveSprites() { return getInstance().m_SpriteBuffer[!getInstance().m_Buffer2]; }

		inline static void setWindowHandle(HWND m_WindowHandle) { getInstance().m_WindowHandle = m_WindowHandle; }
		static void getWindowDimenstions(int* outWidth, int* outHeight);
		static void resizeFrameBuffer(int width, int height);
		static void copyBufferToWindow(HDC deviceContext, int m_WindowWidth, int m_WindowHeight);

		static void clear();
		static void clearBuffers();

		static void RenderObjects();
		static void RenderText(HDC devicContext);
		static void RenderRectangles();
		static void RenderEllipses();
		static void RenderLines();
		static void RenderSprites();

	public:
		static Math::Vector2 WorldToScreenspace(Math::Vector2 p);
		static Math::Vector2 ScreenToWorldspace(Math::Vector2 p);
		static Math::Vector2 ApplyCameraPosition(Math::Vector2 p);
		static Math::Vector2 ApplyCameraTransform(Math::Vector2 p);

	private:
		static void plotLineLow(int x0, int y0, int x1, int y1, const Color& color);
		static void plotLineHigh(int x0, int y0, int x1, int y1, const Color& color);

		static Color ColorFromUInt(uint32_t color);
		static uint32_t UIntFromColor(Color color);
		static uint32_t BlendColor(uint32_t colorA, uint32_t colorB);
	};
}