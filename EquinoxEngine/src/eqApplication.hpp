#pragma once

#include <Windows.h>

#include <string>
#include <functional>
#include <thread>

#include "eqRenderer.hpp"
#include "eqInput.hpp"

namespace eq
{
	class Application
	{
		friend LRESULT CALLBACK WindowCallback(
			HWND m_WindowHandle,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

		friend class std::thread;

	private:
		HINSTANCE m_hInstance;
		HWND m_WindowHandle;
		bool m_Running = false;

		std::wstring m_WindowTitle;
		int m_WindowWidth;
		int m_WindowHeight;

		std::function<void(float delta)> m_Update;
		float m_SecondsPerFrame = 1.f / 60.f;


	public:
		Application();

		Application(const Application&) = delete;
		Application& operator= (const Application&) = delete;

		~Application();

		inline static Application& getInstance()
		{
			static Application app;
			return app;
		}

		inline static void start()
		{
			getInstance().startWindow();
		}

		inline static void setWindowProperties(const std::wstring& title, const int& width, const int& height)
		{
			getInstance().m_WindowTitle = title;
			getInstance().m_WindowWidth = width + 1;
			getInstance().m_WindowHeight = height + 1;
		}

		inline static void setApplicationUpdate(const std::function<void(float delta)>& m_Update) { getInstance().m_Update = m_Update; }

		inline static std::wstring getWindowTitle() { return getInstance().m_WindowTitle; }
		inline static int getWindowWidth() { return getInstance().m_WindowWidth; }
		inline static int getWindowHeight() { return getInstance().m_WindowHeight; }
		inline static HWND getWindowHandle() { return getInstance().m_WindowHandle; }
		inline static bool isRunning() { return getInstance().m_Running; }

	private:
		void startWindow();
	};
}