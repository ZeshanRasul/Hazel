#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

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

		void Run();

		void OnEvent(Event& event);

		inline static Application& Get() { return *s_Instance; };

		inline HWND& GetHWND() { return s_Hwnd; };


		inline void SetHWND(HWND HWNDInput) { s_Hwnd = HWNDInput; };

		inline Window& GetWindow() { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		HWND s_Hwnd;


		ID3D11Device* g_pd3dDevice;
		ID3D11DeviceContext* g_pd3dDeviceContext;
		IDXGISwapChain* g_pSwapChain;
		ID3D11RenderTargetView* g_mainRenderTargetView;
		ID3D11Buffer* g_pVertexConstantBuffer;


	private:
		static Application* s_Instance;

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	// To be defined in client, we just need the declaration, linker will find the definition later
	Application* CreateApplication();
}


