#pragma once
#include "Bindable.h"
#include "DirectXGraphics.h"

namespace Hazel {
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(DirectXGraphics& graphics, const std::vector<unsigned short>& indices);
		void Bind(DirectXGraphics& graphics) noexcept override;
		UINT GetCount() const noexcept;
	
	protected:
		UINT count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	};
}