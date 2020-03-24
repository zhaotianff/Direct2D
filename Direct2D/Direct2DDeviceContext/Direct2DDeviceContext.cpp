// Direct2DDeviceContext.cpp : Defines the entry point for the application.
//

/*
 *Copyright (C) 2019 zti - All Rights Reserved
 *https://github.com/zhaotianff
 *
 *A Direct2D device context (exposes an ID2D1DeviceContext interface pointer) represents a set of state and command buffers that you use to render to a target. 
 *You can call methods on the device context to set pipeline state and generate rendering commands by using the resources owned by a device.
 *see https://docs.microsoft.com/en-us/windows/win32/direct2d/devices-and-device-contexts#what-is-a-direct2d-device
 *
 *On Windows 7 and earlier, you use a ID2D1HwndRenderTarget or another render target interface to render to a window or surface. 
 *Starting with Windows 8, we do not recommend rendering by using methods that rely on interfaces like ID2D1HwndRenderTarget because they won't work with Windows Store apps. 
 *You can use a device context to render to an Hwnd if you want to make a desktop app and still take advantage of the device context's additional features. 
 *However, the device context is required to render content in a Windows Store apps with Direct2D.
 *
 *Direct2D Effect is only supported by ID2D1DeviceContext interface
 */

#include"framework.h"

#pragma comment(lib,"d2d1.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ComPtr<ID2D1DeviceContext>     m_pD2DContext;
ComPtr<ID2D1Factory2>          m_d2dFactory;
D3D_FEATURE_LEVEL              m_featureLevel;
ComPtr<ID2D1Device1>           m_d2dDevice;
ComPtr<ID2D1DeviceContext1>    m_d2dContext;
ComPtr<IDXGISwapChain1>        m_swapChain;
float                          m_dpi;
ComPtr<ID2D1Bitmap1>           m_d2dTargetBitmap;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HRESULT InitID2D1DeviceContext(ID2D1DeviceContext **pDeviceContext,HWND hwnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2DDEVICECONTEXT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DDEVICECONTEXT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2DDEVICECONTEXT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2DDEVICECONTEXT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HRESULT InitID2D1DeviceContext(ID2D1DeviceContext** ppDeviceContext,HWND hwnd)
{
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
	D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory2),
		&options,
		&m_d2dFactory
	);


	// This flag adds support for surfaces with a different color channel ordering than the API default.
   // You need it for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	// This array defines the set of DirectX hardware feature levels this app  supports.
	// The ordering is important and you should  preserve it.
	// Don't forget to declare your app's minimum required feature level in its
	// description.  All apps are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the DX11 API device object, and get a corresponding context.

	// ComPtr class
	// Creates a smart pointer type that represents the interface specified by the template parameter. 
	// header file : wrl/client.h
	// namespace : Mirosoft::WRL
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	
	D3D11CreateDevice(nullptr, // specify null to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,               //optionally set debug and Direct2D compatibility flags
		featureLevels,               //list of feature levels this app can support
		ARRAYSIZE(featureLevels),    //number of possible feature levels
		D3D11_SDK_VERSION,
		&device,                     //return the Direct3D device created
		&m_featureLevel,             //return feature level of device created
		&context                     //returns the device immediate context
	);

	ComPtr<IDXGIDevice1> dxgiDevice;

	//obtain the underlying DXGI device of the Direct3D11 device
	device.As(&dxgiDevice);

	m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);

	m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext);


	//Selecing a target
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;   //this is the most common swapchain format
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;                  //don't use multi-sampling
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;                       //use double buffering to enable flip 
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;//all apps must use this SwapEffect


	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	dxgiFactory->CreateSwapChainForHwnd(device.Get(),
		hwnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&m_swapChain);

	dxgiDevice->SetMaximumFrameLatency(1);

	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET|D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE));

	ComPtr<IDXGISurface> dxgiBackBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));

	m_d2dContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(),&bitmapProperties,&m_d2dTargetBitmap);

	m_d2dContext->SetTarget(m_d2dTargetBitmap.Get());
}
