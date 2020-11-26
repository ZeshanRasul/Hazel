#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

namespace Hazel {

	class TransformConstantBuffer : public Bindable
	{
	public:
		TransformConstantBuffer(DirectXGraphics& graphics, const Drawable& parent);
		void Bind(DirectXGraphics& graphics) noexcept override;
	
	private:
		VertexConstantBuffer<DirectX::XMMATRIX> m_VertexConstantBuffer;
		const Drawable& parent;
	};
}