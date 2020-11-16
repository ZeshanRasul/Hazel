#pragma once

#include "Hazel/Renderer/VertexBuffer.h"

namespace Hazel {

	class DirectX11VertexBuffer : public VertexBuffer
	{
	public:
		DirectX11VertexBuffer(float* vertices, uint32_t size);
		~DirectX11VertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

	private:
		uint32_t m_RendererID;
	};

}