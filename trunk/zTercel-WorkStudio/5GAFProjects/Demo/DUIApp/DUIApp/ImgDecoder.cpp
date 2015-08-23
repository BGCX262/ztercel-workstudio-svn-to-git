#include "ImgDecoder.h"

//------------------------------------------------------------------------------
//class CBMPDecoder
//------------------------------------------------------------------------------
bool CBMPDecoder::Decode(const BYTE* lpSrcData, DWORD dwSrcDataSize, BYTE*& pImgData, DWORD& dwImgDataSize)
{
	bool bResult = false;
	if (NULL != lpSrcData && 0 != dwSrcDataSize)
	{
		BITMAPFILEHEADER* pBfh = reinterpret_cast<BITMAPFILEHEADER*>((BYTE*)lpSrcData); 

		if (NULL != pBfh && 0x4D42 == pBfh->bfType)
		{
			dwImgDataSize = pBfh->bfSize - sizeof(BITMAPFILEHEADER);
			BITMAPINFO* pBmpInfo = reinterpret_cast<BITMAPINFO*>(pBfh + sizeof(BITMAPFILEHEADER));
			DWORD dwCompression = pBmpInfo->bmiHeader.biCompression; 
			if (dwCompression != BI_RLE4 && dwCompression != BI_RLE8)
			{
				pImgData = new BYTE[dwImgDataSize];
				memset(pImgData, 0, dwImgDataSize);
				memcpy(pImgData, lpSrcData + sizeof(BITMAPFILEHEADER), dwImgDataSize);
				bResult = true;
			}
		}
	}

	return bResult;
}