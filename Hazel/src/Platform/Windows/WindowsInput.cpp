#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"

namespace Hazel
{
	Input* Input::s_Instance = new WindowsInput();

	///////////////////////
	///KEYBOARD FUNCTIONS//
	///////////////////////

	bool WindowsInput::IsKeyPressedImpl(unsigned char keycode)
	{
		return keystates[keycode];
	}

	std::optional<WindowsInput::InputKeyboardEvent> WindowsInput::ReadKeyImpl()
	{
		if (keybuffer.size() > 0)
		{
			WindowsInput::InputKeyboardEvent event = keybuffer.front();
			keybuffer.pop();
			return event;
		}
		return {};
	}

	bool WindowsInput::IsKeyEmptyImpl()
	{
		return keybuffer.empty();
	}

	std::optional<char> WindowsInput::ReadCharImpl()
	{
		if (charbuffer.size() > 0)
		{
			unsigned char charcode = charbuffer.front();
			charbuffer.pop();
			return charcode;
		} 
		return {};

	}

	bool WindowsInput::IsCharEmptyImpl()
	{
		return charbuffer.empty();
	}

	void WindowsInput::FlushKeyImpl()
	{
		keybuffer = std::queue<InputKeyboardEvent>();
	}

	void WindowsInput::FlushCharImpl()
	{
		charbuffer = std::queue<char>();
	}

	void WindowsInput::FlushKeyboardImpl()
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
		keybuffer.push(InputKeyboardEvent(InputKeyboardEvent::Type::Press, keycode));
		TrimKeyboardBufferImpl(keybuffer);
	}

	void WindowsInput::OnKeyReleasedImpl(unsigned char keycode) noexcept
	{
		keystates[keycode] = false;
		keybuffer.push(InputKeyboardEvent(InputKeyboardEvent::Type::Release, keycode));
		TrimKeyboardBufferImpl(keybuffer);
	}

	void WindowsInput::OnCharImpl(char character) noexcept
	{
		charbuffer.push(character);
		TrimKeyboardBufferImpl(charbuffer);
	}

	void WindowsInput::ClearStateImpl() noexcept
	{
		keystates.reset();
	}

	template<typename T>
	void WindowsInput::TrimKeyboardBufferImpl(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > bufferSize)
		{ 
			buffer.pop();
		}
	}

	///////////////////////
	///MOUSE FUNCTIONS/////
	///////////////////////

	std::pair<int, int> WindowsInput::GetMousePosImpl() 
	{
		return { x, y };
	}

	int WindowsInput::GetMousePosXImpl()
	{
		return x;
	}

	int WindowsInput::GetMousePosYImpl()
	{
		return y;
	}

	bool WindowsInput::IsInWindowImpl()
	{
		return isInWindow;
	}

	bool WindowsInput::IsLeftPressedImpl()
	{
		return isLeftPressed;
	}

	bool WindowsInput::IsRightPressedImpl()
	{
		return isRightPressed;
	}

	
	std::optional<WindowsInput::InputMouseEvent> WindowsInput::ReadMouseImpl()
	{
		if (mouseBuffer.size() > 0)
		{
			WindowsInput::InputMouseEvent event = mouseBuffer.front();
			mouseBuffer.pop();
			return event;
		}
		return {};
	}
	

	void WindowsInput::FlushMouseImpl()
	{
		mouseBuffer = std::queue<InputMouseEvent>();
	}

	void WindowsInput::OnMouseMoveImpl(int newX, int newY)
	{
		x = newX;
		y = newY;

		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::Move));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnMouseLeaveImpl()
	{
		isInWindow = false;
		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::Leave));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnMouseEnterImpl()
	{
		isInWindow = true;
		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::Enter));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnLeftPressedImpl(int x, int y)
	{
		isLeftPressed = true;

		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::LPress));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnLeftReleasedImpl(int x, int y)
	{
		isLeftPressed = false;

		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::LRelease));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnRightPressedImpl(int x, int y)
	{
		isRightPressed = true;

		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::RPress));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnRightReleasedImpl(int x, int y)
	{
		isRightPressed = false;

		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::RRelease));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnWheelUpImpl(int x, int y)
	{
		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::WheelUp));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnWheelDownImpl(int x, int y)
	{
		mouseBuffer.push(InputMouseEvent(InputMouseEvent::Type::WheelDown));
		TrimMouseBufferImpl();
	}

	void WindowsInput::OnWheelDeltaImpl(int x, int y, int delta)
	{
		wheelDeltaCarry += delta;

		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUpImpl(x, y);
		}

		while (wheelDeltaCarry <= WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelDownImpl(x, y);
		}
	}

	void WindowsInput::TrimMouseBufferImpl()
	{
		while (mouseBuffer.size() > bufferSize)
		{
			mouseBuffer.pop();
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