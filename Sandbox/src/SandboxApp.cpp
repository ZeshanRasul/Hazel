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
		HZ_INFO("Updating Example Layer");
	}

	void OnEvent(Hazel::Event& event) override
	{
		HZ_INFO("Event: {0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Hazel::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}