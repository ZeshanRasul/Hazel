#include "hzpch.h"
#include "TransformConstantBuffer.h"

namespace Hazel {

	TransformConstantBuffer::TransformConstantBuffer(DirectXGraphics& graphics, const Drawable& parent)
		:
		parent(parent)
	{
		if (!m_VertexConstantBuffer)
		{
			m_VertexConstantBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(graphics);
		}
	}

	void TransformConstantBuffer::Bind(DirectXGraphics& graphics) noexcept
	{
		m_VertexConstantBuffer->Update(graphics,
			DirectX::XMMatrixTranspose(
				parent.GetTransformXM() * graphics.GetProjection()
			)
		);

		m_VertexConstantBuffer->Bind(graphics);
	}

	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>>  TransformConstantBuffer::m_VertexConstantBuffer;
}