#include "hzpch.h"
#include "Topology.h"
#include "GraphicsThrowMacros.h"

namespace Hazel {

	Topology::Topology(DirectXGraphics& graphics, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{}

	void Topology::Bind(DirectXGraphics& graphics) noexcept
	{
		GetContext(graphics)->IASetPrimitiveTopology(type);
	}
}