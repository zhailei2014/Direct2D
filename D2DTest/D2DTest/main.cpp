#include "Graphics.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	//Ϊָ���Ķ����ö���Ϣ��(Ϊ��ջ��ȫ�����õģ������ÿ����߾��췢�ֶ�ջ����)
	//HeapEnableTerminationOnCorruption�� �ѱ��ƻ��������ֹ
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	//����Ӧ�ó����̷߳�ʽ����com����
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		CGraphics app;
		if (SUCCEEDED(app.Initialize()))
		{
			app.RunMessageLoop();
		}

		//�ڵ�ǰ�̹߳ر�com�⣬ж���̼߳��ص�����dll���ͷ��̼߳��ص�������Դ
		CoUninitialize();
	}
	return 0;
}