#include "Bitmap.h"

CBitmap::CBitmap()	
	: m_pBmpData(NULL)
	, m_pBmpInfo(NULL)
	, m_pDIB(NULL)
{
	
}

CBitmap::~CBitmap()
{
	Initialize();
}

bool CBitmap::Load(const TCHAR* szFilePath)
{
	bool bLoadSuc = false;
	if (NULL != szFilePath && -1 != GetFileAttributes(szFilePath))
	{		
		Initialize();

		HANDLE hBmpFile = CreateFile(szFilePath, 
									 GENERIC_READ|GENERIC_WRITE, 
									 FILE_SHARE_READ|FILE_SHARE_WRITE, 
									 NULL, 
									 OPEN_EXISTING,
									 FILE_ATTRIBUTE_NORMAL,
									 NULL);
		if (INVALID_HANDLE_VALUE != hBmpFile)
		{
			DWORD dwRead = 0;
			DWORD dwSize = GetFileSize(hBmpFile, NULL);
		
			m_pBmpData	= new BYTE[dwSize];
			if (TRUE == ReadFile(hBmpFile, m_pBmpData, dwSize, &dwRead, NULL))
			{
				bLoadSuc = true;
				CloseHandle(hBmpFile);
			} 
		}
	}
	return bLoadSuc;
}

bool CBitmap::Decode()
{
	bool bDecodeSec	= false;
	if (NULL != m_pBmpData)
	{
		WORD dwType = 0;
		memcpy((void*)&dwType, m_pBmpData, sizeof(WORD));
		if (dwType == 0X4D42)
		{
			//==: File Header
			memcpy((void*)&m_FileHeader, m_pBmpData, sizeof(BITMAPFILEHEADER));

			//==: Bitmap Infomation (contain: BMPINFO, PALETTE or MASK)
			DWORD dwBmpInfoSize = m_FileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
			m_pBmpInfo			= (BITMAPINFO*)new UCHAR[dwBmpInfoSize]; 
			memcpy(m_pBmpInfo, m_pBmpData + sizeof(BITMAPFILEHEADER), dwBmpInfoSize);

			//==: Bitmap bit array
			m_pDIB	= m_pBmpData + m_FileHeader.bfOffBits;

			bDecodeSec = true;
		}
	}

	return bDecodeSec;
}

int CBitmap::GetWidth() const
{
	return (NULL != m_pBmpInfo) ? m_pBmpInfo->bmiHeader.biWidth : 0;
}

int CBitmap::GetHeight() const
{
	return (NULL != m_pBmpInfo) ? m_pBmpInfo->bmiHeader.biHeight : 0;
}

int CBitmap::GetBitCount() const
{
	return (NULL != m_pBmpInfo) ? m_pBmpInfo->bmiHeader.biBitCount : 0;
}

int CBitmap::GetCompress() const
{
	return m_pBmpInfo->bmiHeader.biCompression;
}

bool CBitmap::GetBitMask(DWORD (&dwMask)[4]) const
{
	bool bMaskExist = false;
	if (NULL != m_pBmpInfo && BI_BITFIELDS == m_pBmpInfo->bmiHeader.biCompression)
	{
		memcpy(dwMask, m_pBmpInfo + sizeof(BITMAPINFOHEADER), 4 * sizeof(DWORD));
		bMaskExist	= true;
	}
	return bMaskExist;
}

int CBitmap::GetStride() const
{
	return ((GetWidth() * GetBitCount() / 8 + 31) / 32 * 4);
}

const void* CBitmap::GetBitData() const
{
	return m_pDIB;
}

int CBitmap::GetBitDataSize() const
{
	return (m_FileHeader.bfSize - m_FileHeader.bfOffBits);
}

int CBitmap::GetBmpInfoSize() const
{
	return (sizeof(BITMAPINFOHEADER) + GetPaletteSize());
}

const void* CBitmap::GetPalette() const  
{
	return (NULL != m_pBmpInfo && GetBitCount() <= 8) ? m_pBmpInfo + sizeof(BITMAPINFOHEADER) : NULL;
}

int CBitmap::GetPaletteSize() const
{
	int nPaletteSize = 0;
	if (NULL != m_pBmpInfo && m_pBmpInfo->bmiHeader.biBitCount <= 8)
	{
	 	nPaletteSize = m_pBmpInfo->bmiHeader.biClrUsed;
	 	if(nPaletteSize == 0)
 		{
 			nPaletteSize = (1 << m_pBmpInfo->bmiHeader.biBitCount) * sizeof(RGBQUAD);
 		}
 	}
	return nPaletteSize;
}

const void* CBitmap::GetBMPInfo() const
{
	return (LPVOID)m_pBmpInfo;
}

void CBitmap::Initialize()
{
	if (NULL != m_pBmpInfo)
	{
		delete[] (UCHAR*)m_pBmpInfo;
		m_pBmpInfo	= NULL;
	}

	if (NULL != m_pBmpData)
	{
		delete[] (UCHAR*)m_pBmpData;
		m_pBmpData	= NULL;
	}

	memset(&m_FileHeader, 0, sizeof(m_FileHeader));
	m_pBmpInfo		= NULL;	
	m_pBmpData		= NULL;
	m_pDIB			= NULL;
}
// End --: CBitmap ∂®“Â
//=================================================================================================
