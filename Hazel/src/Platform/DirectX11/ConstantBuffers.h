#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	template <typename C>
	class ConstantBuffer : public Bindable
	{
	public:
		void Update(DirectXGraphics& graphics, const C& consts)
		{
			INFOMAN(graphics);

			D3D11_MAPPED_SUBRESOURCE msr;
			GFX_THROW_INFO(GetContext(graphics)->Map(m_ConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &consts, sizeof(consts));
			GetContext(graphics)->Unmap(m_ConstantBuffer.Get(), 0u);
		}

		ConstantBuffer(DirectXGraphics& graphics, const C& consts)
		{
			INFOMAN(graphics);

			D3D11_BUFFER_DESC constantBufferDesc;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.MiscFlags = 0u;
			constantBufferDesc.ByteWidth = sizeof(consts);
			constantBufferDesc.StructureByteStride = 0u;

			D3D11_SUBRESOURCE_DATA cBufSubresourceData= {};
			cBufSubresourceData.pSysMem = &consts;

			GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, &cBufSubresourceData, &m_ConstantBuffer));

		}

		ConstantBuffer(DirectXGraphics& graphics)
		{
			INFOMAN(graphics);

			D3D11_BUFFER_DESC constantBufferDesc;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.MiscFlags = 0u;
			constantBufferDesc.ByteWidth = sizeof(C);
			constantBufferDesc.StructureByteStride = 0u;

			GFX_THROW_INFO(GetDevice(graphics)->CreateBuffer(&constantBufferDesc, nullptr, &m_ConstantBuffer));

		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
	};

	template<typename C>
	class VertexConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_ConstantBuffer;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(DirectXGraphics& graphics) noexcept override
		{
			GetContext(graphics)->VSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};

	template<typename C>
	class PixelConstantBuffer : public ConstantBuffer<C>
	{
		using ConstantBuffer<C>::m_ConstantBuffer;
		using Bindable::GetContext;
	public:
		using ConstantBuffer<C>::ConstantBuffer;
		void Bind(DirectXGraphics& graphics) noexcept override
		{
			GetContext(graphics)->PSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
		}
	};
}