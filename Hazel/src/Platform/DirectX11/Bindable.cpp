#include "hzpch.h"
#include "Bindable.h"

namespace Hazel {

	ID3D11DeviceContext* Bindable::GetContext(DirectXGraphics& graphics) noexcept
	{
		return graphics.m_DeviceContext.Get();
	}

	ID3D11Device* Bindable::GetDevice(DirectXGraphics& graphics) noexcept
	{
		return graphics.m_Device.Get();
	}

	DxgiInfoManager& Bindable::GetInfoManager(DirectXGraphics& graphics)
	{
#ifdef HZ_DEBUG
		return graphics.infoManager;
#else
		throw std::logic_error("Error! You tried to access graphics.infoManager in Release config!");
#endif
	}
}
