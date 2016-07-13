#include "Graphics.h"
#include "Common\ChatEditBox.h"
#include "Common\MsEdit.h"

UINT g_WndWidth, g_WndHight;
HWND g_MainWnd;
CChatEdit g_edtMsg;
CMsEdit m_edt;

CGraphics::CGraphics() :m_BRunning(FALSE), m_hInstance(NULL),
m_hWnd(NULL), m_pFactory(NULL), m_pRenderTarget(NULL), m_pWriteFactory(NULL)
{

}

CGraphics::~CGraphics()
{
	m_BRunning = FALSE;
	RELEASE(m_pFactory);
	RELEASE(m_pRenderTarget);
	RELEASE(m_pWriteFactory);
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
		SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LAYERED);
		//设置窗口透明度  
		SetLayeredWindowAttributes(m_hWnd, 0, 255, LWA_ALPHA);

		g_MainWnd = m_hWnd;

		hr = m_hWnd ? S_OK : E_FAIL;

		
		if (SUCCEEDED(hr))
			hr = CreateDeviceResource();

		RECT rt;
		GetWindowRect(m_hWnd, &rt);
		g_edtMsg.Create(m_hInstance, g_MainWnd, rt.left + 100, rt.top + 100, 300, 20);

		//m_edt.CreateEdit("EDIT", m_hInstance, g_MainWnd, rt.left + 100, rt.top + 100, 300, 20, SW_SHOW);

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
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pWriteFactory));

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
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pSolidBrush);

		if (SUCCEEDED(hr))
			hr = m_TextFormat.Initialize(this);
	}
	return hr;
}

void CGraphics::DiscardDeviceResource()
{
	RELEASE(m_pRenderTarget);
	RELEASE(m_pSolidBrush);
	m_TextFormat.Release();
}

HRESULT CGraphics::OnRender()
{
	HRESULT hr = CreateDeviceResource();
	UpDate();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear(D2D1::ColorF(170.f / 255.f, 210.f / 255.f, 235.f / 255.f));
		//在此处添加画面渲染代码...

		//
		m_pRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResource();
		}
		InvalidateRect(m_hWnd, NULL, FALSE);
	}
	return hr;
}

void CGraphics::UpDate()
{

}

