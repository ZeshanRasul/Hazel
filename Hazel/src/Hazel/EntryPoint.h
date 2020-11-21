#pragma once

#include <Windows.h>

#include "HazelException.h"

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
		
		catch (const Hazel::HazelException& e)
		{
			MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		}

		catch (const std::exception& e)
		{
			char* exceptionTitle = "Standard Exception";
			MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}

		catch (...)
		{
			std::wstring exceptionTitle = L"Standard Exception";

			MessageBoxA(nullptr, "No details available", "Hello", MB_OK | MB_ICONEXCLAMATION);
		}

		return -1;

	}

#endif
