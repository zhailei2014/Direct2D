#include "CtrlBase.h"

std::list<CControl*> CCtrlBase::m_CtrlList;

void CCtrlBase::AddContrl(CControl* pCtrl)
{
	m_CtrlList.push_back(pCtrl);
}

void CCtrlBase::OnMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_CtrlList.size() > 0)
	{
		for (auto ctrl : m_CtrlList)
		{
			ctrl->OnMsg(message, wParam, lParam);
		}
	}
}

void CCtrlBase::ReleaseRes()
{
	if (m_CtrlList.size() > 0)
	{
		for (auto ctrl : m_CtrlList)
		{
			delete ctrl;
			ctrl = NULL;
		}
		m_CtrlList.clear();
	}
}