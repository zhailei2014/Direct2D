#ifndef _MS_EDIT
#define _MS_EDIT
#include "Control.h"

//�ı� ��� X��Yƫ��λ��
#define TEXT_OFFSET_X 5
#define TEXT_OFFSET_Y 3
//�ı���������
#define TEXTBOX_MAXLENGTH 95
//�ı������С(���������ȡ���������Сһ��)
#define TEXT_SIZE 12


class CMsEdit:public CControl
{
public:
	CMsEdit();
	~CMsEdit();
protected:
	//������
	HWND	m_hWnd;
	//�ı���λ�úͿ��
	int		nX, nY, nRight,nBottom;
	//�Ƿ��н���
	bool	bFocus;
	//�Ƿ���ʾ
	bool	bShow;
	//����ʱ͸���� 0-255
	float	fAlpha;
	//�߿���ɫ
	COLORREF clrEdge;
	//��ɫ
	COLORREF clrBk;
	//������ɫ
	COLORREF clrText;
	//�ı�
	TCHAR	txString[TEXTBOX_MAXLENGTH + 1];
	//���λ��
	int		nScaredPos;
	//�Ƿ���ѡ���ı�
	bool bSelect;
	//ѡ���ı�λ��
	int nSlStart, nSlEnd;
	//ѡ���ı�����
	TCHAR	txSlString[TEXTBOX_MAXLENGTH + 1];
public:
	//����edit����
	BOOL CreateEdit(HWND hwnd, INT x, INT y, INT width, INT hight, bool Show);
	
	//���´���
	void UpdateEdit();
	//���ù��λ��
	void SetCaretPos();
	//�����ı�
	void SetText(TCHAR code);
	//��ȡ�ı�
	TCHAR* GetText() { return txString; }
	//����ѡ���ı���ʵ����λ��
	void SetSelectPosition(int start, int end);
	//���ý���
	void SetFocus(bool bfocus);

	//������ʾ
	void SetShow(bool bshow){ bShow = bshow; }
	//����͸����
	void SetShowAlpha(float farf) { fAlpha = farf; }
	//���ñ߿���ɫ
	void SetClrEdge(COLORREF cl){ clrEdge = cl; }
	//���õ�ɫ
	void SetClrBk(COLORREF cl){ clrBk = cl; }
	//����������ɫ
	void SetClrText(COLORREF cl){ clrText = cl; }
public:
	//�������¼�
	virtual void OnMsg(UINT message, WPARAM wParam, LPARAM lParam);
	//����������
	void OnLBDown( POINT pt);
	//���̧��
	void OnLBUp(POINT pt);
	//������˫��
	void OnLBDBClick(POINT pt);
	//����ƶ�
	void OnMouseMove(POINT pt);
	//���̰���
	void OnKeyMsg(UINT message, WPARAM wParam, LPARAM lParam);
};


#endif // _CHATEDITBOX
