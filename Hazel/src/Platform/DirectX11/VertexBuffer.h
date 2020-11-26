#pragma once
#include "Bindable.h"
#include "DirectXGraphics.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	class VertexBuffer : public Bindable
	{
	public:
		template<class V>
		VertexBuffer(DirectXGraphics& graphics, const std::vector<V>& vertices)
			:stride(sizeof(V))
		{
			INFOMAN(graphics);

			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.CPUAccessFlags = 0u;
			vertexBufferDesc.MiscFlags = 0u;
			vertexBufferDesc.ByteWidth = UINT(sizeof(V) * vertices.size());
			vertexBufferDesc.StructureByteStride = sizeof(V);

			D3D11_SUBRESOURCE_DATA subresourceData = {};
			subresourceData.pSysMem = vertices.data();
			
			GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&vertexBufferDesc, &subresourceData, &m_VertexBuffer));
		}

		void Bind(DirectXGraphics& graphics) noexcept override;
	
	protected:
		UINT stride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	};
}