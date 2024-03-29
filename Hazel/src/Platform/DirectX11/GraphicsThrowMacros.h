#pragma once

#define GFX_EXCEPT_NOINFO(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED(hr = (hrcall))) throw DirectXGraphics::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DirectXGraphics::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if (!v.empty()) {throw DirectXGraphics::InfoException(__LINE__, __FILE__, v);}}
#else
#define GFX_EXCEPT(hr) DirectXGraphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DirectXGraphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY (call) (call)
#endif

// macro for importing infomanager into local scope
// this.GetInfoManager(graphics) must exist
#ifdef NDEBUG
#define INFOMAN(graphics) HRESULT hr
#else
#define INFOMAN(graphics) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((graphics))
#endif