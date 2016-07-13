#include "../IShare.h"
#include "ChatEditBox.h"


static CChatEdit*	g_xChatEdit = NULL;
extern HWND g_MainWnd;

CChatEdit::CChatEdit()
{
	m_hChatEdit = NULL;
	g_xChatEdit	= this;
	m_hFontChatEdit = NULL;
	m_dwFontColor = RGB(255, 0, 255);
	ZeroMemory(m_szInputMsg, MAX_PATH);
}


CChatEdit::~CChatEdit()
{
	DestroyDialog();
}


BOOL CChatEdit::DestroyDialog()
{
	if ( m_hFontChatEdit != NULL )
	{
		DeleteObject(m_hFontChatEdit);
		m_hFontChatEdit = NULL; 
	}

	DestroyWindow(m_hChatEdit);
	return TRUE;
}


INT_PTR CALLBACK ChatEditDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return g_xChatEdit->ChatEditProc(hWnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK CChatEdit::ChatEditProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (nMsg)
	{
		case WM_KEYDOWN:
		{
			switch ( (INT)wParam )
			{
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
			case VK_ESCAPE:
				{
					HWND m_hMainWindow = g_MainWnd;
					SetFocus(m_hMainWindow);
					SendMessage(m_hMainWindow, WM_KEYDOWN, (INT)wParam, lParam);
					return 0L;
				}
			}
			break;
		}
		case WM_CHAR:
		{
			switch ( (INT)wParam )
			{
			case VK_RETURN:
				{
					GetWindowText(hWnd, m_szInputMsg, MAX_PATH);
					SetWindowText(hWnd, "");
					HWND m_hMainWindow = g_MainWnd;
					SetFocus(m_hMainWindow);
					ShowWindow(m_hChatEdit, SW_HIDE);

					if ( m_szInputMsg )
					{
						if ( m_szInputMsg[0] == '@' )
							SendMessage(m_hMainWindow, _WM_USER_MSG_INPUTTED, TRUE, NULL);
						else
						{
							SendMessage(m_hMainWindow, _WM_USER_MSG_INPUTTED, NULL, NULL);
						}
					}

					return 0L;
				}
			case VK_TAB:
				{
					HWND m_hMainWindow = g_MainWnd;
					SetFocus(m_hMainWindow);
					SendMessage(m_hMainWindow, WM_KEYDOWN, (INT)wParam, lParam);
					return 0L;
				}
			}
			break;
		}
		case WM_PAINT:
		{
			BeginPaint(hWnd, &ps);
			HDC dc = GetWindowDC(hWnd);
			SetBkColor(dc, RGB(100, 100, 0));
			////SetBkMode(dc, TRANSPARENT);
			////SetTextColor((HDC)wParam, RGB(200, 132, 10));//文字颜色
			////SetBkColor((HDC)wParam, GetSysColor(COLOR_3DDKSHADOW));//文字的背后颜色

			InvalidateRect(hWnd, NULL, FALSE);
			EndPaint(hWnd, &ps);
		}
		break;
		//case WM_ERASEBKGND:
		//{
		//	HDC dc = GetWindowDC(hWnd);
		//	SetBkColor(dc, RGB(100, 100, 0));
		//}
			break;
		default:
			break;
	}
	return CallWindowProc(m_WndProcChatEdit, hWnd, nMsg, wParam, lParam);
}



BOOL CChatEdit::Create(HINSTANCE hInstance, HWND hWndParent, INT nX, INT nY, INT nXsize, INT nYSize)
{
	m_hChatEdit = CreateWindowEx(WS_EX_NOPARENTNOTIFY , "EDIT", "", WS_POPUP | WS_VISIBLE | ES_LEFT,
								 nX, nY, nXsize, nYSize,
								 hWndParent, NULL, hInstance, NULL);	  
	m_WndProcChatEdit = (WNDPROC)SetWindowLong(m_hChatEdit, GWL_WNDPROC, (LONG)ChatEditDlgProc);

	if ( m_hFontChatEdit != NULL )
	{
		DeleteObject(m_hFontChatEdit); 
		m_hFontChatEdit = NULL; 
	}

	m_hFontChatEdit = CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, GB2312_CHARSET,
								 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
								 DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");

	SendMessage(m_hChatEdit, WM_SETFONT, (WPARAM)m_hFontChatEdit, MAKELPARAM(TRUE, 0));

	SendMessage(m_hChatEdit, EM_LIMITTEXT, (WPARAM)95, 0);
	
	//SetWindowLong(m_hChatEdit, GWL_EXSTYLE, WS_EX_LAYERED);
	////设置窗口透明度   LWA_ALPHA  LWA_COLORKEY
	//SetLayeredWindowAttributes(m_hChatEdit, 0,10, LWA_ALPHA );
	
	ShowWindow(m_hChatEdit, SW_SHOW);

    UpdateWindow(m_hChatEdit);

	return TRUE;
}



BOOL CChatEdit::SetLimitText(INT nLimitText)
{
	if ( m_hChatEdit )
	{
		SendMessage(m_hChatEdit, EM_LIMITTEXT, (WPARAM)nLimitText, 0);
		return TRUE;
	}
	return FALSE;
}