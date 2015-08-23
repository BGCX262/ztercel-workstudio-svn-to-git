#include "SysHeader.h"
#include "Texture.h"

std::map<_tstring, const STexture*>	CTexManager::mpTexList;
const STexture*	CTexManager::Find(const _tstring& szName)
{
	std::map<_tstring, const STexture*>::iterator itr = mpTexList.find(szName);
	if (mpTexList.end() == itr)
	{
		BYTE*		lpSrcData		= NULL;
		DWORD		dwSrcDataSize	= 0;
		STexture*	pTexture		= new STexture;
		UINT		nTexType		= 0;


		HANDLE hFile = ::CreateFile(szName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (hFile != INVALID_HANDLE_VALUE ) 
		{
			DWORD dwSize = ::GetFileSize(hFile, NULL);
			if (0 != dwSize)
			{
				lpSrcData = new BYTE[dwSize];
				::ReadFile(hFile, lpSrcData, dwSize, &dwSrcDataSize, NULL);
				::CloseHandle(hFile);

				CTexLoader	TexLoader;
				TexLoader.Load(lpSrcData, dwSrcDataSize, *pTexture, nTexType);
				Add(szName, pTexture);

				delete[] lpSrcData;
			}
		}
		
		return pTexture;
	} 

	return itr->second;
}

void CTexManager::Add(const _tstring& szName, const STexture* pTexture)
{
	if (NULL != pTexture)
	{
		mpTexList[szName] = pTexture;
	}
	
	return;
}
	
void CTexManager::Remove(const _tstring& szName)
{
	std::map<_tstring, const STexture*>::iterator itr = mpTexList.find(szName);
	if (mpTexList.end() != itr)
	{
		STexture* pTexture = reinterpret_cast<STexture*>((void*)itr->second);
		if (NULL != pTexture)
		{
			delete pTexture;
		}
		mpTexList.erase(itr);
	}

	return;
}

void CTexManager::RemoveAll()
{
	if (!mpTexList.empty())
	{
		std::map<_tstring, const STexture*>::const_iterator itr = mpTexList.begin();
		while (mpTexList.end() != itr++)
		{
			STexture* pTexture = reinterpret_cast<STexture*>((void*)itr->second);
			if (NULL != pTexture)
			{
				delete pTexture;
			}
		}

		mpTexList.clear();
	}

	return;
}
