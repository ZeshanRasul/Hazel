#pragma once
#include "Bindable.h"
#include "DirectXGraphics.h"

namespace Hazel {

	class PixelShader : public Bindable
	{
	public:
		PixelShader(DirectXGraphics& graphics, LPCWSTR path);
		void Bind(DirectXGraphics& graphics) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	};
}