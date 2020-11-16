#include "hzpch.h"
#include "VertexBuffer.h"
#include "Renderer.h"

#include "Platform/DirectX11/DirectX11VertexBuffer.h"

namespace Hazel {

	VertexBuffer* VertexBuffer::Create(float* indices, uint32_t size)
	{
		switch (Renderer::GetRendererApi())
		{
			case RendererApi::None: 
				HZ_CORE_ASSERT(false, "No Renderer API selected!");
				return nullptr;

			case RendererApi::DirectX11: return new DirectX11VertexBuffer(indices, size);			
		}

		HZ_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}