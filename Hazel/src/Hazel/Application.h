#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "HazelTimer.h"

#include "Hazel/ImGui/ImGuiLayer.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")


#include "Window.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public: 
		Application();
		virtual ~Application();

		int Run();

		void OnEvent(Event& event);

		inline static Application& Get() { return *s_Instance; };

		inline Window& GetWindow() { return *m_Window; }

		inline HazelTimer& GetTimer() { return m_Timer; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		
	private:
		static Application* s_Instance;

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		std::unique_ptr<Window> m_Window;
	//	ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;

		HazelTimer m_Timer;

	};

	// To be defined in client, we just need the declaration, linker will find the definition later
	Application* CreateApplication();
}


