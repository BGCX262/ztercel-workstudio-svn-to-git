#include "Texture.h"

STexture::STexture() 
		: m_pData(NULL)
		, m_dwDataSize(0)
{

}

STexture::STexture(SHORT nWidth, SHORT nHeight, BYTE nBitCount, LPVOID lpImgData)
{
	Create(nWidth, nHeight, nBitCount, lpImgData);
}

STexture::~STexture() 
{ 
	Init();
}

//==: 图像属性
SHORT STexture::GetWidth() const 
{
	SHORT nWidth = 0;

	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		nWidth = abs(pBmpInfo->bmiHeader.biWidth);
	}
	return nWidth;
} 

SHORT STexture::GetHeight() const
{
	SHORT nHeight = 0;

	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		nHeight = pBmpInfo->bmiHeader.biHeight;
	}
	return nHeight;
}

BYTE STexture::GetBitCount() const
{
	BYTE nBitCount = 0;

	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		nBitCount = pBmpInfo->bmiHeader.biBitCount;
	}
	return nBitCount;
}

SHORT STexture::GetStride() const
{
	SHORT nStride = 0;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		nStride = (((GetWidth() * GetBitCount() + 31) >> 5) << 2);
	}

	return nStride;
}

bool STexture::GetDirection() const
{
	bool bTopToDown = false;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	

		bTopToDown = pBmpInfo->bmiHeader.biWidth;
	}

	return bTopToDown;
}

BYTE STexture::GetCompress() const
{
	BYTE nCompression = 0;

	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		nCompression = pBmpInfo->bmiHeader.biCompression;
	}
	return nCompression;
}

DWORD STexture::GetMask(DWORD (&dwMask)[3], int nRGB) const
{	
	DWORD dwMaskRet	= 0;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		DWORD dwMaskSize	 = (16 == GetBitCount()) ? 3 * sizeof(RGBQUAD) : (32 == GetBitCount()) ? 4 * sizeof(RGBQUAD) : 0;
		memcpy(dwMask, pBmpInfo->bmiColors, dwMaskSize);
		switch (nRGB)
		{
			case 1:		dwMaskRet = dwMask[0];		break;	// R
			case 2:		dwMaskRet = dwMask[1];		break;	// G
			case 3:		dwMaskRet = dwMask[2];		break;	// B
		}
	}
	return dwMaskRet;
}

SHORT STexture::GetPaletteSize() const
{
	SHORT nPaletteSize = 0;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		if (pBmpInfo->bmiHeader.biBitCount <= 8)
		{
			nPaletteSize = pBmpInfo->bmiHeader.biClrUsed;
			if (0 == pBmpInfo->bmiHeader.biClrUsed)
			{
				nPaletteSize = (1 << pBmpInfo->bmiHeader.biBitCount);
			}
		}
	}

	return nPaletteSize;
}

const BYTE*	STexture::GetPalette() const
{
	BYTE* pPalette = NULL;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO)) 
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		if (pBmpInfo->bmiHeader.biBitCount <= 8)
		{
			pPalette = (m_pData + sizeof(BITMAPINFOHEADER));
		}
	}

	return pPalette;
}

DWORD STexture::GetImageSize() const
{
	DWORD dwImgSize = 0;
	BYTE* pImage = NULL;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO)) 
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		BYTE nBitCount = pBmpInfo->bmiHeader.biBitCount;
		if (nBitCount <= 8)
		{
			dwImgSize = m_dwDataSize - sizeof(BITMAPINFOHEADER) - (sizeof(RGBQUAD) * (1 << nBitCount));
		} else {
			if (BI_BITFIELDS == GetCompress())
			{
				dwImgSize = m_dwDataSize - sizeof(BITMAPINFOHEADER) - sizeof(RGBQUAD) * 4;
			} else {
				dwImgSize = m_dwDataSize - sizeof(BITMAPINFOHEADER);
			}
		}
	}

	return dwImgSize;
}

