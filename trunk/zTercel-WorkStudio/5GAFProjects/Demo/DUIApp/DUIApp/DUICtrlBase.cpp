#include "DUICtrlBase.h"

CDUICtrlBase::CDUICtrlBase(void)
{
}

CDUICtrlBase::~CDUICtrlBase(void)
{

}

void CDUICtrlBase::SetPos(UINT nCtrlX, UINT nCtrlY)
{
	m_nCtrlX	= nCtrlX;
	m_nCtrlY	= nCtrlY;
}

void CDUICtrlBase::SetSize(UINT nCtrlW, UINT nCtrlH)
{
	m_nCtrlW	= nCtrlW;
	m_nCtrlH	= nCtrlH;
}


//////////////////////////////////////////////////////////////////////////
// CDUICanvas
//////////////////////////////////////////////////////////////////////////
void CDUICanvas::Draw(CDUIRender* pDUIRender)
{
	pDUIRender->DrawBitmap(m_nCtrlX, m_nCtrlY, m_szSkinCanvas);
	return;
}

void CDUICanvas::SetSkin(TCHAR* szSkinCanvas)
{
	if (NULL != szSkinCanvas && 0 != *szSkinCanvas)
	{
		_tcscpy(m_szSkinCanvas, szSkinCanvas);
	}
}

void CDUICanvas::MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam) 
{
	return;
}

//////////////////////////////////////////////////////////////////////////
// CDUIButton
//////////////////////////////////////////////////////////////////////////
void CDUIButton::SetSkin(TCHAR* szSkinNor, TCHAR* szSkinSel, TCHAR* szSkinFocus)
{
	if (NULL != szSkinNor && 0 != *szSkinNor)
	{
		_tcscpy(m_szSkinNor, szSkinNor);
	}

	if (NULL != szSkinSel && 0 != *szSkinSel)
	{
		_tcscpy(m_szSkinSel, szSkinSel);
	}

	if (NULL != szSkinNor && 0 != *szSkinFocus)
	{
		_tcscpy(m_szSkinFocus, szSkinFocus);
	}

	return;
}

void CDUIButton::SetSkinAlpha(const TCHAR* szSkinAlpha)
{
	if (NULL != szSkinAlpha && 0 != *szSkinAlpha)
	{
		_tcscpy(m_szSkinAlpha, szSkinAlpha);
	}
}

void CDUIButton::Draw(CDUIRender* pDUIRender)
{
	enum { STATE_CTRL_NOR, STATE_CTRL_SEL, STATE_CTRL_FOCUS};

	TCHAR* szSkinName	= NULL;
	switch (m_nState)
	{
		case STATE_CTRL_NOR:		
		{
			szSkinName = m_szSkinFocus;	
			break;
		}
		case STATE_CTRL_SEL:
		{
			szSkinName = m_szSkinFocus;
			break;
		}
		case STATE_CTRL_FOCUS:
		{
			szSkinName = m_szSkinFocus;
			break;
		}
	}
	pDUIRender->DrawBitmap(m_nCtrlX, m_nCtrlY, szSkinName, m_szSkinAlpha);

	return;
}

void CDUIButton::MessageHandler(UINT nMessage, WPARAM wParam, LPARAM lParam) 
{

	return;
}
// 
// class CDUIRender
// {
// public:
// 	CDUIRender()
// 	{
// 
// 	}
// 	~CDUIRender()
// 	{
// 		Uninit();
// 	}
// 
// 	void	Init(HWND hWnd)
// 	{
// 		m_hWndPaint		= hWnd;
// 		m_hDCOffSrcn	= GetDC(hWnd);
// 	}
// 
// 	void	DrawUI(const vector<CDUICtrlBase*>& DUICtrlList)
// 	{
// 		for (int ii = 0; ii < DUICtrlList.size(); ii++)
// 		{
// 			DUICtrlList[ii]->Draw(this);
// 		}
// 
// 	}
// 
// 	void	DrawBitmap(int x, int y, const TCHAR* szBmpName)
// 	{
// 
// 	}
// 
// 	void	DrawBitmap(int x, int y, const TCHAR* szBmpName, const TCHAR* szBmpAlpName)
// 	{
// 
// 	}
// 
// private:
// 	void	Uninit()
// 	{
// 		ReleaseDC(m_hWndPaint, m_hDCOffSrcn);
// 	}
// 
// protected:
// 	HWND	m_hWndPaint;
// 	HDC		m_hDCOffSrcn;
// };
