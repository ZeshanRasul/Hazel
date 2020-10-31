#pragma once

#ifdef HZ_PLATFORM_WINDOWS

	extern Hazel::Application* Hazel::CreateApplication();

	int main(int argc, char* argv)
	{
		Hazel::Log::Init();
		HZ_CORE_WARN("Initialized Core Logger");
		HZ_INFO("Initialized Client Logger");
		int a = 6;
		HZ_INFO("Var = {}", a);

		auto app = Hazel::CreateApplication();
		app->Run();
		delete app;
	}

#endif