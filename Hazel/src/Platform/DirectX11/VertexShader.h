#pragma once
#include "Bindable.h"

namespace Hazel {

	class VertexShader : public Bindable
	{
	public:
		VertexShader(DirectXGraphics& graphics, LPCWSTR path);
		void Bind(DirectXGraphics& graphics) noexcept override;
		ID3DBlob* GetBytecode() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> m_BytecodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	};
}