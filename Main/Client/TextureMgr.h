#pragma once

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO*		Get_Texture(const TCHAR* pObjKey,
									const TCHAR* pStateKey = L"",
									const int& iCount = 0);

	HRESULT		Insert_Texture(const TCHAR* pFilePath,
								TEXTYPE eType,
								const TCHAR* pObjKey,
								const TCHAR* pStateKey = L"",
								const int& iCount = 0);

	HRESULT		Read_ImgPath(const wstring& wstrPath);


	 void		Release(void);

private:
	map<wstring, CTexture*>		m_mapTexture;
};

