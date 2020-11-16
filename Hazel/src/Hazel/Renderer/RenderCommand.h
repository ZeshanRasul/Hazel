#pragma once

#include "RendererAPI.h"

namespace Hazel {

	class RenderCommand
	{
	public:
		// Update to take proper vector
		inline static void SetClearColour(const float colour) 
		{
			s_RendererAPI->SetClearColour(colour);
		}
		
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed()
		{
			s_RendererAPI->DrawIndexed();
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}