BYTE*	STexture::GetImage() const
{
	BYTE* pImage = NULL;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO)) 
	{
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(m_pData);	
		BYTE nBitCount = pBmpInfo->bmiHeader.biBitCount;
		if (nBitCount <= 8)
		{
			pImage = (m_pData + sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * (1 << nBitCount)));
		} else {
			if (BI_BITFIELDS == GetCompress())
			{
				pImage = (m_pData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 4);
			} else {
				pImage = (m_pData + sizeof(BITMAPINFOHEADER));
			}
		}
	}
	return pImage;
}

const void*	STexture::GetBmpInfo() const
{
	BYTE* pBmpInfo = NULL;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO)) 
	{
		pBmpInfo = m_pData ;
	}

	return pBmpInfo;
}

STexture::operator HBITMAP()
{
	HBITMAP hBitmap	= NULL;
	if (NULL != m_pData && m_dwDataSize > sizeof(BITMAPINFO))
	{
		LPVOID lpDest = NULL;
		HDC hDC = GetDC(NULL);
		BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>((void*)GetBmpInfo());
		hBitmap = CreateDIBSection(hDC, pBmpInfo, DIB_RGB_COLORS, &lpDest, NULL, 0);
		if (NULL != hBitmap)
		{
			memcpy(lpDest, GetImage(), GetImageSize());
		}
		ReleaseDC(NULL, hDC);

	}
	return hBitmap;
}

void STexture::Init()
{
	if (NULL != m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}

	m_dwDataSize	 = 0;
}

bool STexture::Create(SHORT nWidth, SHORT nHeight, BYTE nBitCount, LPVOID lpImgData)
{
	bool bResult = false;
	if (0 != nWidth && 0 != nHeight && 0 != nBitCount /*&& NULL != lpImgData*/)
	{
		Init();

		//==: 计算数据大小(信息头 + 图像数据), 并申请空间
		DWORD dwBISize	= sizeof(BITMAPINFOHEADER);
		dwBISize += (16 == nBitCount) ? 3 * sizeof(RGBQUAD) : 0;
		//dwBISize += (32 == nBitCount) ? 1 * sizeof(RGBQUAD) : 0;	
		DWORD dwImgSize = nHeight * (((nWidth * nBitCount + 31) >> 5) << 2);
		m_dwDataSize = dwBISize + dwImgSize;
		m_pData = new BYTE[m_dwDataSize];
		assert(m_pData);
		memset(m_pData, 0x0, m_dwDataSize);

		//==: 填充信息头
		BITMAPINFO* pBmpInfo = (BITMAPINFO*)m_pData;
		pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pBmpInfo->bmiHeader.biPlanes		= 1;
		pBmpInfo->bmiHeader.biWidth			= nWidth;
		pBmpInfo->bmiHeader.biHeight		= nHeight;
		pBmpInfo->bmiHeader.biBitCount		= nBitCount;
		pBmpInfo->bmiHeader.biCompression	= (nBitCount == 16 /*|| nBitCount == 32*/) ? BI_BITFIELDS : BI_RGB;
		pBmpInfo->bmiHeader.biSizeImage		= m_dwDataSize;

		switch (nBitCount)
		{
			case 16:
			{
				DWORD dwBitMask[] = {0xF800, 0x07E0, 0x001F};				
				memcpy(m_pData + sizeof(BITMAPINFOHEADER), dwBitMask, sizeof(dwBitMask));
				break;
			}
// 			case 32:
// 			{
// 				DWORD dwBitMask[] = {0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};			
// 				memcpy(m_pData + sizeof(BITMAPINFOHEADER), dwBitMask, sizeof(dwBitMask));
// 				break;
// 			}
		}

		//==: 填充图像数据
		if (NULL != lpImgData)
		{
			memcpy(m_pData + dwBISize, lpImgData, dwImgSize);
		} else {
			memset(m_pData + dwBISize, 0x0, dwImgSize);
		}

		bResult = true;
	}

	return bResult;
}

void STexture::CopyImage(LPVOID lpImgData, DWORD dwDataSize)
{
	if (dwDataSize < m_dwDataSize && NULL != lpImgData)
	{
		memcpy(m_pData + (m_dwDataSize - GetImageSize()), lpImgData, dwDataSize);
	}

	return;
}