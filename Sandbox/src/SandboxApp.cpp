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
		Hazel::Application& app = Hazel::Application::Get();
		Hazel::Window& window = app.GetWindow();
		Hazel::Graphics& graphics = window.GetGraphics();
		graphics.DrawTestTriangle();
	}

	void OnEvent(Hazel::Event& event) override
	{

	//	 TODO: RENABLE
		/*
		if (event.GetEventType() == Hazel::EventType::WindowResize)
		{		
			OutputDebugStringA(event.GetName());
		}
		if (event.GetEventType() == Hazel::EventType::MouseButtonPressed)
		{
			OutputDebugStringA(event.GetName());
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