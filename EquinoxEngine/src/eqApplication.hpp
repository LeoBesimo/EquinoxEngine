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

		friend class thread;

	private:
		HINSTANCE m_hInstance;
		HWND m_WindowHandle;
		bool m_Running = false;

		std::wstring m_WindowTitle;
		int m_WindowWidth;
		int m_WindowHeight;

		std::function<void(float delta)> m_Update;
		float m_SecondsPerFrame = 1.f / 120.f;

		float m_FrameTime;

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

		inline static void Start()
		{
			getInstance().startWindow();
		}

		inline static void SetWindowProperties(const std::wstring& title, const int& width, const int& height)
		{
			getInstance().m_WindowTitle = title;
			getInstance().m_WindowWidth = width + 1;
			getInstance().m_WindowHeight = height + 1;
		}

		inline static void SetApplicationUpdate(const std::function<void(float delta)>& m_Update) { getInstance().m_Update = m_Update; }

		inline static std::wstring getWindowTitle() { return getInstance().m_WindowTitle; }
		inline static int getWindowWidth() { return getInstance().m_WindowWidth; }
		inline static int getWindowHeight() { return getInstance().m_WindowHeight; }
		inline static HWND getWindowHandle() { return getInstance().m_WindowHandle; }
		inline static bool IsRunning() { return getInstance().m_Running; }

		inline static void setFrameRate(float frameRate) { getInstance().m_SecondsPerFrame = 1 / frameRate; }

		inline static void SetFrameTime(float frameTime) { getInstance().m_FrameTime = frameTime; }
		inline static float GetFrameTime() { return getInstance().m_FrameTime; }

	private:
		void startWindow();
	};
}