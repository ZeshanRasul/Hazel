#pragma once

namespace Hazel {
	
	enum class RendererApi
	{
		None = 0,
		DirectX11 = 1
	};

	class Renderer
	{
	public:
		inline static RendererApi GetRendererApi() { return s_RendererApi; }

	private:
		static RendererApi s_RendererApi;
	};

}