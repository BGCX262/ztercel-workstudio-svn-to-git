#include <tchar.h>
#include <Windows.h>
#include "WindowWnd.h"
#include "wndmain.h"

BOOL CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	CWindowApp theApp;
	
	CWndMain WndMain;
#ifdef _WIN32_WCE
	HWND hWnd = WndMain.Create(_T("app frame"), WS_OVERLAPPED, 0, 0, 480, 272, NULL, NULL);
#else
	HWND hWnd = WndMain.Create(_T("app frame"), WS_OVERLAPPEDWINDOW, 0, 0, 800, 480, NULL, NULL);
#endif
	WndMain.ShowWindow(true);

	theApp.Run();

	return TRUE;
}