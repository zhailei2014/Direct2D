#include "Graphics.h"

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	//为指定的堆设置堆信息。(为堆栈安全而调用的，可以让开发者尽快发现堆栈错误。)
	//HeapEnableTerminationOnCorruption： 堆被破坏则程序中止
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	//告诉应用程序单线程方式创建com对象
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		CGraphics app;
		if (SUCCEEDED(app.Initialize()))
		{
			app.RunMessageLoop();
		}

		//在当前线程关闭com库，卸载线程加载的所有dll，释放线程加载的所有资源
		CoUninitialize();
	}
	return 0;
}