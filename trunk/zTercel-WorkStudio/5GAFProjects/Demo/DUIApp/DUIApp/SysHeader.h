// SysHeader.h
//------------------------------------------------------------------------------
// 声明:
//			本文件仅定义系统级头文件且超过二个及以上cpp调用的头文件
// 	
//
// 备注:
//==============================================================================
#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

#ifdef _UNICODE
	#define _tstring wstring
#else
	#define _tstring string
#endif
