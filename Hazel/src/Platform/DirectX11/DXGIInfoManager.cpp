#include "hzpch.h"
#include "DXGIInfoManager.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/DirectX11/DirectXGraphics.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

namespace Hazel
{
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw DirectXGraphics::HrException( __LINE__,__FILE__,hr )
}
	DxgiInfoManager::DxgiInfoManager()
	{
		// define function signature of DXGIGetDebugInterface
		typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);
	
		// load the dll that contains the function DXGIGetDebugInterface
		const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
		if (hModDxgiDebug == nullptr)
		{
			throw  Hazel::WindowsWindow::HrException(__LINE__, __FILE__, GetLastError());
		}

		// get address of DXGIGetDebugInterface in dll
		const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
			reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
			);
		if (DxgiGetDebugInterface == nullptr)
		{
			throw Hazel::WindowsWindow::HrException(__LINE__, __FILE__, GetLastError());
		}

		HRESULT hr;
		if (FAILED(hr = (DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pDxgiInfoQueue))))) 
			throw Hazel::DirectXGraphics::HrException(__LINE__, __FILE__, hr);
	}

	DxgiInfoManager::~DxgiInfoManager()
	{
		if (pDxgiInfoQueue != nullptr)
		{
			pDxgiInfoQueue->Release();
		}
	}

	void DxgiInfoManager::Set() noexcept
	{
		// set the index (next) so that the next all to GetMessages()
		// will only get errors generated after this call
		next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	}

	std::vector<std::string> DxgiInfoManager::GetMessages() const
	{
		std::vector<std::string> messages;
		const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
		for (auto i = next; i < end; i++)
		{
			HRESULT hr;
			SIZE_T messageLength;
			// get the size of message i in bytes
			if (FAILED(hr = (pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength)))) throw Hazel::DirectXGraphics::HrException(__LINE__, __FILE__, hr);
			// allocate memory for message
			auto bytes = std::make_unique<byte[]>(messageLength);
			auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			// get the message and push its description into the vector
			if (FAILED(hr = (pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength)))) throw Hazel::DirectXGraphics::HrException(__LINE__, __FILE__, hr);
			messages.emplace_back(pMessage->pDescription);
		}
		return messages;
	}


