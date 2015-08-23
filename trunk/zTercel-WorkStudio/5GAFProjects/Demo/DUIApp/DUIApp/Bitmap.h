#pragma once
#include <Windows.h>
#include <tchar.h>

class CBitmap// : public CImgDecoder
{
public:
	CBitmap();
	virtual ~CBitmap();

	//==: Bmp ����
public:
	bool			Load(const TCHAR* szFilePath);
	bool			Decode();

	//==: Bmp ����
public:
	int				GetWidth() const;
	int				GetHeight() const;
	int				GetBitCount() const;

	int				GetCompress() const;
	bool			GetBitMask(DWORD (&dwMask)[4]) const;
	int				GetStride() const;

	const void*		GetBitData() const;
	int				GetBitDataSize() const;

	const void*		GetBMPInfo() const;
	int				GetBmpInfoSize() const;

	const void*		GetPalette() const;
	int				GetPaletteSize() const;
protected:
	void			Initialize();

	//==: Bmp ����
protected:
	BITMAPFILEHEADER	m_FileHeader;
	BITMAPINFO*			m_pBmpInfo;
	LPBYTE				m_pDIB;

	LPBYTE				m_pBmpData;
};