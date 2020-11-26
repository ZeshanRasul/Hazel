#pragma once

#include "DirectXGraphics.h"

namespace Hazel {
	class Bindable
	{
	public:
		virtual void Bind(DirectXGraphics& graphics) noexcept = 0;
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext(DirectXGraphics& graphics) noexcept;
		static ID3D11Device* GetDevice(DirectXGraphics& graphics) noexcept;
		static DxgiInfoManager& GetInfoManager(DirectXGraphics& graphics);
	};
}