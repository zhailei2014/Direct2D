#ifndef _MS_EDIT
#define _MS_EDIT
#include "Control.h"

//文本 光标 X、Y偏移位置
#define TEXT_OFFSET_X 5
#define TEXT_OFFSET_Y 3
//文本字数限制
#define TEXTBOX_MAXLENGTH 95
//文本字体大小(需设置与获取到的字体大小一样)
#define TEXT_SIZE 12


class CMsEdit:public CControl
{
public:
	CMsEdit();
	~CMsEdit();
protected:
	//窗体句柄
	HWND	m_hWnd;
	//文本框位置和宽高
	int		nX, nY, nRight,nBottom;
	//是否有焦点
	bool	bFocus;
	//是否显示
	bool	bShow;
	//隐藏时透明度 0-255
	float	fAlpha;
	//边框颜色
	COLORREF clrEdge;
	//底色
	COLORREF clrBk;
	//文字颜色
	COLORREF clrText;
	//文本
	TCHAR	txString[TEXTBOX_MAXLENGTH + 1];
	//光标位置
	int		nScaredPos;
	//是否有选中文本
	bool bSelect;
	//选中文本位置
	int nSlStart, nSlEnd;
	//选中文本内容
	TCHAR	txSlString[TEXTBOX_MAXLENGTH + 1];
public:
	//创建edit窗体
	BOOL CreateEdit(HWND hwnd, INT x, INT y, INT width, INT hight, bool Show);
	
	//更新窗口
	void UpdateEdit();
	//设置光标位置
	void SetCaretPos();
	//设置文本
	void SetText(TCHAR code);
	//获取文本
	TCHAR* GetText() { return txString; }
	//设置选中文本其实结束位置
	void SetSelectPosition(int start, int end);
	//设置焦点
	void SetFocus(bool bfocus);

	//设置显示
	void SetShow(bool bshow){ bShow = bshow; }
	//设置透明度
	void SetShowAlpha(float farf) { fAlpha = farf; }
	//设置边框颜色
	void SetClrEdge(COLORREF cl){ clrEdge = cl; }
	//设置底色
	void SetClrBk(COLORREF cl){ clrBk = cl; }
	//设置字体颜色
	void SetClrText(COLORREF cl){ clrText = cl; }
public:
	//鼠标键盘事件
	virtual void OnMsg(UINT message, WPARAM wParam, LPARAM lParam);
	//鼠标左键按下
	void OnLBDown( POINT pt);
	//鼠标抬起
	void OnLBUp(POINT pt);
	//鼠标左键双击
	void OnLBDBClick(POINT pt);
	//鼠标移动
	void OnMouseMove(POINT pt);
	//键盘按下
	void OnKeyMsg(UINT message, WPARAM wParam, LPARAM lParam);
};


#endif // _CHATEDITBOX
