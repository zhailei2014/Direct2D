#include "Graphics.h"
#include "Common\CtrlBase.h"

#include "Common\ChatEditBox.h"
#include "Common\MsEdit.h"

using namespace Microsoft::WRL;

UINT g_WndWidth, g_WndHight;
HWND g_MainWnd;
CChatEdit g_edtMsg;
CMsEdit m_edt;

IDWriteTextLayout *pTextlayout;
//WCHAR m_str[MAX_PATH];

CGraphics::CGraphics() :m_BRunning(FALSE), m_hInstance(NULL), m_pFactory1(NULL), m_pDevice(NULL), m_pDeviceContext(NULL),
m_hWnd(NULL), m_pFactory(NULL), m_pRenderTarget(NULL), m_pWriteFactory(NULL), m_pSwapChain(NULL)
{
	m_parameters.DirtyRectsCount = 0;
	m_parameters.pDirtyRects = nullptr;
	m_parameters.pScrollRect = nullptr;
	m_parameters.pScrollOffset = nullptr;
	//ZeroMemory(m_str, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, "123哈哈哈", -1, m_str, MAX_PATH);
}

CGraphics::~CGraphics()
{
	m_BRunning = FALSE;
	RELEASE(m_pFactory);
	RELEASE(m_pFactory1);
	RELEASE(m_pDevice);
	RELEASE(m_pSwapChain);
	RELEASE(m_pDeviceContext);
	RELEASE(m_pRenderTarget);
	RELEASE(m_pWriteFactory);
	RELEASE(m_pD2DTargetBimtap);
	m_hWnd = NULL;
	m_hInstance = NULL;
}

HRESULT CGraphics::Initialize()
{
	HRESULT hr = CreateIndependentDeviceResource();
	if (SUCCEEDED(hr))
	{
		m_hInstance = HINST_THISCOMPONENT;
		WNDCLASSEX wndcls = {sizeof(WNDCLASSEX)};
		wndcls.style = CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = WndProc;
		wndcls.cbClsExtra = 0;
		wndcls.cbWndExtra = sizeof(LONG_PTR);
		wndcls.hInstance = m_hInstance;
		wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndcls.hbrBackground = NULL;//(HBRUSH)COLOR_WINDOW;
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = "BASE";
		RegisterClassEx(&wndcls);

		FLOAT dpiX,dpiY;
		m_pFactory->GetDesktopDpi(&dpiX, &dpiY);

		m_hWnd = CreateWindow("BASE", "D2dTest", WS_OVERLAPPEDWINDOW, 200, 50,
			static_cast<UINT>(ceil(1024.0f * dpiX / 96.0f)),
			static_cast<UINT>(ceil(768.0f * dpiY / 96.0f)),
			NULL, NULL, m_hInstance,this);
		
		// 当取值为LWA_ALPHA时，crKey参数无效，bAlpha参数有效； 
		//当取值为LWA_COLORKEY时，bAlpha参数有效，而窗体中的所有颜色为crKey的地方将变为透明
		//设置窗体扩展样式
		//SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
		////设置窗口透明度  
		//SetLayeredWindowAttributes(m_hWnd, 0, 255, LWA_ALPHA);

		g_MainWnd = m_hWnd;

		hr = m_hWnd ? S_OK : E_FAIL;

		
		if (SUCCEEDED(hr))
			hr = CreateDeviceResource();

		//RECT rt;
		//GetWindowRect(m_hWnd, &rt);
		//g_edtMsg.Create(m_hInstance, g_MainWnd, rt.left + 100, rt.top + 100, 300, 20);

		//m_edt.CreateEdit("EDIT", m_hInstance, g_MainWnd, rt.left + 100, rt.top + 100, 300, 20, SW_SHOW);

		//CCtrlBase::AddContrl(new CMsEdit());


		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hWnd, SW_SHOW);
			m_BRunning = true;
		}
	}


	return hr;
}

