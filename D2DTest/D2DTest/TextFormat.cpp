#include "TextFormat.h"
#include "Graphics.h"

CTextFormat::CTextFormat() :pTitle(NULL), pText(NULL), pTest(NULL), m_graphics(NULL)
{

}

CTextFormat::~CTextFormat()
{
	RELEASE(pTitle); 
	RELEASE(pText);
	RELEASE(pTest);
	m_graphics = NULL;
}

HRESULT CTextFormat::Initialize(CGraphics *gfx)
{
	HRESULT res;
	res=gfx->GetWriteFactory()->CreateTextFormat(L"Î¢ÈíÑÅºÚ",
			NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, 20.0f * 96.0f / 72.0f, L"zh-CN", &pTitle);
	if (SUCCEEDED(res))
		res = gfx->GetWriteFactory()->CreateTextFormat(L"ËÎÌå",
			NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, 12.0f * 96.0f / 72.0f, L"zh-CN", &pText);
	if (SUCCEEDED(res))
		res = gfx->GetWriteFactory()->CreateTextFormat(L"¿¬Ìå",
			NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, 40.0f * 96.0f / 72.0f, L"zh-CN", &pTest);
	return res;
}

void CTextFormat::Release()
{
	RELEASE(pTitle);
	RELEASE(pText);
	RELEASE(pTest);
	m_graphics = NULL;
}

IDWriteTextFormat* CTextFormat::GetTextFormat(TEXTSTYLE ts)
{
	switch (ts)
	{
	case TEXTSTYLE::TITLE:
		return pTitle;
		break;
	case TEXTSTYLE::TEXT:
		return pText;
		break;
	case TEXTSTYLE::TEST:
		return pTest;
		break;
	default:
		return pText;
		break;
	}
}