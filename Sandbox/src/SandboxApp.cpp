#include <Hazel.h>

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
		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		{
			HZ_TRACE("S key pressed (poll)!");
		}
	}

	void OnEvent(Hazel::Event& event) override
	{

	// TODO Add a To wchar_t* fn to events to debug log them.

	//	 TODO: RENABLE
		if (event.GetEventType() == Hazel::EventType::WindowResize)
		{		
			event.GetName();
		}
		if (event.GetEventType() == Hazel::EventType::MouseButtonPressed)
		{
			event.GetName();
		}
		
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