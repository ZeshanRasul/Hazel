#include <Hazel.h>
#include <algorithm>
#include <memory>

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

		class Factory
		{
		public: 
			Factory(Hazel::DirectXGraphics& graphics)
				:
				graphics(graphics)
			{}

			std::unique_ptr<Hazel::Drawable> operator()()
			{
				switch (typedist(rng))
				{
				case 0:
				//	return std::make_unique<Hazel::Pyramid>(graphics, rng, adist, ddist, odist, rdist);
					return {};

				case 1:
					return std::make_unique<Hazel::Melon>(graphics, rng, adist, ddist, odist, rdist, longdist, latdist);
					return {};
				case 2:
//					return std::make_unique<Hazel::Box>(graphics, rng, adist, ddist, odist, rdist, bdist);
				default:
					assert(false && "Bad drawable type in factory");
					return {};
				}
			}

		private:
			Hazel::DirectXGraphics& graphics;
			std::mt19937 rng{ std::random_device{}() };
			std::uniform_real_distribution<float> adist{0.0f, PI * 2.0f};
			std::uniform_real_distribution<float> ddist{0.0f, PI * 0.5f};
			std::uniform_real_distribution<float> odist{0.0f, PI * 0.08f};
			std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
			std::uniform_real_distribution<float> bdist{0.4f, 3.0f};
			std::uniform_int_distribution<int> latdist{5, 20};
			std::uniform_int_distribution<int> longdist{10, 40};
			std::uniform_int_distribution<int> typedist{0, 2};
		};

		Factory f(graphics);
		drawables.reserve(nDrawables);
		std::generate_n(std::back_inserter(drawables), nDrawables, f);

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
	/*
		for (auto& d : drawables)
		{
			d->Update(dt);
			d->Draw(graphics);
		}
		*/
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
	std::vector<std::unique_ptr<class Hazel::Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
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