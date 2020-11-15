#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

namespace Hazel {

	class DirectXGraphicsContext : public GraphicsContext
	{
	public:
		DirectXGraphicsContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
	};

}