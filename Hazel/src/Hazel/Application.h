#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"


#include "Window.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public: 
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

	private:
		bool OnWindowClose(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client, we just need the declaration, linker will find the definition later
	Application* CreateApplication();
}


