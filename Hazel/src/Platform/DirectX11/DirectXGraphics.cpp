#include "hzpch.h"
#include "Platform/DirectX11/DirectXGraphics.h"

#include "DXErr.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Hazel {

#define GFX_EXCEPT_NOINFO(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw DirectXGraphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
	#define GFX_EXCEPT(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
	#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
	#define GFX_DEVICE_REMOVED_EXCEPT(hr) DirectXGraphics::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
	#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if (!v.empty()) {throw DirectXGraphics::InfoException(__LINE__, __FILE__, v);}}
#else
	#define GFX_EXCEPT(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr))
	#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
	#define GFX_DEVICE_REMOVED_EXCEPT(hr) DirectXGraphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
	#define GFX_THROW_INFO_ONLY (call) (call)
#endif

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

	}

	void DirectXGraphics::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = { red, green, blue, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	}

	void DirectXGraphics::DrawTestTriangle()
	{
		HRESULT hr;

		// 1) Set Vertex Buffer
		// Create Vertex Structure
		struct Vertex
		{
			float x;
			float y;
		};

		// Create an array of vertices for the vertex buffer
		const Vertex vertices[] = 
		{ 
			{0.0f, 0.5f}, 
			{0.5f, -0.5f}, 
			{-0.5f, -0.5f} 
		};

		// Create a buffer and description for the buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(vertices);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(Vertex);

		// Create a subresource and fill that subresource with the array of vertices
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices;
		
		// Create the buffer
		GFX_THROW_INFO(m_Device->CreateBuffer(&bufferDesc, &subresourceData, &pVertexBuffer));
		
		// Bind the Vertex Buffers on the Input Assembler stage of pipeline
		UINT strides = sizeof(Vertex);
		UINT offset = 0;
		m_DeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &strides, &offset);

		// 2) Create Vertex Shader
		// Create a vertex shader and a blob to store the vertex shader file bytecode
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		// Read the compiled shader to a blob
		GFX_THROW_INFO(D3DReadFileToBlob(L"../Hazel/VertexShader.cso", &pBlob));

		// Create the Vertex Shader using the device
		GFX_THROW_INFO(m_Device->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

		// Bind Vertex Shader to the Vertex Shader stage pipeline
		m_DeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// 3) Create Pixel Shader
		// Create a Pixel Shader and a blob to hold the bytecode from the PixelShader.cso
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> pBlobPS;
		
		// Read the compiled Pixel Shader to the Pixel Shader blob
		GFX_THROW_INFO(D3DReadFileToBlob(L"../Hazel/PixelShader.cso", &pBlobPS));

		// Create the Pixel Shader using the blob to fill the Pixel Shader D3D ptr
		GFX_THROW_INFO(m_Device->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), nullptr, &pPixelShader));

		// Bind Pixel Shader to Pixel Shader stage of pipeline
		m_DeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		// 4) Set the Render Target(s)
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), nullptr);

		// 5) Create a Viewport
		
		// Create Viewport object
		D3D11_VIEWPORT vp;
		vp.Width = 800;
		vp.Height = 400;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		// Set Viewport(s) to Rasterizer Stage of pipeline
		m_DeviceContext->RSSetViewports(1u, &vp);


		// Draw call
		GFX_THROW_INFO_ONLY(m_DeviceContext->Draw(3u, 0u));
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