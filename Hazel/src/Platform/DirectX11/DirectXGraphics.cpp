#include "hzpch.h"
#include "Platform/DirectX11/DirectXGraphics.h"
#include "GraphicsThrowMacros.h"

#include <DirectXMath.h>
#include "DXErr.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Hazel {

	DirectXGraphics::DirectXGraphics()
	{

	}

	void DirectXGraphics::Init(HWND hWnd)
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

		UINT swapChainCreateFlags = 0u;
#ifndef NDEBUG
		swapChainCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr;
		// Create Device and Swap Chain

		GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapChainCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_SwapChain,
			&m_Device,
			nullptr,
			&m_DeviceContext
		));

		// Gain access to texture subresource in swap chain (back buffer)

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
		GFX_THROW_INFO(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_INFO(m_Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_RenderTargetView));

		// Create Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// Create Depth Stencil State with Device
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW_INFO(m_Device->CreateDepthStencilState(&dsDesc, &pDSState));

		// Set Depth Stencil State to Output Merger stage of pipeline
		m_DeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// Create Depth Stencil texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC dsTexDesc = {};
		dsTexDesc.Width = 1280u;
		dsTexDesc.Height = 960u;
		dsTexDesc.MipLevels = 1u;
		dsTexDesc.ArraySize = 1u;
		dsTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsTexDesc.SampleDesc.Count = 1u;
		dsTexDesc.SampleDesc.Quality = 0u;
		dsTexDesc.Usage = D3D11_USAGE_DEFAULT;
		dsTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		GFX_THROW_INFO(m_Device->CreateTexture2D(&dsTexDesc, nullptr, &pDepthStencil));

		// Create View of Depth Stencil Texture

		// Create Depth Stencil View Description
		D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc = {};
		dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsViewDesc.Texture2D.MipSlice = 0u;

		GFX_THROW_INFO(m_Device->CreateDepthStencilView(pDepthStencil.Get(), &dsViewDesc, &m_DepthStencilView));

		// Bind Depth Stencil View to Output Merger stage of pipeline

		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		// Create Viewport object
		D3D11_VIEWPORT vp;
		vp.Width = 1280;
		vp.Height = 960;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		// Set Viewport(s) to Rasterizer Stage of pipeline
		m_DeviceContext->RSSetViewports(1u, &vp);

	}

	void DirectXGraphics::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void DirectXGraphics::DrawIndexed(UINT count)
	{
		GFX_THROW_INFO_ONLY(m_DeviceContext->DrawIndexed(count, 0u, 0));
	}

	void DirectXGraphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
	{
		m_Projection = proj;
	}

	DirectX::XMMATRIX DirectXGraphics::GetProjection() const noexcept
	{
		return m_Projection;
	}

	void DirectXGraphics::EndFrame()
	{
		HRESULT hr;
#ifndef NDEBUG
		infoManager.Set();
#endif
		// Present
		if (FAILED(hr = m_SwapChain->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(m_Device->GetDeviceRemovedReason());
			} 
			else
			{
				throw GFX_EXCEPT(hr);
			}
		};
	}

	// Graphics exception functions
	DirectXGraphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(line, file),
		hr(hr)
	{
		// Join all information messages with new lines into a single string
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}

		// Remove final new line if it exists
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* DirectXGraphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;

		if (!info.empty())
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}

		oss << GetOriginString();

		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* DirectXGraphics::HrException::GetType() const noexcept
	{
		return "Hazel Graphics Exception";
	}

	HRESULT DirectXGraphics::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string DirectXGraphics::HrException::GetErrorString() const noexcept
	{
		return DXGetErrorStringA(hr);
	}

	std::string DirectXGraphics::HrException::GetErrorDescription() const noexcept
	{
		char buf[512];
		DXGetErrorDescriptionA(hr, buf, sizeof(buf));
		return buf;
	}

	std::string DirectXGraphics::HrException::GetErrorInfo() const noexcept
	{
		return info;
	}

	const char* DirectXGraphics::DeviceRemovedException::GetType() const noexcept
	{
		return "Hazel Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}

	DirectXGraphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
		:
		Exception(line, file)
	{
		// Join all info messages with newlines into single string
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}

		// Remove final newline if exists
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* DirectXGraphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		oss << GetOriginString();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* DirectXGraphics::InfoException::GetType() const noexcept
	{
		return "Hazel Graphics Info Exception";
	}

	std::string DirectXGraphics::InfoException::GetErrorInfo() const noexcept
	{
		return info;
	}
}