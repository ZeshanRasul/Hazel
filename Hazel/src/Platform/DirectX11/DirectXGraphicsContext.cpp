#include "hzpch.h"
#include "Platform/DirectX11/DirectXGraphicsContext.h"

namespace Hazel {

	DirectXGraphicsContext::DirectXGraphicsContext()
	{

	}

	DirectXGraphicsContext::~DirectXGraphicsContext()
	{
		if (m_RenderTargetView != nullptr)
		{
			m_RenderTargetView->Release();
		}
		// Release ptr resources
		if (m_DeviceContext != nullptr)
		{
			m_DeviceContext->Release();
		}

		if (m_SwapChain != nullptr)
		{
			m_SwapChain->Release();
		}

		if (m_Device != nullptr)
		{
			m_Device->Release();
		}
	}

	void DirectXGraphicsContext::Init(HWND hWnd)
	{
		// Create description structure

		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		// Create Device and Swap Chain

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext
		);

		// Gain access to texture subresource in swap chain (back buffer)

		ID3D11Resource* pBackBuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
		m_Device->CreateRenderTargetView(pBackBuffer, nullptr, &m_RenderTargetView);
		pBackBuffer->Release();
		

	}

	void DirectXGraphicsContext::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	}

	void DirectXGraphicsContext::EndFrame()
	{
		// Present
		m_SwapChain->Present(1, 0);
	}

}