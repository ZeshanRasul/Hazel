#pragma once
#include "hzpch.h"

#include "Hazel/Renderer/GraphicsContext.h"

#include <wrl.h>
#include <d3d11.h>

namespace Hazel {

	class DirectXGraphicsContext : public GraphicsContext
	{
	public:
		DirectXGraphicsContext();
		DirectXGraphicsContext(const DirectXGraphicsContext&) = delete;
		DirectXGraphicsContext& operator=(const DirectXGraphicsContext&) = delete;
		~DirectXGraphicsContext() = default;

		virtual void Init(HWND hWnd) override;
		virtual void EndFrame() override;
		
		virtual void ClearBuffer(float red, float green, float blue) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView = nullptr;
	};

}