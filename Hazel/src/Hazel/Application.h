#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public: 
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		inline static Application& Get() { return *s_Instance; };

		inline HWND& GetHWND() { return s_Hwnd; };


		inline void SetHWND(HWND HWNDInput) { s_Hwnd = HWNDInput; };

		inline Window& GetWindow() { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		HWND s_Hwnd;

	private:
		static Application* s_Instance;


		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in client, we just need the declaration, linker will find the definition later
	Application* CreateApplication();
}


