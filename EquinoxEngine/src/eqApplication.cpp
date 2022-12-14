#include "eqApplication.hpp"

namespace eq
{
	Application::Application()
	{
		m_WindowTitle = L"Equinox Application";
		m_WindowWidth = 1921;
		m_WindowHeight = 1081;
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

		case WM_PAINT:
		{
			OutputDebugString(L"window paint\n");

			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(m_WindowHandle, &paint);

			int width, height;
			Renderer::getWindowDimenstions(&width, &height);

			Renderer::copyBufferToWindow(deviceContext, width, height);

			EndPaint(m_WindowHandle, &paint);
		}break;

		default:
			result = DefWindowProc(m_WindowHandle, message, wParam, lParam);
		}

		return result;
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

		DWORD windowStyle = WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;

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

			Renderer::setWindowHandle(m_WindowHandle);

			int bufferWidth = 0;
			int bufferHeight = 0;
			Renderer::getWindowDimenstions(&bufferWidth, &bufferHeight);
			Renderer::resizeFrameBuffer(bufferWidth, bufferHeight);

			LARGE_INTEGER cpuFrequency;
			QueryPerformanceFrequency(&cpuFrequency);

			LARGE_INTEGER lastCounter;
			QueryPerformanceCounter(&lastCounter);

			std::thread renderThread([]() {

				while (Application::isRunning())
				{
					HWND m_WindowHandle = Application::getWindowHandle();

					HDC deviceContext = GetDC(m_WindowHandle);

					int width, height;
					Renderer::getWindowDimenstions(&width, &height);

					Renderer::copyBufferToWindow(deviceContext, width, height);
					
					Renderer::RenderText(deviceContext);

					ValidateRect(m_WindowHandle, NULL);

					ReleaseDC(m_WindowHandle, deviceContext);
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

				MSG message;
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					if (message.message == WM_QUIT)

						m_Running = false;
					TranslateMessage(&message);
					DispatchMessage(&message);

				}

				Renderer::clear();

				getInstance().m_Update(delta);

				Renderer::swapBuffers();

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