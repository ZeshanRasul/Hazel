#include "hzpch.h"
#include "TransformConstantBuffer.h"

namespace Hazel {

	TransformConstantBuffer::TransformConstantBuffer(DirectXGraphics& graphics, const Drawable& parent)
		:
		m_VertexConstantBuffer(graphics),
		parent(parent)
	{}

	void TransformConstantBuffer::Bind(DirectXGraphics& graphics) noexcept
	{
		m_VertexConstantBuffer.Update(graphics,
			DirectX::XMMatrixTranspose(
				parent.GetTransformXM() * graphics.GetProjection()
			)
		);

		m_VertexConstantBuffer.Bind(graphics);
	}


}