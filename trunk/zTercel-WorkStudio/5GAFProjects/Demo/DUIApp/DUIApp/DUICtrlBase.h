#pragma once
#include "SysHeader.h"
#include "Texture.h"

class CDUIRender;

class IMessageHandler
{
public:
	virtual void	MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam) = 0;
};

class CDUICtrlBase : IMessageHandler
{
public:
	CDUICtrlBase(void);
	~CDUICtrlBase(void);

public:
	virtual void	SetPos(UINT nCtrlX, UINT nCtrlY);
	virtual void	SetSize(UINT nCtrlW, UINT nCtrlH);
	
	virtual void	Draw(CDUIRender* pDUIRender) = 0;
	virtual void	MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam) = 0;

protected:
	UINT		m_nCtrlX;
	UINT		m_nCtrlY;
	UINT		m_nCtrlW;
	UINT		m_nCtrlH;
};


class CDUICanvas : public CDUICtrlBase
{
public:
 	void Draw(CDUIRender* pDUIRender);
	
	void	SetSkin(TCHAR* szSkinCanvas);
	

	void	MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam);
	
protected:
	TCHAR	m_szSkinCanvas[MAX_PATH];
};


class CDUIButton : public CDUICtrlBase
{
public:
	void	SetSkin(TCHAR* szSkinNor, TCHAR* szSkinSel, TCHAR* szSkinFocus);
	
	void	SetSkinAlpha(const TCHAR* szSkinAlpha);
	
	void	Draw(CDUIRender* pDUIRender);

	void	MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam);
	
protected:
	TCHAR	m_szSkinNor[MAX_PATH];
	TCHAR	m_szSkinSel[MAX_PATH];
	TCHAR	m_szSkinFocus[MAX_PATH];
	TCHAR	m_szSkinAlpha[MAX_PATH];

	UINT	m_nState;
};

class CDUIBuilder
{


};

class CDUIRender
{
public:
	CDUIRender()
	{

	}
	~CDUIRender()
	{
		Uninit();
	}

	void	Init(HWND hWnd)
	{
		RECT rtClient	= {0};
		GetWindowRect(hWnd, &rtClient);
		int cx			= rtClient.right - rtClient.left;
		int cy			= rtClient.bottom - rtClient.top;

		m_hWndPaint		= hWnd;
		m_hDCDevice		= GetDC(hWnd);
		m_hDCOffSrcn	= CreateCompatibleDC(m_hDCDevice);
		m_hBmpOffSrcn	= CreateCompatibleBitmap(m_hDCDevice, cx, cy);
		SelectObject(m_hDCOffSrcn, m_hBmpOffSrcn);

		UINT nBitCount	= GetDeviceCaps(m_hDCDevice, BITSPIXEL);

		m_RenderBuffer.Create(cx, cy, nBitCount, NULL);
	}

	void	DrawUI(const vector<CDUICtrlBase*>& DUICtrlList)
	{
		STexture* pTexture = (STexture*)CTexManager::Find(_T("E:\\5GAFProjects\\Demo\\DUIApp\\DUIApp\\Res\\Images\\BackGround.bmp"));

		for (int ii = 0; ii < DUICtrlList.size(); ii++)
		{
			DUICtrlList[ii]->Draw(this);
		}

		//INT nBitCount	= GetDeviceCaps(m_hDCDevice, BITSPIXEL);
	
		//m_RenderBuffer.Create(800, 480, 24, (LPVOID)pTexture->GetImage());
		SHORT cx = m_RenderBuffer.GetWidth();
		SHORT cy = m_RenderBuffer.GetHeight();

		

		HDC hDCMem		= CreateCompatibleDC(m_hDCDevice);
		HBITMAP hOldBmp	= (HBITMAP)SelectObject(hDCMem, (HGDIOBJ)(HBITMAP)m_RenderBuffer);
		BitBlt(m_hDCDevice, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);

		SelectObject(hDCMem, hOldBmp);
		DeleteObject(hDCMem);





// 		SHORT cx = m_RenderBuffer.GetWidth();
// 		SHORT cy = m_RenderBuffer.GetHeight();
// 		
//  		HDC hDCMem		= CreateCompatibleDC(m_hDCDevice);
//  		//HBITMAP hOldBmp	= (HBITMAP)SelectObject(hDCMem, m_RenderBuffer);
//  
//  		//BitBlt(m_hDCOffSrcn, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
// 
// 		HBITMAP hOldBmp	= (HBITMAP)SelectObject(m_hDCOffSrcn, m_RenderBuffer);
// 
// 		BitBlt(m_hDCDevice, 0, 0, cx, cy, m_hDCOffSrcn, 0, 0, SRCCOPY);
// 		
// 		SelectObject(hDCMem, hOldBmp);
// 		DeleteObject(hDCMem);

		return;
	}

