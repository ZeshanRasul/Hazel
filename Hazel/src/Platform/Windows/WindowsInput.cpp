#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"

namespace Hazel
{
	Input* Input::s_Instance = new WindowsInput();

	// TODO: Remove blanket return false from each function

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		/*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
		*/
		return false;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		/*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
		*/
		return false;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		/*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
		*/
		return { 0.0f, 0.0f};
	}

	float WindowsInput::GetMouseXImpl()
	{
		/*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto [xPos, yPos] = GetMousePositionImpl();
		return xPos;
		*/
		return false;
	}

	float WindowsInput::GetMouseYImpl()
	{
		/*
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto [xPos, yPos] = GetMousePositionImpl();
		return yPos;
		*/
		return false;
	}


}