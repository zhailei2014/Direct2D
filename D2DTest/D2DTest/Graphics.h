#pragma once
#ifndef __GRAPHIC
#define __GRAPHIC

#include <D2D1_1.h>
#include <DxErr.h>
#include <dwrite_2.h>
#include <d3d11_1.h>
#include <WRL\client.h>
#include <wincodec.h>
#include <d2d1effects.h>

#include <xnamath.h>
#include "IShare.h"
#include "TextFormat.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#define FIXED_DPI (96.f)


class CGraphics
{
public:
	CGraphics();
	~CGraphics();
	
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

	ID2D1Factory *m_pFactory;
	ID2D1HwndRenderTarget *m_pRenderTarget;
	IDWriteFactory *m_pWriteFactory;
	ID2D1SolidColorBrush *m_pSolidBrush;
	CTextFormat m_TextFormat;
	BOOL m_BRunning;

	ID2D1Factory1 *m_pFactory1;
	ID2D1Device *m_pDevice;
	ID2D1DeviceContext * m_pDeviceContext;
	IDXGISwapChain1 *m_pSwapChain;
	// 载入的图片
	ID2D1Bitmap1* m_pBitmapLoaded = nullptr;
	ID2D1Bitmap1* m_pD2DTargetBimtap=nullptr;
	// WIC 工厂
	IWICImagingFactory2* m_pWICFactory = nullptr;
	// 测试特效
	ID2D1Effect* m_pEffectTest = nullptr;
	
	// 手动交换链
	DXGI_PRESENT_PARAMETERS m_parameters;
public:
	BOOL IsRunning(){ return m_BRunning; }
	void OnDestroy(){ m_BRunning = FALSE; }
	

	HRESULT Initialize();
	void RunMessageLoop();
	void OnResize(UINT uWidth, UINT uHight);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	ID2D1Factory * GetFactory() { return m_pFactory; }
	ID2D1HwndRenderTarget *GetRenderTarget(){ return m_pRenderTarget; }
	IDWriteFactory * GetWriteFactory(){ return m_pWriteFactory; }
	ID2D1SolidColorBrush *GetSolidBrush() { return m_pSolidBrush; }
	IDWriteTextFormat* GetTextFormat(TEXTSTYLE ts){ return m_TextFormat.GetTextFormat(ts); }

	HRESULT CreateIndependentDeviceResource();
	HRESULT CreateDeviceResource();
	void DiscardDeviceResource();

	HRESULT OnRender();
	void UpDate();


	HRESULT LoadBitmapFromFile(ID2D1DeviceContext *pRenderTarget,IWICImagingFactory2 *pIWICFactory,PCWSTR uri,UINT destinationWidth,UINT destinationHeight,ID2D1Bitmap1 **ppBitmap);
//测试
public:
	
	WCHAR m_str[MAX_PATH];
};

#endif