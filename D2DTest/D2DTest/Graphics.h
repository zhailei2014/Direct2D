#pragma once
#ifndef __GRAPHIC
#define __GRAPHIC

#include <D2D1.h>
#include <DxErr.h>
#include <xnamath.h>
#include "IShare.h"
#include "TextFormat.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif




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

public:
	IDWriteRenderingParams *pWriteparam;
};

#endif