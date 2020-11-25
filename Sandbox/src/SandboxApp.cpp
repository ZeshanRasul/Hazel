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
		Hazel::DirectXGraphics& graphics = window.GetGraphics();
		Hazel::HazelTimer& timer = app.GetTimer();

		const float c = sin(timer.Peek()) / 2.0f + 0.5f;
		graphics.ClearBuffer(c, c, 0.9f);
		graphics.DrawTestTriangle(timer.Peek(), 0.0f, 0.0f);
		graphics.DrawTestTriangle(timer.Peek(), (float)Hazel::Input::GetMousePosX() / 640.0f - 1.0f, (float)-Hazel::Input::GetMousePosY() / 480.0f + 1.0f);
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