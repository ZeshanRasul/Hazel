#include "hzpch.h"
#include "Hazel/Window.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

#include "Hazel/KeyCodes.h"

#include "Hazel/Application.h"

#include "Platform/DirectX11/DirectXGraphicsContext.h"



namespace Hazel {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		switch (msg)
		{
			case WM_CLOSE:
			{

				WindowsWindow::WindowData& data = *(WindowsWindow::WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			
				WindowCloseEvent event;
				data.EventCallback(event);

				PostQuitMessage(0);
				break;
			}
			case WM_KEYDOWN:
			{
				if (wParam == HZ_KEY_S)
				{
					SetWindowText(hWnd, L"Hello World");
				}
				break;
			}
			case WM_KEYUP:
			{
				if (wParam == HZ_KEY_S)
				{
					SetWindowText(hWnd, L"Hazel Engine");
				}
				break;
			}

		} 



		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
		:m_hInstance(GetModuleHandle(nullptr))
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void CALLBACK WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		const wchar_t* pClassName = L"Hazel Window";

		// Register window class
		// First setup configuration structure
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof( wc );
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 4;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = pClassName;
		wc.hIconSm = nullptr;

		RegisterClassEx( &wc );

		const wchar_t* pWindowName = L"Hazel Engine";

		HWND m_Hwnd = CreateWindowEx(
			0, pClassName,
			pWindowName,
			WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
			200, 200, 640, 480,
			nullptr, nullptr, m_hInstance, nullptr
		);

		ShowWindow(m_Hwnd, SW_SHOW);

		SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, (LONG_PTR)&m_Data);
		
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
		
	}

	void WindowsWindow::OnUpdate()
	{
		MSG msg;
		BOOL result;

		while (result = GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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

}