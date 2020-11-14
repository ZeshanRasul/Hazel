#include "hzpch.h"
#include "ImGuiLayer.h"
#include "Hazel/Core.h"

#include "imgui.h"
#include "Platform/DirectX11/imgui_impl_dx11.h"
#include "examples/imgui_impl_glfw.h"

// Temporary includes
#include "GLFW/glfw3.h"
#include "d3d11.h"
#pragma comment(lib, "d3d11.lib")


#include "Hazel/Application.h"

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		Application& app = Application::Get();

		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForDirectX(window, true);
		ImGui_ImplDX11_Init(app.g_pd3dDevice, app.g_pd3dDeviceContext);
	
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImVec4 clear_color = ImVec4(0.5f, 0.49f, 1.00f, 0.5f);

		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());


		ImGui::Render();
		app.g_pd3dDeviceContext->OMSetRenderTargets(1, &app.g_mainRenderTargetView, NULL);
		app.g_pd3dDeviceContext->ClearRenderTargetView(app.g_mainRenderTargetView, (float*)&clear_color);


		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData(), app.g_pd3dDevice, app.g_pd3dDeviceContext);

		/*
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		*/
		
		app.g_pSwapChain->Present(1, 0);
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		//ImGui::ShowDemoWindow(&show);
/*
		ImGui::Begin("Framerate", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize(ImVec2(200, 30), true);
		ImGui::SetWindowPos(ImVec2(2, 2), true);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		*/
		ImGui::ShowDemoWindow(&show);

	}

}