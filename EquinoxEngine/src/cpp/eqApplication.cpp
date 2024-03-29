#include "../Equinox/Application/eqApplication.hpp"

namespace eq
{
	Application::Application()
	{
		m_WindowTitle = L"Equinox Application";
		m_WindowWidth = 1921;
		m_WindowHeight = 1081;
		m_ResolutionX = 1921;
		m_ResolutionY = 1081;
	}

	Application::~Application()
	{}

	LRESULT CALLBACK WindowCallback(
		HWND m_WindowHandle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	)
	{
		LRESULT result = 0;

		switch (message)
		{
		case WM_CLOSE:
		{
			Application::getInstance().m_Running = false;
			OutputDebugString(L"Window close\n");
		}break;

		case WM_DESTROY:
		{
			Application::getInstance().m_Running = false;
			OutputDebugString(L"window destroy\n");
		}break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32_t VKCode = wParam;
			bool wasDown = (lParam & (1 << 30)) != 0;
			bool isDown = (lParam & (1 << 31)) == 0;

			Input::processKeyboardInput(VKCode, wasDown, isDown);

		}break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		{
			Input::processMouseInput(wParam, lParam);
		}break;

		case WM_MOUSEMOVE:
		{
			Input::updateMousePosition(lParam);
		}break;

		case WM_ERASEBKGND:
			return (LRESULT)1;

		case WM_PAINT:
		{
			OutputDebugString(L"window paint\n");

			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(m_WindowHandle, &paint);

			int width, height;
			Renderer::getWindowDimensions(&width, &height);

			Renderer::copyBufferToWindow(deviceContext, width, height);

			EndPaint(m_WindowHandle, &paint);
		}break;

		default:
			result = DefWindowProc(m_WindowHandle, message, wParam, lParam);
		}

