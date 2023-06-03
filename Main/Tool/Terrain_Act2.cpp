#include "stdafx.h"
#include "Terrain_Act2.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "MainFrm.h"

CTerrain_Act2::CTerrain_Act2()
{
}


CTerrain_Act2::~CTerrain_Act2()
{
	Release();
}

HRESULT CTerrain_Act2::Initialize()
{
	m_eType = OBJ_TYPE::TERRAIN;

	CMainFrame*		pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	m_pMainView = static_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/00.Tile/Tool/Tile_%d.png", TEX_MULTI, L"Terrain_Tool", L"Tile", 3)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/00.Tile/Act2/Tile%d.png", TEX_MULTI, L"Act2Terrain", L"Tile", 32)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

int CTerrain_Act2::Update()
{
	return 0;
}

void CTerrain_Act2::Late_Update()
{
}

void CTerrain_Act2::Render()
{
	if (m_vecAct2Tile.empty())
		return;

	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR		szBuf[MIN_STR] = L"";
	int			iIndex = 0;

	RECT	rc{};

	// GetClientRect : 현재 클라이언트 영역의 rect 정보를 얻어옴
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);


	for (auto iter : m_vecAct2Tile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x - m_pMainView->GetScrollPos(0), // 0일 경우 x 스크롤 값 얻어옴
			iter->vPos.y - m_pMainView->GetScrollPos(1), // 1일 경우 y 스크롤 값 얻어옴
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);

		const TEXINFO*	pTexInfo;
		if (iter->eType == TERRIAN_TYPE::TYPEEND)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain_Tool", L"Tile", iter->byDrawID);
		}
		else if (iter->eType == TERRIAN_TYPE::ACT1)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act2Terrain", L"Tile", iter->byDrawID);
		}

		float	fX = pTexInfo->tImgInfo.Width / 2.f;
		float	fY = pTexInfo->tImgInfo.Height / 2.f;

		// 이미지에 행렬을 반영
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,							// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0 기준으로 출력
			&D3DXVECTOR3(fX, fY, 0.f),			// 출력할 이미지의 중심축에 대한 vector3 주소, null인 경우 이미지의 0, 0이 중심 좌표
			nullptr,							// 위치 좌표에 대한 vector3 주소, null인 경우 스크린 상의 0, 0좌표 출력
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 원본 색상 유지


	/*	swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,
			lstrlen(szBuf),
			nullptr,
			DT_CENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		++iIndex;*/
	}
}

void CTerrain_Act2::Release()
{
	for_each(m_vecAct2Tile.begin(), m_vecAct2Tile.end(), CDeleteObj());
	m_vecAct2Tile.clear();
	m_vecAct2Tile.shrink_to_fit();
}

void CTerrain_Act2::Tool_Render(const D3DXVECTOR3 & _vWorld)
{
}

void CTerrain_Act2::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}

void CTerrain_Act2::Create_Terrian(int _TileX, int _TileY)
{
	for (int i = 0; i < _TileX; ++i)
	{
		for (int j = 0; j < _TileY; ++j)
		{
			TILE* pTile = new TILE;

			float	fX = (TILECX * j) + ((TILECX / 2.f) * (i % 2));
			float	fY = (TILECY / 2.f) * i;

			pTile->eType = TERRIAN_TYPE::TYPEEND;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 1.f };
			pTile->byOption = NONETILE;
			pTile->byDrawID = 0;

			m_vecAct2Tile.push_back(pTile);
		}
	}
}

int CTerrain_Act2::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecAct2Tile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

void CTerrain_Act2::Tile_Change(const D3DXVECTOR3 & vPos, TILE* _Tile)
{
	if (!_Tile)
		return;

	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecAct2Tile[iIndex]->eType = _Tile->eType;
	m_vecAct2Tile[iIndex]->byDrawID = _Tile->byDrawID;
	m_vecAct2Tile[iIndex]->byOption = _Tile->byOption;
	m_vecAct2Tile[iIndex]->fDamage = _Tile->fDamage;
}

bool CTerrain_Act2::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3		vPoint[4]{

		{ m_vecAct2Tile[iIndex]->vPos.x,m_vecAct2Tile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecAct2Tile[iIndex]->vPos.x + (TILECX / 2.f),m_vecAct2Tile[iIndex]->vPos.y, 0.f },
		{ m_vecAct2Tile[iIndex]->vPos.x,m_vecAct2Tile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecAct2Tile[iIndex]->vPos.x - (TILECX / 2.f),m_vecAct2Tile[iIndex]->vPos.y , 0.f }
	};

	D3DXVECTOR3			vDir[4]{

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3],
	};

	D3DXVECTOR3			vNormal[4]{
		{ -vDir[0].y, vDir[0].x, 0.f },
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f },
	};

	D3DXVECTOR3			vMouseDir[4]{

		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}