	void	DrawBitmap(int x, int y, const TCHAR* szBmpName)
	{
		const STexture* pBmp = CTexManager::Find(szBmpName);
		
		switch (m_RenderBuffer.GetBitCount())
		{
			case 16:
			{
				switch (pBmp->GetBitCount())
				{
					case 16:		break;
					case 24:		break;
					case 32:		break;
				}
			}
			case 24:
			{
				switch (pBmp->GetBitCount())
				{
					case 16:		break;
					case 24:		
					{
						BYTE* pRenderBuffer = reinterpret_cast<BYTE*>((void*)m_RenderBuffer.GetImage());
						BYTE* pBmpBuffer	= reinterpret_cast<BYTE*>((void*)pBmp->GetImage());

						RECT rtDest			= {0};
						RECT rtRenderBuffer	= {0, 0, m_RenderBuffer.GetWidth(), m_RenderBuffer.GetHeight()};
						RECT rtBmp			= {x, y, x + pBmp->GetWidth(), y + pBmp->GetHeight()};

						if (0 != IntersectRect(&rtDest, &rtRenderBuffer, &rtBmp))
						{

						}

						UINT	nColPix1	= 0;
						UINT	nColPix2	= 0;
						DWORD	dwMask[3]	= {0};
						for (int ii = 0; ii < m_RenderBuffer.GetHeight(); ii++)
						{
							for (int jj = 0; jj < m_RenderBuffer.GetWidth(); jj++)
							{
								nColPix1 = jj * 3;
								nColPix2 = jj * 3;
								pRenderBuffer[nColPix1 + 0] = (BYTE)(pBmpBuffer[nColPix2 + 0]);
								pRenderBuffer[nColPix1 + 1] = (BYTE)(pBmpBuffer[nColPix2 + 1]);
								pRenderBuffer[nColPix1 + 2] = (BYTE)(pBmpBuffer[nColPix2 + 2]);								
							}
							pRenderBuffer += m_RenderBuffer.GetStride();
							pBmpBuffer	  += pBmp->GetStride();
						}
						break;					
					}
					case 32:		break;
				}
			}
			case 32:
			{
				switch (pBmp->GetBitCount())
				{
					case 16:		
					{
						BYTE* pRenderBuffer = reinterpret_cast<BYTE*>((void*)m_RenderBuffer.GetImage());
						BYTE* pBmpBuffer	= reinterpret_cast<BYTE*>((void*)pBmp->GetImage());
			
						RECT rtDest			= {0};
						RECT rtRenderBuffer	= {0, 0, m_RenderBuffer.GetWidth(), m_RenderBuffer.GetHeight()};
						RECT rtBmp			= {x, y, x + pBmp->GetWidth(), y + pBmp->GetHeight()};

						if (0 != IntersectRect(&rtDest, &rtRenderBuffer, &rtBmp))
						{
														
						}

						DWORD dRMask1 = 0xF800;
						DWORD dGMask1 = 0x07E0;
						DWORD dBMask1 = 0x001F;

						UINT	nColPix1	= 0;
						UINT	nColPix2	= 0;
						DWORD dwMask[3];
						for (int ii = 0; ii < m_RenderBuffer.GetHeight(); ii++)
						{
							for (int jj = 0; jj < m_RenderBuffer.GetWidth(); jj++)
							{
								nColPix1 = jj * 4;
								nColPix2 = jj * 2;

//  								DWORD dRMask1 = pBmp->GetMask(dwMask, 1);
//  								DWORD dGMask1 = pBmp->GetMask(dwMask, 2);
//  								DWORD dBMask1 = pBmp->GetMask(dwMask, 3);
							
 								pRenderBuffer[nColPix1 + 0] = (BYTE)(((WORD)((WORD*)pBmpBuffer)[nColPix2] & dRMask1) >> 8);
 								pRenderBuffer[nColPix1 + 1] = (BYTE)(((WORD)((WORD*)pBmpBuffer)[nColPix2] & dGMask1) >> 3);
 								pRenderBuffer[nColPix1 + 2] = (BYTE)(((WORD)((WORD*)pBmpBuffer)[nColPix2] & dBMask1) << 3);
								
// 								pRenderBuffer[nColPix1 + 2] = (BYTE)((((WORD)pBmpBuffer[nColPix2]) & dRMask1) >> 8);
// 								pRenderBuffer[nColPix1 + 1] = (BYTE)((((WORD)pBmpBuffer[nColPix2]) & dGMask1) >> 3);
// 								pRenderBuffer[nColPix1 + 0] = (BYTE)((((WORD)pBmpBuffer[nColPix2]) & dBMask1) << 3);

								pRenderBuffer[nColPix1 + 3] = 0xFF;								
							}
							pRenderBuffer += m_RenderBuffer.GetStride();
							pBmpBuffer	  += pBmp->GetStride();
						}

						break;
					}
					case 24:		
					{
						BYTE* pRenderBuffer = reinterpret_cast<BYTE*>((void*)m_RenderBuffer.GetImage());
						BYTE* pBmpBuffer	= reinterpret_cast<BYTE*>((void*)pBmp->GetImage());

						RECT rtDest			= {0};
						RECT rtRenderBuffer	= {0, 0, m_RenderBuffer.GetWidth(), m_RenderBuffer.GetHeight()};
						RECT rtBmp			= {x, y, x + pBmp->GetWidth(), y + pBmp->GetHeight()};

						if (0 != IntersectRect(&rtDest, &rtRenderBuffer, &rtBmp))
						{

						}

						UINT	nColPix1	= 0;
						UINT	nColPix2	= 0;
						DWORD	dwMask[3]	= {0};
						for (int ii = 0; ii < m_RenderBuffer.GetHeight(); ii++)
						{
							for (int jj = 0; jj < m_RenderBuffer.GetWidth(); jj++)
							{
								nColPix1 = jj * 4;
								nColPix2 = jj * 3;
								pRenderBuffer[nColPix1 + 0] = (BYTE)(pBmpBuffer[nColPix2 + 0]);
								pRenderBuffer[nColPix1 + 1] = (BYTE)(pBmpBuffer[nColPix2 + 1]);
								pRenderBuffer[nColPix1 + 2] = (BYTE)(pBmpBuffer[nColPix2 + 2]);		
								pRenderBuffer[nColPix1 + 3] = 0xFF;		
							}
							pRenderBuffer += m_RenderBuffer.GetStride();
							pBmpBuffer	  += pBmp->GetStride();
						}
						break;	
					}
					case 32:		
					{
						

						break;
					}
				}
			}
		}

		return;
	}

	void	DrawBitmap(int x, int y, const TCHAR* szBmpName, const TCHAR* szBmpAlpName)
	{

	}

private:
	void	Uninit()
	{
		DeleteObject(m_hBmpOffSrcn);
		DeleteObject(m_hDCOffSrcn);
		ReleaseDC(m_hWndPaint, m_hDCDevice);

		return;
	}

protected:
	HWND		m_hWndPaint;
	HDC			m_hDCOffSrcn;
	HDC			m_hDCDevice;
	HBITMAP		m_hBmpOffSrcn;

	STexture	m_RenderBuffer;
};
