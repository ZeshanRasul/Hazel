#include "hzpch.h"
#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	IndexBuffer::IndexBuffer(DirectXGraphics& graphics, const std::vector<unsigned short>& indices)
		: count((UINT)indices.size())
	{
		INFOMAN(graphics);
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.MiscFlags = 0u;
		indexBufferDesc.ByteWidth = UINT(count * sizeof(unsigned short));
		indexBufferDesc.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
		indexSubresourceData.pSysMem = indices.data();

		GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &m_IndexBuffer));
	}

	void IndexBuffer::Bind(DirectXGraphics& graphics) noexcept
	{
		GetContext(graphics)->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}
}