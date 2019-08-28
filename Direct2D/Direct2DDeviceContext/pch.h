#pragma once

#include "framework.h"
#include "Direct2DDeviceContext.h"
#include <d2d1.h>
#include<d2d1_1.h>
#include<d3d11.h>
#include<wrl/client.h>

#define SAFERELEASE(ppInterfaceToRelease) if(ppInterfaceToRelease){ppInterfaceToRelease->Release() ; ppInterfaceToRelease = NULL ;}

namespace DX
{
	// Convert DirectX error codes to exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DX API errors.
			throw hr;
			//need Windows 8+ OS
			//throw Platform::Exception::CreateException(hr);
		}
	}

#if defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
		);

		return SUCCEEDED(hr);
	}
#endif
}
