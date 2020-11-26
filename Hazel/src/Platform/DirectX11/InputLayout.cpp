#include "hzpch.h"
#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	InputLayout::InputLayout(DirectXGraphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
	{
		INFOMAN(graphics);

		GFX_THROW_INFO(GetDevice(graphics)->CreateInputLayout(
			layout.data(),
			(UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&m_InputLayout));
	}

	void InputLayout::Bind(DirectXGraphics& graphics) noexcept
	{
		GetContext(graphics)->IASetInputLayout(m_InputLayout.Get());
	}
}