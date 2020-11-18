#pragma once

#include <Windows.h>

#ifdef HZ_PLATFORM_WINDOWS

	extern Hazel::Application* Hazel::CreateApplication();

	int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	{
		try {
			Hazel::Log::Init();
			HZ_CORE_WARN("Initialized Core Logger");
			HZ_INFO("Initialized Client Logger");
			int a = 6;
			HZ_INFO("Var = {}", a);

			auto app = Hazel::CreateApplication();
			app->Run();
			delete app;
		}
		
		catch (const std::exception& e)
		{
			MessageBox(nullptr, reinterpret_cast<LPCWSTR>(e.what()), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}

		catch (...)
		{
			MessageBox(nullptr, L"No details available", L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}

		return -1;

	}

#endif
