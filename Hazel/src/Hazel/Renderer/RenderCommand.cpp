#include "hzpch.h"

#include "RenderCommand.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"

namespace Hazel {

	RendererAPI* RenderCommand::s_RendererAPI = new DirectX11RendererAPI();

}