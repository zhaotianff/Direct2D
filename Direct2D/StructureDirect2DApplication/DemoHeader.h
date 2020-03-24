#pragma once

#include<Windows.h>

#include<d2d1.h>
#include<d2d1helper.h>
#include<dwrite.h>
#include<wincodec.h>

#include<wrl/client.h>

using namespace Microsoft::WRL;

#pragma comment(lib,"d2d1.lib")


class Demo
{
public:
	Demo();
	~Demo();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	//Process and dispatch messages
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
};