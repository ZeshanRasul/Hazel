#include "hzpch.h"
#include "IndexBuffer.h"
#include "Renderer.h"

#include "Platform/DirectX11/DirectX11IndexBuffer.h"

namespace Hazel {

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetApi())
		{
			case RendererAPI::API::None: 
				HZ_CORE_ASSERT(false, "RendererApi::None is not supported!");
				return nullptr;

			case RendererAPI::API::DirectX11: return new DirectX11IndexBuffer(indices, count);
		}

		HZ_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

}