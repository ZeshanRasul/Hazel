#pragma once
#include "hzpch.h"

#include "Hazel/HazelException.h"
#include "DXGIInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

#include <wrl.h>
#include <d3d11.h>

namespace Hazel {

	class DirectXGraphics
	{
		friend class Bindable;
	public:
		class Exception : public HazelException
		{
			using HazelException::HazelException;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT hr;
			std::string info;
		};

		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;

		public: 
			const char* GetType() const noexcept override;
		private:
			std::string reason;
		};

	public:
		DirectXGraphics();
		DirectXGraphics(const DirectXGraphics&) = delete;
		DirectXGraphics& operator=(const DirectXGraphics&) = delete;
		~DirectXGraphics() = default;

		virtual void Init(HWND hWnd);
		virtual void EndFrame();
		
		virtual void ClearBuffer(float red, float green, float blue);
		void DrawIndexed(UINT count);
		void SetProjection(DirectX::FXMMATRIX proj) noexcept;
		DirectX::XMMATRIX GetProjection() const noexcept;

	private:
		DirectX::XMMATRIX m_Projection;
	#ifndef NDEBUG
		DxgiInfoManager infoManager;
	#endif

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView = nullptr;
	};

}