#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	// CTexture을(를) 통해 상속됨
	virtual const TEXINFO * Get_Texture(const TCHAR * pStateKey = L"", const int & iCount = 0) override
	{
		return m_pTexInfo;
	}

public:
	virtual HRESULT Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey = L"", const int & iCount = 0) override;
	virtual void Release(void) override;

private:
	TEXINFO*			m_pTexInfo;
};

