#include "hzpch.h"
#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>


namespace Hazel {

	PixelShader::PixelShader(DirectXGraphics& graphics, LPCWSTR path)
	{
		INFOMAN(graphics);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));

		GFX_THROW_INFO(GetDevice(graphics)->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&m_PixelShader));
	}

	void PixelShader::Bind(DirectXGraphics& graphics) noexcept
	{
		GetContext(graphics)->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}
}