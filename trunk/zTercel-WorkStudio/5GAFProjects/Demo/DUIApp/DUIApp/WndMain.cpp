#include "WndMain.h"
#include "Texture.h"

typedef struct tagTImageInfo
{
	HBITMAP		m_hBitmap;
	int			m_nWidth;
	int			m_nHeight;
	int			m_nBitCount;
	bool		m_bAlpha;
} TImageInfo;

DWORD dwDecode = 0;

TImageInfo* LoadImage(HWND hWnd, const TCHAR* szPath)
{
	DWORD dwStart = GetTickCount();

	LPBYTE pData = NULL;
	HANDLE hFile = ::CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE ) return NULL;
	DWORD dwSize = ::GetFileSize(hFile, NULL);
	if (dwSize == 0) return NULL;

	DWORD dwRead = 0;
	pData = new BYTE[dwSize];
	::ReadFile(hFile, pData, dwSize, &dwRead, NULL);
	::CloseHandle(hFile);

	CTexLoader TexLoader;
	STexture  texBackground;
	TImageInfo* data = new TImageInfo;
	if (TexLoader.Load(pData, dwSize, texBackground, TEXTYPE_BMP))
	{
		if (NULL != pData) 
		{
			delete[] pData;
			pData = NULL;
		}

		data->m_hBitmap		= texBackground;
		data->m_nWidth		= texBackground.GetWidth();
		data->m_nHeight		= texBackground.GetHeight();
		data->m_nBitCount	= texBackground.GetBitCount();
		data->m_bAlpha		= false;
		
		dwDecode = GetTickCount() - dwStart;

		return data;
	}

	return data;
}

CWndMain::CWndMain(void)
{

}

CWndMain::~CWndMain(void)
{

}

const TCHAR* CWndMain::GetWindowClassName()
{
	return _T("DUIWndMain");
}


TImageInfo* pImageInfo = NULL;
LRESULT CWndMain::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	TCHAR szMsg[128];
	_stprintf(szMsg, _T("CWndMain: 0X%08X \n"), uMsg);
	OutputDebugString(szMsg);
#endif

	static HDC		hOffSrnDC	= NULL;
	static HBITMAP	hOffSrnBmp	= NULL;
	switch (uMsg)
	{
		case WM_CREATE:
		{	
			DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
			dwStyle &= ~WS_CAPTION;
			SetWindowLong(hWnd, GWL_STYLE, dwStyle);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_DRAWFRAME|SWP_NOSIZE);
			
			//==: test env: sansung 2443, 128MB, 480 * 272£¬   image: 800 
			//TCHAR*	szPath = _T("d:\\Images\\04Xbit.bmp");
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\08xRGB.bmp");		//  OK
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\16x565.bmp");		// dec: 256ms, dis: 4ms fields
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\16A555.bmp");		// dec: 225ms, dis: 53ms fields
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\24xRGB.bmp");		// dec: 265ms, dis: 42ms ok
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\16x555.bmp");		// dec: 184ms, dis: 53ms ok
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\32ARGB.bmp");		// dec: 338ms, dis: 47ms ok
			//TCHAR*	szPath = _T("\\NandFlash\\Images\\32XRGB.bmp");		// dec: 3454ms, dis: 56ms fields ok

			//TCHAR*	szPath = _T("E:\\5GAFProjects\\Resource\\Images\\Demo\\DUIApp\\08xrgb.BMP");

			//DWORD dwSize = sizeof(USHORT);
			//pImageInfo = LoadImage(hWnd, szPath);
			
			break;
		}
		case WM_LBUTTONDOWN:
		{
#ifdef _WIN32
			PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
#endif
			//for (int ii = 0; ii < 100; ii++)
// 			{
// 				RECT rt;
// 				GetClientRect(hWnd, &rt);
// 				InvalidateRect(hWnd, &rt, TRUE);
// 			}
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps	= {0};
			HDC hDC = BeginPaint(hWnd, &ps);
			
			if (NULL != pImageInfo)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				if (NULL == hOffSrnDC)
				{
					hOffSrnDC = CreateCompatibleDC(hDC);
					hOffSrnBmp = CreateCompatibleBitmap(hDC, rect.right - rect.left ,rect.bottom - rect.top);
					HBRUSH hBrush =  CreateSolidBrush(RGB(128,128,128));
					FillRect(hOffSrnDC, &rect, hBrush);
					SelectObject(hOffSrnDC, hOffSrnBmp);
				}

				DWORD dwStart = GetTickCount();
				if (NULL != pImageInfo && NULL != pImageInfo->m_hBitmap)
				{
					HDC hMemDC = CreateCompatibleDC(hDC);
					HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, pImageInfo->m_hBitmap);
	 				
					BitBlt(hOffSrnDC, 0, 0, 800, 480, hMemDC, 0, 0, SRCCOPY);		
					SelectObject(hMemDC, hOldBmp);
					DeleteDC(hMemDC);
				}

				TCHAR szTime[128];
				_stprintf(szTime, _T("decode & display time: %d x %dms [%d x %d x %d]"), 
							dwDecode, GetTickCount() - dwStart, 
							pImageInfo->m_nWidth, pImageInfo->m_nHeight, pImageInfo->m_nBitCount);
				SetBkMode(hOffSrnDC, TRANSPARENT);
				::DrawText(hOffSrnDC, szTime, -1, &rect, 0);
				
				BitBlt(hDC, 0, 0, 800, 480, hOffSrnDC, 0, 0, SRCCOPY);
			}
			EndPaint(hWnd, &ps);

			return 0;
		}
		case WM_DESTROY:
		{
			if (NULL != pImageInfo)
			{
				DeleteObject(pImageInfo);
			}
			delete pImageInfo;
			PostQuitMessage(0);
			break;
		}
	}

	return CWindowWnd::MessageHandler(hWnd, uMsg, wParam, lParam);
}
