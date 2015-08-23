#pragma once
#include "WindowWnd.h"

class CWndMain : public CWindowWnd
{
public:
	CWndMain(void);
	~CWndMain(void);
protected:
	const TCHAR* GetWindowClassName();
	LRESULT MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
