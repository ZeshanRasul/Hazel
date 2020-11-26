#include "hzpch.h"
#include "VertexBuffer.h"

namespace Hazel {

	void VertexBuffer::Bind(DirectXGraphics& graphics) noexcept
	{
		UINT offset = 0u;
		GetContext(graphics)->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}
}