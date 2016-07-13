#pragma once
#include "IShare.h"

class CGraphics;

class CTextFormat
{
public:
	CTextFormat();
	~CTextFormat();
private:
	CGraphics *m_graphics;
	IDWriteTextFormat *pTitle,*pText,*pTest;
public:
	IDWriteTextFormat* GetTextFormat(TEXTSTYLE ts);
	HRESULT Initialize(CGraphics *gfx);
	void Release();
};