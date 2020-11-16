#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {

	class DirectX11RendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColour(float colour) override;
		virtual void Clear() override;

		virtual void DrawIndexed() override;
	};
}