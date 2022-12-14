#pragma once

#pragma once

#include <Windows.h>
#include <stdint.h>
#include <assert.h>
#include <vector>
#include <memory>

#include <eqMath.hpp>
#include "Shapes/eqShapes.hpp"
#include "eqRenderUtilities.hpp"

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
		std::vector<std::unique_ptr<Drawable>> m_ShapeBuffer[2];
		std::vector<std::unique_ptr<Drawable>> m_TextBuffer[2];
		Color m_ClearColor;
		std::vector<Text> text;
		bool m_Buffer2 = false;
		bool m_SwappedBuffers = false;

		static float alphaScaler;

	public:
		static void setClearColor(const Color& color) { getInstance().m_ClearColor = color; }
		static void SetPixel(int x, int y, const Color& color);
		static void FillRectangle(const Rect& rect, const Color& color);
		static void DrawRectangle(const Rect& rect, const Color& color);
		static void DrawLine(int x0, int y0, int x1, int y1, const Color& color);
		static void DrawLine(Math::Vector2 a, Math::Vector2 b, const Color& color);
		static void FillCircle(int originX, int originY, int radius, const Color& color);
		static void FillEllipse(int originX, int originY, int radiusX, int radiusY, const Color& color);
		static void DrawCircle(int originX, int originY, int radius, const Color& color);
		static void DrawCircle(Math::Vector2 position, int radius, const Color& color);
		static void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, const Color& color);

		static void WriteText(const wchar_t* text, int x, int y, const Color& color);

		static void draw(Drawable& drawable);

		static void swapBuffers()
		{
			getInstance().m_Buffer2 = !getInstance().m_Buffer2;
			getInstance().m_SwappedBuffers = true;
			getActiveShapes().clear();
			getActiveText().clear();

		}

		static bool buffersSwapped() { return getInstance().m_SwappedBuffers; }

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

		static std::vector<std::unique_ptr<Drawable>>& getActiveShapes() { return getInstance().m_ShapeBuffer[getInstance().m_Buffer2]; }
		static std::vector<std::unique_ptr<Drawable>>& getInactiveShapes() { return getInstance().m_ShapeBuffer[!getInstance().m_Buffer2]; }

		static std::vector<std::unique_ptr<Drawable>>& getActiveText() { return getInstance().m_TextBuffer[getInstance().m_Buffer2]; }
		static std::vector<std::unique_ptr<Drawable>>& getInactiveText() { return getInstance().m_TextBuffer[!getInstance().m_Buffer2]; }

		inline static void setWindowHandle(HWND m_WindowHandle) { getInstance().m_WindowHandle = m_WindowHandle; }
		static void getWindowDimenstions(int* outWidth, int* outHeight);
		static void resizeFrameBuffer(int width, int height);
		static void copyBufferToWindow(HDC deviceContext, int m_WindowWidth, int m_WindowHeight);
		static void clear();

		static void plotLineLow(int x0, int y0, int x1, int y1, const Color& color);
		static void plotLineHigh(int x0, int y0, int x1, int y1, const Color& color);

		static Color ColorFromUInt(uint32_t color);
		static uint32_t UIntFromColor(Color color);
		static uint32_t BlendColor(Color original, Color Background);
		static uint32_t BlendColor(Color original, uint32_t background);
		static uint32_t BlendColor(uint32_t colorA, uint32_t colorB);
	};
}