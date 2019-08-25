// Direct2DBrushes.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Direct2DBrushes.h"
#include <d2d1.h>
#include <exception>
#include<wincodec.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"windowscodecs.lib")

#define MAX_LOADSTRING 100

#define SAFERELEASE(ppInterfaceToRelease) if(ppInterfaceToRelease){ppInterfaceToRelease->Release() ; ppInterfaceToRelease = NULL ;}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1LinearGradientBrush* pLinearBrush = NULL;
ID2D1BitmapBrush* pBitmapBrush = NULL;
ID2D1RadialGradientBrush* pRadialBrush = NULL;
BOOL bInitBrushes = FALSE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HRESULT InitD2D1(HWND hWnd);
HRESULT InitBrushes();
HRESULT CreateD2D1ColorBrush(ID2D1SolidColorBrush** pSollidColorBrush, D2D1::ColorF color);
HRESULT CreateD2D1LinearGradientBrush(ID2D1LinearGradientBrush** pLinearGradientBrush);
HRESULT CreateD2D1RadialGradientBrush(ID2D1RadialGradientBrush** pRadialGradientBrush);
HRESULT CreateD2D1BitmapBrush(PCWSTR uri, ID2D1BitmapBrush** pBitmapBrush);
void DrawBrushes();
void ReleaseResource();
HRESULT LoadD2D1BitampFromFile(PCWSTR uri,ID2D1Bitmap** ppBitmap);

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
    LoadStringW(hInstance, IDC_DIRECT2DBRUSHES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DBRUSHES));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2DBRUSHES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2DBRUSHES);
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

   InitD2D1(hWnd);
  
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
		DrawBrushes();
        }
        break;
    case WM_DESTROY:
		ReleaseResource();
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

HRESULT InitD2D1(HWND hWnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	if (SUCCEEDED(hr) == false)
		throw "Create D2D1 Factory failed";

	RECT rt;

	GetClientRect(hWnd,&rt);

	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(hWnd,D2D1::SizeU(rt.right-rt.left,rt.bottom-rt.top)),&pRT);

	if (SUCCEEDED(hr) == false)
		throw "Create HwndRenderTarget failed";

	return hr;
}

HRESULT InitBrushes()
{
	if (bInitBrushes == TRUE)
		return S_OK;

	//LinearGradientBrush
	HRESULT hr = CreateD2D1LinearGradientBrush(&pLinearBrush);

	if (SUCCEEDED(hr) == false)
		throw "";

	//BitmapBrush
	hr = CreateD2D1BitmapBrush(L"0.jpg", &pBitmapBrush);

	if (SUCCEEDED(hr) == false)
		throw "";

	//RadialGradientBrush
	hr = CreateD2D1RadialGradientBrush(&pRadialBrush);

	if (SUCCEEDED(hr) == false)
		throw "";

	bInitBrushes = TRUE;

	return hr;
}

HRESULT CreateD2D1ColorBrush(ID2D1SolidColorBrush** pSollidColorBrush,D2D1::ColorF color = D2D1::ColorF::Green)
{
	HRESULT hr = pRT->CreateSolidColorBrush(color, pSollidColorBrush);
	return hr;
}

HRESULT CreateD2D1LinearGradientBrush(ID2D1LinearGradientBrush** pLinearGradientBrush)
{
	//Create an array of gradient stops to put in the gradient stop
    //collection that will be used in the gradient brush.

	ID2D1GradientStopCollection* pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black);
	gradientStops[1].position = 1.0f;

	HRESULT hr = pRT->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pGradientStops);
	
	// The line that determines the direction of the gradient starts at
	// the upper-left corner of the square and ends at the lower-right corner.
	if (SUCCEEDED(hr) == false)
		throw "CreateGradientStopCollection failed";

	pRT->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(300, 300)),
		pGradientStops,
		pLinearGradientBrush);

	SAFERELEASE(pGradientStops);

	return hr;
}

HRESULT CreateD2D1RadialGradientBrush(ID2D1RadialGradientBrush** pRadialGradientBrush)
{
	// Create an array of gradient stops to put in the gradient stop
    // collection that will be used in the gradient brush.
	ID2D1GradientStopCollection* pGradientStops = NULL;

	D2D1_GRADIENT_STOP gradientStops[2];
	gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::Green);
	gradientStops[0].position = 0.0f;
	gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Yellow);
	gradientStops[1].position = 1.0f;

	HRESULT hr = pRT->CreateGradientStopCollection(gradientStops, 2, &pGradientStops);

	if (SUCCEEDED(hr) == FALSE)
		throw "CreateGradientStopCollection failed";

	hr = pRT->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(750, 150), D2D1::Point2F(0, 0), 150, 150),  pGradientStops, pRadialGradientBrush);

	SAFERELEASE(pGradientStops);

	return hr;
}

HRESULT CreateD2D1BitmapBrush(PCWSTR uri ,ID2D1BitmapBrush** pBitmapBrush)
{
	ID2D1Bitmap* pBitmap = NULL;
	HRESULT hr = LoadD2D1BitampFromFile(uri, &pBitmap);

	if ((SUCCEEDED(hr)) == false)
		throw "Load bitmap from file failed";
	pRT->CreateBitmapBrush(pBitmap, pBitmapBrush);
	SAFERELEASE(pBitmap);
	return hr;
}

void DrawBrushes()
{	
	InitBrushes();

	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	//bitmap brush
	pRT->FillRectangle(D2D1::RectF(0.f,0.f,300.f,300.f), pLinearBrush);

	//linear gradient brush
	pRT->FillRectangle(D2D1::RectF(300.f,0.f,600.f,300.f), pBitmapBrush);

	//radial gradient brush
	pRT->FillEllipse(D2D1::Ellipse(D2D1::Point2F(750.f,150.f),150.f,150.f), pRadialBrush);

	//solid color brush 
	//Refrence Direct2DquickStart

	pRT->EndDraw();
}

void ReleaseResource()
{
	SAFERELEASE(pD2DFactory);
	SAFERELEASE(pRT);
}

HRESULT LoadD2D1BitampFromFile(PCWSTR uri, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;
	IWICImagingFactory* pIWICFactory = NULL;

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory1,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&pIWICFactory)
	);

	if (SUCCEEDED(hr) == false)
		throw "";

	hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr) == false)
		throw "";

	hr = pDecoder->GetFrame(0, &pSource);

	if (SUCCEEDED(hr) == false)
		throw "";

	hr = pIWICFactory->CreateFormatConverter(&pConverter);

	if (SUCCEEDED(hr) == false)
		throw "";

	hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

	if (SUCCEEDED(hr) == false)
		throw "";

	hr = pRT->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);


	SAFERELEASE(pDecoder);
	SAFERELEASE(pSource);
	SAFERELEASE(pStream);
	SAFERELEASE(pConverter);
	SAFERELEASE(pScaler);
	SAFERELEASE(pIWICFactory);

	return hr;
}
