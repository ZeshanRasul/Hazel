#pragma once
#include "Bindable.h"
#include "DirectXGraphics.h"

namespace Hazel {

	class Topology : public Bindable
	{
	public:
		Topology(DirectXGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(DirectXGraphics& graphics) noexcept override;
	
	private:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
}