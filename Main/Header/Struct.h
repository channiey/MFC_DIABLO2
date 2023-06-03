#pragma once
#include "Enum.h"

#pragma region HEEJUNE


#pragma endregion


#pragma region CHAN


typedef	struct tagUnitData
{
#ifndef _AFX
	wstring		strName;
#else
	CString	strName;
#endif
	int		iAttack;
	int		iHp;

	BYTE	byJobIndex;
	BYTE	byItem;

}UNITDATA;


typedef struct tagAnimation
{
	float	fSecondPerFrame;
	bool	bLoop;
	int		iMaxFrame;
	int		iCurFrame;

}ANIMATION;
#pragma endregion




typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vRot; 
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;
}INFO;

typedef struct tagStat
{
	int	iHP;
	int	iAD;
	int iSpeed;

}STAT;

typedef	struct tagFrame
{
	float		fFrame; // 프레임을 세기 위한 변수
	float		fMax;	// 최대 이미지의 프레임 수
}FRAME;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // 컴 객체

	D3DXIMAGE_INFO			tImgInfo;	// 구조체

}TEXINFO;

typedef struct tagTile
{
	D3DXVECTOR3	vPos;
	D3DXVECTOR3 vSize;

	TERRIAN_TYPE eType;

	BYTE		byOption;
	float		fDamage;

	UINT		byDrawID;

	bool		bCheckUnit;
	bool		bCheckTile;

	int			iIndex;
	int			iParentIdx;

}TILE;



typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath = L"";
	int			iCount = 0;

}IMGPATH;


static D3DXVECTOR3		Get_Mouse()
{
	POINT	Pt{};

	GetCursorPos(&Pt);
	ScreenToClient(g_hWnd, &Pt);

	return D3DXVECTOR3((float)Pt.x, (float)Pt.y, 0.f);
}