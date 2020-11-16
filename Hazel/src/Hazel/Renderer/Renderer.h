#pragma once

#include "RenderCommand.h"

namespace Hazel {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		// Will eventuall take in a shared ptr vertex array
		static void Submit();

		inline static RendererAPI::API GetApi() { return RendererAPI::GetAPI(); }
	};

}