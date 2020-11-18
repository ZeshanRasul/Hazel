#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"

namespace Hazel
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(unsigned char keycode)
	{
		return keystates[keycode];
	}

	WindowsInput::Win32KeyboardEvent WindowsInput::ReadKeyImpl()
	{
		if (keybuffer.size() > 0)
		{
			WindowsInput::Win32KeyboardEvent event = keybuffer.front();
			keybuffer.pop();
			return event;
		}
		else
		{
			// TODO return optional instead
			return WindowsInput::Win32KeyboardEvent();
		}
	}

	bool WindowsInput::IsKeyEmptyImpl()
	{
		return keybuffer.empty();
	}

	char WindowsInput::ReadCharImpl()
	{
		if (charbuffer.size() > 0)
		{
			unsigned char charcode = charbuffer.front();
			charbuffer.pop();
			return charcode;
		} 
		else
		{
			return 0;
		}

	}

	bool WindowsInput::IsCharEmptyImpl()
	{
		return charbuffer.empty();
	}

	void WindowsInput::FlushKeyImpl()
	{
		keybuffer = std::queue<Win32KeyboardEvent>();
	}

	void WindowsInput::FlushCharImpl()
	{
		charbuffer = std::queue<char>();
	}

	void WindowsInput::FlushImpl()
	{
		FlushKeyImpl();
		FlushCharImpl();
	}

	void WindowsInput::EnableAutorepeatImpl()
	{
		autorepeatEnabled = true;
	}

	void WindowsInput::DisableAutorepeatImpl()
	{
		autorepeatEnabled = false;
	}

	bool WindowsInput::AutorepeatIsEnabledImpl()
	{
		return autorepeatEnabled;
	}

	void WindowsInput::OnKeyPressedImpl(unsigned char keycode) noexcept
	{
		keystates[keycode] = true;
		keybuffer.push(Win32KeyboardEvent(Win32KeyboardEvent::Type::Press, keycode));
		TrimBufferImpl(keybuffer);
	}

	void WindowsInput::OnKeyReleasedImpl(unsigned char keycode) noexcept
	{
		keystates[keycode] = false;
		keybuffer.push(Win32KeyboardEvent(Win32KeyboardEvent::Type::Release, keycode));
		TrimBufferImpl(keybuffer);
	}

	void WindowsInput::OnCharImpl(char character) noexcept
	{
		charbuffer.push(character);
		TrimBufferImpl(charbuffer);
	}

	void WindowsInput::ClearStateImpl() noexcept
	{
		keystates.reset();
	}

	template<typename T>
	void WindowsInput::TrimBufferImpl(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > bufferSize)
		{ 
			buffer.pop();
		}
	}

	// TODO: Remove blanket return false from each function

	/*
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
		
		return false;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
		
		return false;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
		
		return { 0.0f, 0.0f};
	}

	float WindowsInput::GetMouseXImpl()
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto [xPos, yPos] = GetMousePositionImpl();
		return xPos;
		
		return false;
	}

	float WindowsInput::GetMouseYImpl()
	{
		
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto [xPos, yPos] = GetMousePositionImpl();
		return yPos;
		
		
		return false;
	}
	*/


}