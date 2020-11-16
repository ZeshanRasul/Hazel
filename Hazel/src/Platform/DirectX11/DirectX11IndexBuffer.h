#pragma once

#include "Hazel/Renderer/IndexBuffer.h"

namespace Hazel {

	class DirectX11IndexBuffer : public IndexBuffer
	{
	public:
		DirectX11IndexBuffer(uint32_t* indices, uint32_t count);
		~DirectX11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}