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

BOOL CMsEdit::CreateEdit(HWND hwnd, INT x, INT y, INT width, INT hight, bool Show)
{
	m_hWnd = hwnd;
	nX = x;
	nY = y;
	nRight = x + width;
	nBottom = y + hight;
	bShow = Show;
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
//���ý���
void CMsEdit::SetFocus(bool bfocus)
{
	bFocus = bfocus; 
	if (bfocus)
		::ShowCaret(m_hWnd);
	else
		::HideCaret(m_hWnd);
}








//����������
void CMsEdit::OnLBDown(POINT pt)
{

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