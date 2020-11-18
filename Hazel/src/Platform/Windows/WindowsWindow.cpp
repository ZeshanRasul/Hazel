#include "hzpch.h"
#include "Hazel/Window.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/Input.h"

#include "Hazel/KeyCodes.h"

#include "Hazel/Application.h"

#include "Platform/DirectX11/DirectXGraphicsContext.h"

#include "../../resource.h"



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
		wc.hIcon = static_cast<HICON>(LoadImage(
			GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 32, 32, 0));
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = GetName();
		wc.hIconSm = static_cast<HICON>(LoadImage(
			GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
			IMAGE_ICON, 16, 16, 0));

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
		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, FALSE);

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

	
		m_GraphicsContext = new DirectXGraphicsContext();
		m_GraphicsContext->Init();
		 
		
		SetVSync(true);

		// TODO REDO Event callbacks with Windows events
		/*
		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				// either use data directly as m_Data or use Win32SetWindowLongPtrA
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});


		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)(glfwGetWindowUserPointer(window));

				WindowCloseEvent event;
				data.EventCallback(event);
			}); DONE

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);

					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);

					break;
				}

				case GLFW_REPEAT:
				{
					// GLFW doesn't provide us with a repeat count so for now we will hardcode it as 1.
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(key);
				data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPosition, double yPosition)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPosition, (float)yPosition);
				data.EventCallback(event);
			});
			*/


	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(m_Hwnd);
	}

	void WindowsWindow::OnUpdate()
	{
		MSG msg;
		BOOL result;

		while (result = GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			/*
			if (Input::IsKeyPressed(VK_MENU))
			{
				MessageBox(nullptr, L"Something Happened!", L"Alt Key Was Pressed", MB_OK | MB_ICONEXCLAMATION);
			}
			*/
		}

		m_GraphicsContext->SwapBuffers();

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
		switch (msg)
		{
			case WM_CLOSE:
			{
				WindowsWindow* const p_Wnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
				WindowData& data = p_Wnd->m_Data;

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
				KeyPressedEvent event(wParam, 0);
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

				KeyReleasedEvent event(wParam);
				data.EventCallback(event);

				break;
			}
			case WM_CHAR:
			{
				Input::OnChar(static_cast<unsigned char>(wParam));
			}

		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	WindowsWindow::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
		:
		HazelException(line, file),
		hr(hr)
	{

	}

	const char* WindowsWindow::Exception::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code]" << GetErrorCode() << std::endl
			<< "[Description]" << GetErrorString() << std::endl
			<< GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* WindowsWindow::Exception::GetType() const noexcept
	{
		return "Hazel Window Exception";
	}
	
	std::string WindowsWindow::Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* p_MsgBuffer = nullptr;
		DWORD nMsgLen = FormatMessage(
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

	HRESULT WindowsWindow::Exception::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string WindowsWindow::Exception::GetErrorString() const noexcept
	{
		return TranslateErrorCode(hr);
	}
}

