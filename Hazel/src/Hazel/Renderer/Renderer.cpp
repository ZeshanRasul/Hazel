#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {
	
	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit()
	{
		// Bind Vertex Array
		RenderCommand::DrawIndexed();
	}

}