		return result;
	}

	void Application::SetFullscreen()
	{
		if (!getInstance().m_WindowHandle) return;

		getInstance().m_Resizing = true;

		while (!Renderer::FinishedFrame()) {}

		int newWidth = GetSystemMetrics(SM_CXSCREEN);
		int newHeight = GetSystemMetrics(SM_CYSCREEN);

		getInstance().m_WindowWidth = newWidth;
		getInstance().m_WindowHeight = newHeight;

		getInstance().m_ResolutionX = newWidth;
		getInstance().m_ResolutionY = newHeight;

		Renderer::resizeFrameBuffer(newWidth, newHeight);
		SetWindowLong(getInstance().m_WindowHandle, GWL_STYLE, 0);
		SetWindowPos(getInstance().m_WindowHandle, HWND_TOP, 0, 0, newWidth, newHeight, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW | SWP_FRAMECHANGED);

		getInstance().m_Resizing = false;

		return;
	}

	void Application::SetWindowSize(unsigned int newWidth, unsigned int newHeight)
	{
		if (!getInstance().m_WindowHandle) return;

		getInstance().m_Resizing = true;

		while (!Renderer::FinishedFrame()) {}

		Renderer::resizeFrameBuffer(newWidth, newHeight);
		SetWindowLong(getInstance().m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		LONG style = GetWindowLong(getInstance().m_WindowHandle, GWL_STYLE);
		style &= ~WS_THICKFRAME;
		style &= ~WS_MAXIMIZEBOX;
		SetWindowLong(getInstance().m_WindowHandle, GWL_STYLE, style);

		int x = (GetSystemMetrics(SM_CXSCREEN) - newWidth) / 2;
		int y = (GetSystemMetrics(SM_CYSCREEN) - newHeight) / 2;

		getInstance().m_WindowWidth = newWidth;
		getInstance().m_WindowHeight = newHeight;

		getInstance().m_ResolutionX = newWidth;
		getInstance().m_ResolutionY = newHeight;

		SetWindowPos(getInstance().m_WindowHandle, HWND_TOP, x, y, newWidth, newHeight, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW);
		getInstance().m_Resizing = false;
		return;
	}

	void Application::SetResolution(unsigned int width, unsigned int height)
	{
		if (!getInstance().m_WindowHandle) return;

		getInstance().m_ResolutionX = width;
		getInstance().m_ResolutionY = height;

		getInstance().m_Resizing = true;
		while (!Renderer::FinishedFrame()) {}
		Renderer::resizeFrameBuffer(width, height);
		getInstance().m_Resizing = false;
	}

	void Application::startWindow()
	{
		const wchar_t* className = L"Equinox Window";

		WNDCLASS windowClass = {};

		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowCallback;
		windowClass.hInstance = m_hInstance;
		windowClass.lpszClassName = className;

		if (!RegisterClass(&windowClass))
		{
			OutputDebugString(L"Failed to Register Window Class\n");
		}

		DWORD windowStyle = CS_HREDRAW | CS_VREDRAW;//WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;

		RECT windowRect = {};
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = m_WindowWidth;
		windowRect.bottom = m_WindowHeight;

		AdjustWindowRect(&windowRect, windowStyle, false);

		m_WindowHandle = CreateWindowEx(
			0,
			className,
			m_WindowTitle.c_str(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			0,
			0,
			m_hInstance,
			0
		);

		if (m_WindowHandle)
		{
			OutputDebugString(L"Application Init\n");
			m_Running = true;

			LONG style = GetWindowLong(m_WindowHandle, GWL_STYLE);
			style &= ~WS_THICKFRAME;
			style &= ~WS_MAXIMIZEBOX;
			SetWindowLong(m_WindowHandle, GWL_STYLE, style);
			SetWindowPos(m_WindowHandle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

			Renderer::setWindowHandle(m_WindowHandle);

			int bufferWidth = 0;
			int bufferHeight = 0;
			Renderer::getWindowDimensions(&bufferWidth, &bufferHeight);
			Renderer::resizeFrameBuffer(bufferWidth, bufferHeight);

			LARGE_INTEGER cpuFrequency;
			QueryPerformanceFrequency(&cpuFrequency);

			LARGE_INTEGER lastCounter;
			QueryPerformanceCounter(&lastCounter);

			std::thread renderThread([]() {

				LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);

			LARGE_INTEGER last;
			QueryPerformanceCounter(&last);

			LARGE_INTEGER current;

			getInstance().m_Init();


			while (Application::IsRunning())
			{
				HWND m_WindowHandle = Application::GetWindowHandle();

#ifdef NDEBUG
				while (m_WindowHandle != GetForegroundWindow())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
#endif // !debug

				while (Renderer::WaitForSwap() && m_WindowHandle == GetForegroundWindow() || getInstance().IsResizing()) {}
				Renderer::SetFrameFinished(false);

				HDC deviceContext = GetDC(m_WindowHandle);
				RECT rc;
				HDC hdcMem;
				HBITMAP hbmMem, hbmOld;

				if (Renderer::BuffersSwapped() == true) {
					//Renderer::getInstance().m_SwappedBuffers = false;
					//Renderer::clearBuffers();
				}

				int width, height;
				Renderer::getWindowDimensions(&width, &height);

				//HBRUSH hbrBkGnd;
				//HFONT hfntOld;

				GetClientRect(m_WindowHandle, &rc);
				hdcMem = CreateCompatibleDC(deviceContext);
				hbmMem = CreateCompatibleBitmap(deviceContext, rc.right - rc.left, rc.bottom - rc.top);
				hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

				//hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
				//FillRect(hdcMem, &rc, hbrBkGnd);
				//DeleteObject(hbrBkGnd);

				QueryPerformanceCounter(&current);

				//int64_t elapsed = current.QuadPart - last.QuadPart;

				//SetFrameTime((float)elapsed / (float)freq.QuadPart);
				last = current;

				Renderer::clear();
				Renderer::RenderObjects();
				Renderer::copyBufferToWindow(hdcMem, width, height);
				Renderer::RenderText(hdcMem);

				BitBlt(deviceContext,
					rc.left, rc.top,
					rc.right - rc.left, rc.bottom - rc.top,
					hdcMem,
					0, 0,
					SRCCOPY);

				SelectObject(hdcMem, hbmOld);
				DeleteObject(hbmMem);
				DeleteDC(hdcMem);

				ValidateRect(m_WindowHandle, NULL);

				ReleaseDC(m_WindowHandle, deviceContext);

				QueryPerformanceCounter(&current);

				int64_t elapsed = current.QuadPart - last.QuadPart;
				SetFrameTime((float)elapsed / (float)freq.QuadPart);
				last = current;

				Renderer::SetFrameFinished(true);
			}

				});

			while (m_Running)
			{

				LARGE_INTEGER currentCounter;
				float delta = 0;
				QueryPerformanceCounter(&currentCounter);
				while (delta < m_SecondsPerFrame)
				{
					QueryPerformanceCounter(&currentCounter);

					int64_t counterElapsed = currentCounter.QuadPart - lastCounter.QuadPart;

					delta = (float)counterElapsed / (float)cpuFrequency.QuadPart;
				}
				lastCounter = currentCounter;
#ifdef NDEBUG
				while (m_WindowHandle != GetForegroundWindow())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
#endif
				MSG message;
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					if (message.message == WM_QUIT)

						m_Running = false;
					TranslateMessage(&message);
					DispatchMessage(&message);

				}

				getInstance().m_Update(delta);

				while (!Renderer::FinishedFrame()) {}
				Renderer::swapBuffers();
				Renderer::clearBuffers();
			}

			renderThread.join();
		}
		else
		{
			OutputDebugString(L"Failed to Create Window\n");
			return;
		}
	}
}