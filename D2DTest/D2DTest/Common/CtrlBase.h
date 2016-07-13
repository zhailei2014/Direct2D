#pragma once
#include "Control.h"
#include <list>


class CCtrlBase
{
private:
	static std::list<CControl*> m_CtrlList;
public:
	static void AddContrl(CControl* pCtrl);
	static void OnMsg(UINT message, WPARAM wParam, LPARAM lParam);
	static void ReleaseRes();
};

