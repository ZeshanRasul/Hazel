#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnAttach() override
	{
		Hazel::Application& app = Hazel::Application::Get();
		Hazel::Window& window = app.GetWindow();
		Hazel::DirectXGraphics& graphics = window.GetGraphics();
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
		for (auto i = 0; i < 80; i++)
		{
			boxes.push_back(std::make_unique<Hazel::Box>(
				graphics, rng, adist,
				ddist, odist, rdist
				));
		}

		graphics.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));


	}

	void OnUpdate() override
	{
		Hazel::Application& app = Hazel::Application::Get();
		Hazel::Window& window = app.GetWindow();
		Hazel::DirectXGraphics& graphics = window.GetGraphics();

		Hazel::HazelTimer& timer = app.GetTimer();

		auto dt = timer.Mark();
		
		graphics.ClearBuffer(0.6f, 0.4f, 0.6f);
	
		for (auto& b : boxes)
		{
			b->Update(dt);
			b->Draw(graphics);
		}
		
		graphics.EndFrame();

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

private:
	std::vector<std::unique_ptr<class Hazel::Box>> boxes;
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