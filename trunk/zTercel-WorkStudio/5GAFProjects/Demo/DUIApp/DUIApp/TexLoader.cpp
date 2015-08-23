#pragma once
#include "Texture.h"
#include "ImgDecoder.h"

bool CTexLoader::Load(BYTE* szSrcData, DWORD dwSrcDataSize, STexture& Texture, UINT nTexType)
{
	bool bResult = false;
	if (NULL != szSrcData && NULL != dwSrcDataSize)
	{
		IImgDecoder* pImgDecoder = NULL;
		switch (nTexType)
		{
		case TEXTYPE_BMP:	pImgDecoder	= new CBMPDecoder();		break;
		case TEXTYPE_JPG:	/*pImgDecoder	= new CJPGDecoder;*/ 	break;
		case TEXTYPE_PNG:	/*pImgDecoder	= new CPNGDecoder;*/ 	break;
		case TEXTYPE_GIF:	/*pImgDecoder	= new CGIFDecoder;*/ 	break;
		case TEXTYPE_ICO:	/*pImgDecoder	= new CICODecoder;*/ 	break;
		case TEXTYPE_TGA:	/*pImgDecoder	= new CTGADecoder;*/ 	break;
		}
		if (NULL != pImgDecoder)
		{
			bResult = pImgDecoder->Decode(szSrcData, dwSrcDataSize, Texture.m_pData, Texture.m_dwDataSize);

			delete pImgDecoder;
			pImgDecoder = NULL;
		}
	}
	return bResult;
}
