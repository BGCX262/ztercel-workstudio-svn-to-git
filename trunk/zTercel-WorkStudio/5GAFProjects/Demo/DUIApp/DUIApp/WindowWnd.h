#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "DUICtrlBase.h"

class CWindowWnd
{
public:
	CWindowWnd(void);
	virtual ~CWindowWnd(void);
	
public:
	HWND	Create(LPTSTR szWndName, DWORD dwStyle, int x, int y, int cx, int cy, HWND hParent, HMENU hMenu);
	
	HWND	Create(LPTSTR szWndName, DWORD dwStyle, RECT rtClient, HWND hParent, HMENU hMenu);

	void	ShowWindow(bool bShow);
protected:
	virtual const TCHAR* GetWindowClassName() = 0;

	virtual LRESULT MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual bool RegisterWindowClass();

private:
	static LRESULT CALLBACK _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
protected:
	HWND		m_hWnd;
	WNDPROC		m_fnOldWndProc;

	CDUIRender	m_DUIRender;
	vector<CDUICtrlBase*> m_DUICtrlList;
};


class CWindowApp
{
public:
	LRESULT Run();
	
};