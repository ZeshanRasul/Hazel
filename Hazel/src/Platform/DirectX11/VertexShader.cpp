#include "hzpch.h"
#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>


namespace Hazel {

	VertexShader::VertexShader(DirectXGraphics& graphics, LPCWSTR path)
	{
		INFOMAN(graphics);

		GFX_THROW_INFO(D3DReadFileToBlob(path, &m_BytecodeBlob));
		GFX_THROW_INFO(GetDevice(graphics)->CreateVertexShader(
			m_BytecodeBlob->GetBufferPointer(),
			m_BytecodeBlob->GetBufferSize(),
			nullptr,
			m_VertexShader.GetAddressOf()));
	}

	void VertexShader::Bind(DirectXGraphics& graphics) noexcept
	{
		GetContext(graphics)->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* VertexShader::GetBytecode() const noexcept
	{
		return m_BytecodeBlob.Get();
	}
}
