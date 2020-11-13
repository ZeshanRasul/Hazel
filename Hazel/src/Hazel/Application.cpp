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

		Application& app = Application::Get();

		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;
		sd.BufferDesc.Width = app.GetWindow().GetWidth();
		sd.BufferDesc.Height = app.GetWindow().GetHeight();
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

		bool CreatedD3DDevice = false;
		CreatedD3DDevice = CreateDeviceD3D(this->s_Hwnd);
		HZ_CORE_ASSERT(CreatedD3DDevice, "Error creating DirectX 11 Device");

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	//	GLFWwindow* ourGLFWWindow =  m_Window2.m_W;
	//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//	HWND hWnd = glfwGetWin32Window(ourGLFWWindow);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		Application& app = Application::Get();
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		
	//	HZ_CORE_TRACE("Event: {0}", event);

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
	//	const float ClearColour[4] = { 0, 100, 249, 0 };
		
		while (m_Running)
		{
			//g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, ClearColour);
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();

	//		g_pSwapChain->Present(1, 0);

		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		HZ_CORE_TRACE(event);
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

		//Handle window resizing for DirectX
		if (g_pSwapChain)
		{
			g_pd3dDeviceContext->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			g_mainRenderTargetView->Release();

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = g_pSwapChain->ResizeBuffers(0, event.GetWidth(), event.GetHeight(), DXGI_FORMAT_UNKNOWN, 0);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			// Perform error handling here!

			hr = g_pd3dDevice->CreateRenderTargetView(pBuffer, NULL,
				&g_mainRenderTargetView);
			// Perform error handling here!
			pBuffer->Release();

			g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = event.GetWidth();
			vp.Height = event.GetHeight();
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			g_pd3dDeviceContext->RSSetViewports(1, &vp);
		}

		return false;

	}


	
	}
