#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGlRenderer.h"
#include "Platform/DirectX11/imgui_impl_dx11.h"
#include "Platform/DirectX11/imgui_impl_win32.h"

#include "GLFW/glfw3.h"

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
		ImGui::CreateContext();
		ImGui::StyleColorsClassic();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	//s	ImGui_ImplOpenGL3_Init("#version 150");
		Application& app = Application::Get();
		ImGui_ImplDX11_Init(app.g_pd3dDevice, app.g_pd3dDeviceContext);
		//ImGui_ImplWin32_Init(app.GetHWND());

		
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
	//	io.DisplaySize = ImVec2(1800, 1200);

		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		float deltaTime = time > 0.0f ? time - m_Time : 1.0f / 60.0f;
		m_Time = time;

		ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

	//	ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		//ImGui::ShowDemoWindow(&show);

	//	ImGui::Begin("Framerate", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	//	ImGui::SetWindowSize(ImVec2(200, 30), true);
	//	ImGui::SetWindowPos(ImVec2(2, 2), true);
	//	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
		ImGui::ShowDemoWindow(&show);

		HZ_CORE_TRACE("Rendering IMGUI");

		ImGui::Render();
		app.g_pd3dDeviceContext->OMSetRenderTargets(1, &app.g_mainRenderTargetView, NULL);
		app.g_pd3dDeviceContext->ClearRenderTargetView(app.g_mainRenderTargetView, (float*)&clear_color);


		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData(), app.g_pd3dDevice, app.g_pd3dDeviceContext);
		app.g_pSwapChain->Present(1, 0);

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
	}

}