void CGraphics::RunMessageLoop()
{
	while (IsRunning())
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CGraphics::OnResize(UINT uWidth, UINT uHight)
{
	if (m_pRenderTarget)
	{
		D2D1_SIZE_U size;
		g_WndWidth = size.width = uWidth;
		g_WndHight = size.height = uHight;
		m_pRenderTarget->Resize(size);
	}
}

LRESULT CALLBACK CGraphics::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT res=0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		CGraphics *pDemoApp = (CGraphics *)pcs->lpCreateParams;
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(pDemoApp));
		res = 1;
	}
	else
	{
		CGraphics *pDemoApp = reinterpret_cast<CGraphics *>(static_cast<LONG_PTR>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;
		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
				RECT rt;
				GetWindowRect(hwnd, &rt);
				MoveWindow(g_edtMsg.GetSafehWnd(), rt.left + 100, rt.top + 100, 300, 20, true);
			}
			res = 0;
			wasHandled = true;
			break;
			case WM_MOVE:
				RECT rt;
				GetWindowRect(hwnd, &rt);
				MoveWindow(g_edtMsg.GetSafehWnd(), rt.left + 100, rt.top + 100, 300, 20, true);
				break;
			case WM_PAINT:
			case WM_DISPLAYCHANGE:
			{
				PAINTSTRUCT ps;
				BeginPaint(hwnd, &ps);
				pDemoApp->OnRender();

				EndPaint(hwnd, &ps);
			}
			res = 0;
			wasHandled = true;
			break;
			case WM_DESTROY:
			{
				pDemoApp->OnDestroy();
				PostQuitMessage(0);
			}
			res = 1;
			wasHandled = true;
			break;
			//case WM_CTLCOLOREDIT:
			//	SetTextColor((HDC)wParam, RGB(200, 132, 10));//文字颜色
			//	SetBkColor((HDC)wParam, GetSysColor(COLOR_3DDKSHADOW));//文字的背后颜色
			//	break;
			case WM_MOUSEMOVE:
			{
				//pTextlayout->HitTestTextRange()
			}
			break;
			}

		}
		if (!wasHandled)
		{
			res = DefWindowProc(hwnd, message, wParam, lParam);
		}

	}
	return res;
}

HRESULT CGraphics::CreateIndependentDeviceResource()
{
	HRESULT hr=0;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);

	if (SUCCEEDED(hr))
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory1);

	if (SUCCEEDED(hr))
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory));
	if (SUCCEEDED(hr))
		hr = CoCreateInstance(CLSID_WICImagingFactory2,nullptr,CLSCTX_INPROC_SERVER,IID_IWICImagingFactory2,reinterpret_cast<void **>(&m_pWICFactory));

	return hr;
}

HRESULT CGraphics::CreateDeviceResource()
{
	HRESULT hr=S_OK;

	if (!m_pRenderTarget)
	{
		RECT rt;
		GetClientRect(m_hWnd, &rt);
		D2D1_SIZE_U uSize = D2D1::SizeU(rt.right - rt.left, rt.bottom - rt.top);

		hr = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, uSize, D2D1_PRESENT_OPTIONS_IMMEDIATELY), &m_pRenderTarget);

		if (SUCCEEDED(hr))
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pSolidBrush);

		if (SUCCEEDED(hr))
			hr = m_TextFormat.Initialize(this);


		if (SUCCEEDED(hr))
			hr = m_pWriteFactory->CreateTextLayout(m_str, MAX_PATH, GetTextFormat(TEXTSTYLE::TEXT), 100, 30, &pTextlayout);





		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

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
		D3D_FEATURE_LEVEL m_featureLevel;

		ID3D11Device *pD3Ddevice = nullptr;

		ComPtr<ID3D11DeviceContext> context;

		if (SUCCEEDED(hr))
			hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, creationFlags, featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, &pD3Ddevice, &m_featureLevel, &context);

		ComPtr<IDXGIDevice1> dxgiDevice;
		if (SUCCEEDED(hr))
			hr = pD3Ddevice->QueryInterface(__uuidof(IDXGIDevice1), &dxgiDevice);

		if (SUCCEEDED(hr))
			hr = m_pFactory1->CreateDevice(dxgiDevice.Get(), &m_pDevice);

		if (SUCCEEDED(hr))
			hr = m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pDeviceContext);


		// 获取窗口大小
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		rect.right -= rect.left;
		rect.bottom -= rect.top;


		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
		// 设置刷新率60FPS
		fullscreenDesc.RefreshRate.Numerator = 60;
		fullscreenDesc.RefreshRate.Denominator = 1;
		// 扫描方案
		fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		// 缩放方案
		fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// 全屏显示
		fullscreenDesc.Windowed = true;

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		swapChainDesc.Width = rect.right;                           // use automatic sizing
		swapChainDesc.Height = rect.bottom;
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
		swapChainDesc.Stereo = false;
		swapChainDesc.SampleDesc.Count = 1;                // don't use multi-sampling
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;                     // use double buffering to enable flip
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // all apps must use this SwapEffect
		swapChainDesc.Flags = 0;

		ComPtr<IDXGIAdapter> dxgiAdapter;

		if (SUCCEEDED(hr))
			hr = dxgiDevice->GetAdapter(&dxgiAdapter);

		ComPtr<IDXGIFactory2> dxgiFactory;
		if (SUCCEEDED(hr))
			hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		if (SUCCEEDED(hr))
			hr = dxgiFactory->CreateSwapChainForHwnd(pD3Ddevice, m_hWnd, &swapChainDesc, &fullscreenDesc, nullptr, &m_pSwapChain);

		if (SUCCEEDED(hr))
			hr = dxgiDevice->SetMaximumFrameLatency(1);


		ComPtr<ID3D11Texture2D> backBuffer;

		if (SUCCEEDED(hr))
			hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

		if (SUCCEEDED(hr)) {
			D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				FIXED_DPI,
				FIXED_DPI
				);

			ComPtr<IDXGISurface> dxgiBackBuffer;

			hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
			if (SUCCEEDED(hr))
				hr = m_pDeviceContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				&bitmapProperties,
				&m_pD2DTargetBimtap
				);
			if (SUCCEEDED(hr))
			{
				m_pDeviceContext->SetTarget(m_pD2DTargetBimtap);
				m_pDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
			}
		}


		if (SUCCEEDED(hr))
			hr = LoadBitmapFromFile(m_pDeviceContext, m_pWICFactory, L"1.jpg", 0, 0, &m_pBitmapLoaded);

		
		if (SUCCEEDED(hr))
			hr = m_pDeviceContext->CreateEffect(/*CLSID_D2D1DirectionalBlur*/CLSID_D2D1ColorMatrix, &m_pEffectTest);
		// 设置输入

		D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(1, 0, 0, 0.05,  0, 1, 0, 0.05,  0, 0, 1, 0.05,  0, 0, 0, 1, 0, 0, 0, 0);
		if (SUCCEEDED(hr))
		{
			m_pEffectTest->SetInput(0, m_pBitmapLoaded);
			hr = m_pEffectTest->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
		}
			

		RELEASE(pD3Ddevice);
	}
	return hr;
}

