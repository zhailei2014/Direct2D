#pragma once
#include "..\Graphics.h"

class CControl
{
protected:
	static CGraphics * graphics;
public:
	static void Init(CGraphics *gfx){ graphics = gfx; }
	virtual void OnMsg(UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

