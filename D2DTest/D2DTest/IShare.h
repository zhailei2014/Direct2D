#pragma once
#ifndef __ISHARE
#define __ISHARE
#include <tchar.h>
#include <DWrite.h>
#include <Windows.h>

#define RELEASE(p) {if(p){(p)->Release();(p)=NULL;}}
#define SIZEOF(a) (sizeof(a)/sizeof(*(a)))

#define _WM_USER_MSG_INPUTTED WM_USER +1000

//×ÖÌå·ç¸ñ
enum TEXTSTYLE
{
	TITLE,
	TEXT,
	TEST
};




#endif