#pragma once
#include "Bindable.h"
#include "DirectXGraphics.h"

namespace Hazel {
	
	class InputLayout : public Bindable
	{
	public:
		InputLayout(DirectXGraphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);
		void Bind(DirectXGraphics& graphics) noexcept override;

	protected:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}
