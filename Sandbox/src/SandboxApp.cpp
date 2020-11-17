#include <Hazel.h>

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
	//	HZ_TRACE("Updating Example Layer");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
		{
			HZ_TRACE("Tab key pressed (poll)!");
		}
	}

	void OnEvent(Hazel::Event& event) override
	{
	//	std::string& string = event.ToString();
	//	std::wstring widestr = std::wstring(string.begin(), string.ToString().end());

	//	const wchar_t* widecstr = widestr.c_str();

	//	OutputDebugString((event.ToString().c_str());

	// TODO Add a To wchar_t* fn to events to debug log them.

		/* TODO: RENABLE
	//	HZ_TRACE("Event: {0}", event);
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
			{
				HZ_TRACE("Tab key pressed (event)!");
			}
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
		*/
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}