void CGraphics::DiscardDeviceResource()
{
	RELEASE(m_pRenderTarget);
	RELEASE(m_pSolidBrush);
	RELEASE(m_pDeviceContext);
	m_TextFormat.Release();
}

HRESULT CGraphics::OnRender()
{
	HRESULT hr = CreateDeviceResource();
	UpDate();

	if (SUCCEEDED(hr))
	{
		m_pDeviceContext->BeginDraw();
		m_pDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

		m_pDeviceContext->DrawImage(m_pEffectTest/*.Get()*/, D2D1_POINT_2F());

		m_pDeviceContext->EndDraw();
		
		//m_pRenderTarget->BeginDraw();
		//m_pRenderTarget->Clear(D2D1::ColorF(170.f / 255.f, 210.f / 255.f, 235.f / 255.f));
		////在此处添加渲染代码...

		//m_pRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResource();
		}
		hr = m_pSwapChain->Present1(1, 0, &m_parameters);
		//InvalidateRect(m_hWnd, NULL, FALSE);
	}
	return hr;
}

void CGraphics::UpDate()
{

}



// 从文件读取位图
HRESULT CGraphics::LoadBitmapFromFile(ID2D1DeviceContext *pRenderTarget,IWICImagingFactory2 *pIWICFactory,PCWSTR uri,UINT destinationWidth,UINT destinationHeight,ID2D1Bitmap1 **ppBitmap)
{
	IWICBitmapDecoder *pDecoder = nullptr;
	IWICBitmapFrameDecode *pSource = nullptr;
	IWICStream *pStream = nullptr;
	IWICFormatConverter *pConverter = nullptr;
	IWICBitmapScaler *pScaler = nullptr;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
		uri,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
		);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr))
	{
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}


	if (SUCCEEDED(hr))
	{
		if (destinationWidth != 0 || destinationHeight != 0)
		{
			UINT originalWidth, originalHeight;
			hr = pSource->GetSize(&originalWidth, &originalHeight);
			if (SUCCEEDED(hr))
			{
				if (destinationWidth == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
					destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
				}
				else if (destinationHeight == 0)
				{
					FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
					destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
				}

				hr = pIWICFactory->CreateBitmapScaler(&pScaler);
				if (SUCCEEDED(hr))
				{
					hr = pScaler->Initialize(
						pSource,
						destinationWidth,
						destinationHeight,
						WICBitmapInterpolationModeCubic
						);
				}
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(
						pScaler,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						nullptr,
						0.f,
						WICBitmapPaletteTypeMedianCut
						);
				}
			}
		}
		else
		{
			hr = pConverter->Initialize(
				pSource,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				nullptr,
				0.f,
				WICBitmapPaletteTypeMedianCut
				);
		}
	}
	if (SUCCEEDED(hr))
	{
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			nullptr,
			ppBitmap
			);
	}

	RELEASE(pDecoder);
	RELEASE(pSource);
	RELEASE(pStream);
	RELEASE(pConverter);
	RELEASE(pScaler);

	return hr;
}
