#include "hzpch.h"
#include "Hazel/Window.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/Input.h"
#include "Hazel/KeyCodes.h"
#include "Hazel/Application.h"

#include "Platform/DirectX11/DirectXGraphics.h"
#include "Platform/Windows/WindowsThrowMacros.h"

#include "Hazel/resource.h"

namespace Hazel {

	// Window Class Code:
	// Setup static WindowClass
	WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

	WindowsWindow::WindowClass::WindowClass() noexcept
		: hInstance(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMessageSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetInstance();
		wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

		RegisterClassEx(&wc);
	}

	WindowsWindow::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClassName, GetInstance());
	}

	const wchar_t* WindowsWindow::WindowClass::GetName() noexcept
	{
		return wndClassName;
	}

	HINSTANCE WindowsWindow::WindowClass::GetInstance() noexcept
	{
		return wndClass.hInstance;
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// Calculate window size based on desired client region size

		RECT wr;
		wr.left = 100;
		wr.right = m_Data.Width + wr.left;
		wr.top = 100;
		wr.bottom = m_Data.Height + wr.top;
		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		{
			throw HZWND_LAST_EXCEPT();
		}

		// Possibly change this to a parameter passed in to window props.
		const wchar_t* pWindowName = L"Hazel Engine";

		HWND m_Hwnd = CreateWindowEx(
			0, WindowClass::GetName(),
			pWindowName,
			WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, WindowClass::GetInstance(), this
		);

		if (m_Hwnd == nullptr)
		{
			throw HZWND_LAST_EXCEPT();
		}

		ShowWindow(m_Hwnd, SW_SHOWDEFAULT);
			
		m_Graphics = new DirectXGraphics();
		m_Graphics->Init(m_Hwnd);
		 
		
		SetVSync(true);

	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(m_Hwnd);
	}

	void WindowsWindow::OnUpdate()
	{

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			
		}
		else
		{
			
		}

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	std::optional<int> WindowsWindow::ProcessMessages() noexcept
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return (int)msg.wParam;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		
			return {};
		}

		return {};
	}

	DirectXGraphics& WindowsWindow::GetGraphics() const
	{
		if (!m_Graphics)
		{
			throw HZWND_NOGFX_EXCEPT();
		}
		return *m_Graphics;
	}


	LRESULT CALLBACK WindowsWindow::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Use the create long paramater passed in from CreateWindow() to store our window class pointer on the WinAPI side
		if (msg == WM_NCCREATE)
		{
			// Extract pointer from creation data and cast to window pointer
			const CREATESTRUCT* const p_CreationData = reinterpret_cast<CREATESTRUCT*>(lParam);
			WindowsWindow* const p_Wnd = static_cast<WindowsWindow*>(p_CreationData->lpCreateParams);

			// Set long pointer to be the pointer to our window
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_Wnd));
			// Set our wnd proc to our normal non-setup window proc
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowsWindow::HandleMessagThunk));

			// Forward message to our member function message handler
			return p_Wnd->HandleMessage(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK WindowsWindow::HandleMessagThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Retrieve window ptr to class
		WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// Forward message to our member function message handler
		return p_Wnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT WindowsWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// TODO Redo event creation to match previous implementation

		switch (msg)
		{
			case WM_CLOSE:
			{
				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = (p_Wnd->m_Data);

				WindowCloseEvent event;
				data.EventCallback(event);

				PostQuitMessage(0);
				break;
			}
			case WM_KILLFOCUS:
			{
				Input::ClearState();
				break;
			}

			///////////////////////
			///KEYBOARD Messages///
			///////////////////////

			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				// TODO Decide if we want to keep autorepeat enabled or not
				if (!(lParam & 0x40000000) || Input::AutorepeatIsEnabled())
				{
					Input::OnKeyPressed(static_cast<unsigned char>(wParam));
				}
				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;
				if (wParam == HZ_KEY_S)
				{
					SetWindowText(hWnd, L"Hello World");
				}

				// TODO Condition based on whether it is a repeat count or not
				KeyPressedEvent event((int)wParam, 0);
				data.EventCallback(event);

				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Input::OnKeyReleased(static_cast<unsigned char>(wParam));
				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				if (wParam == HZ_KEY_S)
				{
					SetWindowText(hWnd, L"Hazel Engine");
				}

				KeyReleasedEvent event((int)wParam);
				data.EventCallback(event);

				break;
			}
			case WM_CHAR:
			{
				Input::OnChar(static_cast<unsigned char>(wParam));
				break;
			}

			///////////////////////
			///MOUSE MESSAGES//////
			///////////////////////

			case WM_MOUSEMOVE:
			{
				const POINTS pt = MAKEPOINTS(lParam);

				// Check if mouse is in client region or not

				if (pt.x >= 0 && pt.x <= (int)m_Data.Width && pt.y >= 0 && pt.y <= (int)m_Data.Height)
				{
					Input::OnMouseMove(pt.x, pt.y);
					
					if (!Input::IsInWindow())
					{
						SetCapture(hWnd);
						Input::OnMouseEnter();
					}
				}
				else // not in client region. Only log move is left or right mouse button is down
				{
					if (wParam & (MK_LBUTTON | MK_RBUTTON))
					{
						Input::OnMouseMove(pt.x, pt.y);
					}
					else
					{
						ReleaseCapture();
						Input::OnMouseLeave();
					}
				}


				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				MouseMovedEvent event((float)pt.x, (float)pt.y);
				data.EventCallback(event);

				break;
			}

			case WM_LBUTTONDOWN:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				Input::OnLeftPressed(pt.x, pt.y);

				SetForegroundWindow(m_Hwnd);

				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				MouseButtonPressedEvent event(0);
				data.EventCallback(event);

				break;
			}

			case WM_RBUTTONDOWN:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				Input::OnRightPressed(pt.x, pt.y);

				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				MouseButtonPressedEvent event(1);
				data.EventCallback(event);

				break;
			}

			case WM_LBUTTONUP:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				Input::OnLeftReleased(pt.x, pt.y);

				// Release mouse drag if mouse release is outside the window
				if (pt.x < 0 || pt.x > (int)m_Data.Width || pt.y < 0 || pt.y > (int)m_Data.Height)
				{
					ReleaseCapture();
					Input::OnMouseLeave();
				}
			
				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				MouseButtonReleasedEvent event(0);
				data.EventCallback(event);

				break;
			}

			case WM_RBUTTONUP:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				Input::OnRightReleased(pt.x, pt.y);

				if (pt.x < 0 || pt.x > (int)m_Data.Width || pt.y < 0 || pt.y > (int)m_Data.Height)
				{
					ReleaseCapture();
					Input::OnMouseLeave();
				}

				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				MouseButtonReleasedEvent event(1);
				data.EventCallback(event);

				break;
			}

			case WM_MOUSEWHEEL:
			{
				const POINTS pt = MAKEPOINTS(lParam);

				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				// TODO create offset variable
				MouseScrolledEvent event(pt.x, pt.y);
				data.EventCallback(event);


				const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
				Input::OnWheelDelta(pt.x, pt.y, delta);
				break;
			}
			
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);

				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				if (data.EventCallback != nullptr)
				{
					data.EventCallback(event);
				}
				break;
			}

		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	WindowsWindow::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
		:
		Exception(line, file),
		hr(hr)
	{}

	const char* WindowsWindow::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() 
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Description]" << GetErrorDescription() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* WindowsWindow::HrException::GetType() const noexcept
	{
		return "Hazel Window Exception";
	}
	
	std::string WindowsWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* p_MsgBuffer = nullptr;
		const DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPWSTR>(&p_MsgBuffer), 0, nullptr
		);

		if (nMsgLen == 0)
		{
			return "Unidentified Error Code";
		}

		std::string errorString = p_MsgBuffer;
		LocalFree(p_MsgBuffer);
		return errorString;
	}

	HRESULT WindowsWindow::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string WindowsWindow::HrException::GetErrorDescription() const noexcept
	{
		return Exception::TranslateErrorCode(hr);
	}

	const char* WindowsWindow::NoGraphicsException::GetType() const noexcept
	{
		return "Hazel Window Exception [No Graphics]";
	}
}

