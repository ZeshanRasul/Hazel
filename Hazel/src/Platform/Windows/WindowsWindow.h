#pragma once

#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"
#include "Hazel/HazelException.h"

namespace Hazel {

	class WindowsWindow : public Window
	{
	public: 
		class Exception : public HazelException
		{
		public:
			Exception(int line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept;
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;

		private:
			HRESULT hr;
		};

	private:
		class WindowClass
		{
		public:
			static const wchar_t* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;

		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			static constexpr const wchar_t* wndClassName = L"Hazel Engine Window";
			static WindowClass wndClass;
			HINSTANCE hInstance;
		};

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		WindowsWindow(const WindowsWindow&) = delete;
		Window& operator=(const Window&) = delete;

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }


	private:
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessagThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		
	private:
		void* m_Window;

		WindowData m_Data;

		GraphicsContext* m_GraphicsContext;

		HWND m_Hwnd;
	};

// Error exception helper macro
#define HZWND_EXCEPT(hr) WindowsWindow::Exception(__LINE__, __FILE__, hr)
#define HZWND_LAST_EXCEPT() WindowsWindow::Exception(__LINE__, __FILE__, GetLastError())
}