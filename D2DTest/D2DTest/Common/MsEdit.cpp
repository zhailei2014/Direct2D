#include "..\IShare.h"
#include "MsEdit.h"

CMsEdit::CMsEdit() :m_hWnd(NULL), bFocus(false), bShow(false)
{
	nX = nY = nRight = nBottom = 0;
	fAlpha = 255.f;
	clrEdge = RGB(0,0,255);
	clrBk = RGB(255, 255, 255);
	clrText = RGB(0, 0, 0);
	nScaredPos = TEXT_OFFSET_X;
	nSlStart = nSlEnd = 0;
	bSelect = false;
	ZeroMemory(txString, TEXTBOX_MAXLENGTH + 1);
	ZeroMemory(txSlString, TEXTBOX_MAXLENGTH + 1);
}

CMsEdit::~CMsEdit()
{
	m_hWnd = NULL;
	bFocus = false;
	bShow = false;
	nX = nY = nRight = nBottom = 0;
	fAlpha = 255.f;
	clrEdge = RGB(0, 0, 255);
	clrBk = RGB(255, 255, 255);
	clrText = RGB(0, 0, 0);
	nScaredPos = TEXT_OFFSET_X;
	nSlStart = nSlEnd = 0;
	bSelect = true;
	ZeroMemory(txString, TEXTBOX_MAXLENGTH + 1);
	ZeroMemory(txSlString, TEXTBOX_MAXLENGTH + 1);
}

//����EDIT
BOOL CMsEdit::CreateEdit(HWND hwnd, INT x, INT y, INT width, INT hight, bool Show)
{
	m_hWnd = hwnd;
	nX = x;
	nY = y;
	nRight = x + width;
	nBottom = y + hight;
	bShow = Show;
	SetFocus(false);
	return TRUE;
}

//����ѡ���ı���ʵ����λ��
void CMsEdit::SetSelectPosition(int start, int end)
{
	
	nSlStart = start; 
	nSlEnd = end;

	if (start == end)
		bSelect = false;
	else
		bSelect = true;
}
//���ù��λ��
void CMsEdit::SetCaretPos()
{
//
	
}


//���ý���
void CMsEdit::SetFocus(bool bfocus)
{
	bFocus = bfocus; 
	if (bfocus)
	{
		::SetCaretPos(nScaredPos, TEXT_OFFSET_Y);
		::ShowCaret(m_hWnd);
	}
	else
	{
		::HideCaret(m_hWnd);
	}
		
}

//�����ı�
void CMsEdit::SetText(TCHAR code)
{

}


//���´��� ����ʱ���У�
void CMsEdit::UpdateEdit()
{

}

void CMsEdit::OnMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
	{//�Ҽ����¸��ݵ�λ������/ȡ������
		POINT pt;
		pt.x = LOWORD(lParam);  // horizontal position of cursor 
		pt.y = HIWORD(lParam);  // vertical position of cursor 
		RECT rt;
		rt.left = nX;
		rt.top = nY;
		rt.right = nRight;
		rt.bottom = nBottom;
		SetFocus(PtInRect(&rt, pt));
	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);  // horizontal position of cursor 
		pt.y = HIWORD(lParam);  // vertical position of cursor 
		OnLBDown(pt);
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);  // horizontal position of cursor 
		pt.y = HIWORD(lParam);  // vertical position of cursor 
		OnMouseMove(pt);
	}
	break;
	case WM_LBUTTONUP:
	{
		POINT pt;
		pt.x = LOWORD(lParam);  // horizontal position of cursor 
		pt.y = HIWORD(lParam);  // vertical position of cursor 
		OnLBUp(pt);
	}
	break;
	case WM_LBUTTONDBLCLK:
	{//������ڷ�Χ�� ��������˫��
		POINT pt;
		pt.x = LOWORD(lParam);  // horizontal position of cursor 
		pt.y = HIWORD(lParam);  // vertical position of cursor 
		RECT rt;
		rt.left = nX;
		rt.top = nY;
		rt.right = nRight;
		rt.bottom = nBottom;
		if (PtInRect(&rt, pt))
			OnLBDown(pt);
	}
	break;
	case WM_KEYDOWN:
	case WM_CHAR:
	{
		if (bFocus)
			OnKeyMsg(message, wParam, lParam);
	}
	break;
	default:
		break;
	}
}

//����������
void CMsEdit::OnLBDown(POINT pt)
{
	RECT rt;
	rt.left = nX;
	rt.top = nY;
	rt.right = nRight;
	rt.bottom = nBottom;
	if (PtInRect(&rt, pt))
	{
		bFocus = true;
	}
	else
	{
		bFocus = false;
	}
}
//���̧��
void CMsEdit::OnLBUp(POINT pt)
{

}
//������˫��
void CMsEdit::OnLBDBClick(POINT pt)
{

}
//����ƶ�
void CMsEdit::OnMouseMove(POINT pt)
{

}

//���̰���
void CMsEdit::OnKeyMsg(UINT message, WPARAM wParam, LPARAM lParam)
{


}