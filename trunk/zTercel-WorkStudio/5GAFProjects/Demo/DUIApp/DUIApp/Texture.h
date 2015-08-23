#pragma once
#include "SysHeader.h"


enum eTexType {
	TEXTYPE_BMP, TEXTYPE_JPG, TEXTYPE_PNG, TEXTYPE_GIF, TEXTYPE_ICO, TEXTYPE_TGA 
};

///////////////////////////////////////////////////////////////////////////////
// class STexture
///////////////////////////////////////////////////////////////////////////////
struct STexture 
{
public:
	STexture();
	STexture(SHORT nWidth, SHORT nHeight, BYTE nBitCount, LPVOID lpImgData);

	~STexture();

public:
	//==: ÕºœÒ Ù–‘
	SHORT		GetWidth() const;
	SHORT		GetHeight() const;
	BYTE		GetBitCount() const;
	SHORT		GetStride() const;
	bool		GetDirection() const;
	BYTE		GetCompress() const;
	DWORD		GetMask(DWORD (&dwMask)[3], int nRGB = 0) const;
	SHORT		GetPaletteSize() const;
	const BYTE*	GetPalette() const;
	DWORD		GetImageSize() const;

	BYTE*		GetImage() const;
	const void*	GetBmpInfo() const;
	operator HBITMAP();
	
	bool		Create(SHORT nWidth, SHORT nHeight, BYTE nBitCount, LPVOID lpData);
	void		CopyImage(LPVOID lpData, DWORD dwDataSize);
private:
	BYTE*		m_pData;
	DWORD		m_dwDataSize;

	friend class CTexLoader;

protected:
	void	Init();
};


///////////////////////////////////////////////////////////////////////////////
// class CTexLoader
///////////////////////////////////////////////////////////////////////////////
class CTexLoader
{
public:
	bool Load(BYTE* szSrcData, DWORD dwSrcDataSize, STexture& Texture, UINT nTexType);
};

///////////////////////////////////////////////////////////////////////////////
// class CTexManager
///////////////////////////////////////////////////////////////////////////////
class CTexManager
{
public: 
	static const STexture*	Find(const _tstring& szName);
	static void				Add(const _tstring& szName, const STexture* pTexture);
	static void				Remove(const _tstring& szName);
	static void				RemoveAll();

private:
	static std::map<_tstring, const STexture*>	mpTexList;
};
