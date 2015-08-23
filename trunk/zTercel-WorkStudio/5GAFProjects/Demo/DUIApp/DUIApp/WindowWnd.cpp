#include "WindowWnd.h"
#include <assert.h>

LRESULT CWindowApp::Run()
{
	MSG msg	= {0};
	while (0 != GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

CWindowWnd::CWindowWnd(void) : m_hWnd(NULL), m_fnOldWndProc(DefWindowProc)
{
}

CWindowWnd::~CWindowWnd(void)
{
}

bool CWindowWnd::RegisterWindowClass()
{
	WNDCLASS wc			= {0};
	wc.style			= 0; //CS_HREDRAW|CS_VREDRAW;
	wc.hInstance		= GetModuleHandle(NULL);
	wc.lpfnWndProc		= _WndProc;
	wc.lpszClassName	= GetWindowClassName();
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= NULL; //(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.hCursor			= (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	ATOM ret = RegisterClass(&wc);
	assert(ret!=NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
	return (ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS);
}

HWND CWindowWnd::Create(LPTSTR szWndName, DWORD dwStyle, int x, int y, int cx, int cy, HWND hParent, HMENU hMenu)
{
	DWORD dwError = 0;
	if (NULL != GetWindowClassName() && RegisterWindowClass())
	{
		m_hWnd = CreateWindow(GetWindowClassName(), szWndName, dwStyle, x, y, cx, cy, hParent, hMenu, GetModuleHandle(NULL), this);
	}
	return m_hWnd;
}

HWND CWindowWnd::Create(LPTSTR szWndName, DWORD dwStyle, RECT rtClient, HWND hParent, HMENU hMenu)
{
	return Create(szWndName, dwStyle, rtClient.left, rtClient.top, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, hParent, hMenu);
}

void CWindowWnd::ShowWindow(bool bShow)
{
	if (NULL != m_hWnd)
	{
		::ShowWindow(m_hWnd, bShow ? SW_SHOW : SW_HIDE);
		UpdateWindow(m_hWnd);
	}
	return;
}

LRESULT CWindowWnd::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return m_fnOldWndProc(hWnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK CWindowWnd::_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG	
	TCHAR szMsg[128];
	_stprintf(szMsg, _T("CWindowWnd: 0X%08X \n"), uMsg);
	OutputDebugString(szMsg);
#endif

	CWindowWnd* pThis = NULL;
	if (WM_CREATE == uMsg)
	{
		LPCREATESTRUCT lpCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CWindowWnd*>(lpCreateStruct->lpCreateParams);
		pThis->m_hWnd	= hWnd;
		SetWindowLong(pThis->m_hWnd, GWL_USERDATA, (LPARAM)pThis);
	} else {
		pThis = reinterpret_cast<CWindowWnd*>(GetWindowLong(hWnd, GWL_USERDATA));
	}
	
	switch (uMsg)
	{
		case WM_CREATE:
		{
			pThis->m_DUIRender.Init(hWnd);

			//==: Canvas
			CDUICanvas*	pDUICanvas	= new CDUICanvas;
			TCHAR* szSkinBk[]	= { 		
				_T("E:\\5GAFProjects\\Demo\\DUIApp\\DUIApp\\Res\\Images\\BackGround.bmp")
			};
			pDUICanvas->SetPos(0, 0);
			pDUICanvas->SetSize(800, 480);
			pDUICanvas->SetSkin(szSkinBk[0]);
			pThis->m_DUICtrlList.push_back(pDUICanvas);

			//==: Button
			CDUIButton*	pDUIButton	= new CDUIButton;
			TCHAR* szSkinBtn[]	= { 
				_T("E:\\5GAFProjects\\Demo\\DUIApp\\DUIApp\\Res\\Images\\Setting_FM-n.bmp"),
				_T("E:\\5GAFProjects\\Demo\\DUIApp\\DUIApp\\Res\\Images\\Setting_FM-d.bmp"),
				_T("E:\\5GAFProjects\\Demo\\DUIApp\\DUIApp\\Res\\Images\\Setting_FM-alp.bmp")
			};
			pDUIButton->SetPos(0, 0);
			pDUIButton->SetSkin(szSkinBtn[0], szSkinBtn[1], szSkinBtn[2]);
		
			pThis->m_DUICtrlList.push_back(pDUIButton);

			// CDUIBuilder.Create("MainSurface.xml", DUICtrlList);
			//
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			
			pThis->m_DUIRender.DrawUI(pThis->m_DUICtrlList);

			EndPaint(hWnd, &ps);
			break;
		}
	}

	if (NULL != pThis)
	{
		return pThis->MessageHandler(hWnd, uMsg, wParam, lParam);
	} 

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	
}
