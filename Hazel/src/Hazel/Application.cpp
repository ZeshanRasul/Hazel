#include "hzpch.h"
#include "Application.h"

#include "Glad/glad.h"

#include "GLFW/glfw3.h"
#include "imgui.h"


#include "Platform/DirectX11/imgui_impl_dx11.h"

#include <d3d11.h>

#include "Platform/Windows/WindowsWindow.h"
#define DIRECTINPUT_VERSION 0x0800


// Data



namespace Hazel {

	// Data
//	static ID3D11Device* g_pd3dDevice = NULL;
	//static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	//static IDXGISwapChain* g_pSwapChain = NULL;
	//static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	// Forward declarations of helper functions
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();


	bool CreateDeviceD3D(HWND hWnd)
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;

		Application& app = Application::Get();

		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &(app.g_pSwapChain), &(app.g_pd3dDevice), &featureLevel, &app.g_pd3dDeviceContext) != S_OK)
			return false;

		CreateRenderTarget();
		return true;
	}



	void CleanupDeviceD3D()
	{
		Application& app = Application::Get();

		CleanupRenderTarget();
		if (app.g_pSwapChain) { app.g_pSwapChain->Release(); app.g_pSwapChain = NULL; }
		if (app.g_pd3dDeviceContext) { app.g_pd3dDeviceContext->Release(); app.g_pd3dDeviceContext = NULL; }
		if (app.g_pd3dDevice) { app.g_pd3dDevice->Release(); app.g_pd3dDevice = NULL; }
	}

	void CreateRenderTarget()
	{
		Application& app = Application::Get();

		ID3D11Texture2D* pBackBuffer;
		app.g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		app.g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &app.g_mainRenderTargetView);
		pBackBuffer->Release();
	}

	void CleanupRenderTarget()
	{
		Application& app = Application::Get();

		if (app.g_mainRenderTargetView) { app.g_mainRenderTargetView->Release(); app.g_mainRenderTargetView = NULL; }
	}


	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		
	//	GLFWwindow* ourGLFWWindow =  m_Window2.m_W;
	//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//	HWND hWnd = glfwGetWin32Window(ourGLFWWindow);
		bool CreatedD3DDevice = false;
		CreatedD3DDevice = CreateDeviceD3D(this->s_Hwnd);
		HZ_CORE_ASSERT(CreatedD3DDevice, "Error creating DirectX 11 Device");
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		Application& app = Application::Get();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
		//ImGui_ImplDX11_Init(app.g_pd3dDevice, app.g_pd3dDeviceContext);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		
		HZ_CORE_TRACE("Event: {0}", event);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
			{
				break;
			}
		}
	}
	
	void Application::Run()
	{
		Application& app = Application::Get();

		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
				//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData(), app.g_pd3dDevice, app.g_pd3dDeviceContext);

			}

	//		glClearColor(1, 0, 1, 1);
	//		glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}


	
	}
