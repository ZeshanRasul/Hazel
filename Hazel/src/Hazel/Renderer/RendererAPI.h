#pragma once

namespace Hazel {

	class RendererAPI 
	{
	public: 
		enum class API
		{
			None = 0,
			DirectX11 = 1
		};

	public:
		// TODO change the float to a proper vector
		virtual void SetClearColour(float colour) = 0;
		virtual void Clear() = 0;

		// TODO: DrawIndexed parameters
		virtual void DrawIndexed() = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}