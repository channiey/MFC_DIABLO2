#include "stdafx.h"
#include "Terrain_Act.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "MyForm.h"

CTerrain_Act::CTerrain_Act()
{
}


CTerrain_Act::~CTerrain_Act()
{
	Release();
}

HRESULT CTerrain_Act::Initialize()
{
	m_eType = OBJ_TYPE::TERRAIN;

	CMainFrame*		pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	m_pMainView =	static_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	m_pFormView = static_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Tool/Tile_%d.png", TEX_MULTI, L"Terrain_Tool", L"Tile", 3)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Act1/Tile_%d.png", TEX_MULTI, L"Act1Terrain", L"Tile", 326)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}
	
	return S_OK;
}

int CTerrain_Act::Update()
{
	return 0;
}

void CTerrain_Act::Late_Update()
{
}

void CTerrain_Act::Render()
{
	if (m_vecActTile.empty())
		return;

	D3DXMATRIX	matWorld, matScale, matTrans;

	int		iCullX = int(m_pMainView->GetScrollPos(0) / TILECX);
	int		iCullY = int(m_pMainView->GetScrollPos(1)) / (TILECY / 2);

	RECT	rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fRatioX = WINCX / float(rc.right - rc.left);
	float	fRatioY = WINCY / float(rc.bottom - rc.top);

	int	iCullEndX = rc.right / TILECX + 2;
	int	iCullEndY = rc.bottom / (TILECY / 2) + 2;

	for (int i = iCullY; i < iCullY + iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullEndX; ++j)
		{
			int		iIndex = i * m_ActTileX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecActTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecActTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0),
				m_vecActTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1),
				m_vecActTile[iIndex]->vPos.z);

			matWorld = matScale * matTrans;

			Set_Ratio(&matWorld, fRatioX, fRatioY);

			const TEXINFO*		pTexInfo = nullptr;
			if (m_vecActTile[iIndex]->eType == TERRIAN_TYPE::TYPEEND)
			{
				pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain_Tool", L"Tile", m_vecActTile[iIndex]->byDrawID);
			}
			else if (m_vecActTile[iIndex]->eType == TERRIAN_TYPE::ACT1)
			{
				pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act1Terrain", L"Tile", m_vecActTile[iIndex]->byDrawID);
			}
			else if (m_vecActTile[iIndex]->eType == TERRIAN_TYPE::ACT2)
			{
				pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act2Terrain", L"Tile", m_vecActTile[iIndex]->byDrawID);
			}
			else if (m_vecActTile[iIndex]->eType == TERRIAN_TYPE::ACT3)
			{
				pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act3Terrain", L"Tile", m_vecActTile[iIndex]->byDrawID);
			}
			if (nullptr == pTexInfo)
				return;

			float		fX = pTexInfo->tImgInfo.Width / 2.f;
			float		fY = pTexInfo->tImgInfo.Height / 2.f;

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(fX, fY, 0.f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain_Act::Release()
{
	for_each(m_vecActTile.begin(), m_vecActTile.end(), CDeleteObj());
	m_vecActTile.clear();
	m_vecActTile.shrink_to_fit();
}


void CTerrain_Act::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
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

void CTerrain_Act::Create_Terrian(int _TileX, int _TileY)
{
	for (int i = 0; i < _TileY; ++i)
	{
		for (int j = 0; j < _TileX; ++j)
		{
			TILE* pTile = new TILE;

			float	fX = (TILECX * j) + ((TILECX / 2.f) * (i % 2));
			float	fY = (TILECY / 2.f) * i;

			pTile->eType = TERRIAN_TYPE::TYPEEND;
			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { 1.f, 1.f, 1.f };
			pTile->byOption = NONETILE;
			pTile->byDrawID = 0;
			pTile->bCheckUnit = false;
			pTile->bCheckTile = false;

			pTile->iIndex = i * _TileX + j;
			pTile->iParentIdx = 0;

			m_vecActTile.push_back(pTile);
		}
	}
	m_ActTileX = _TileX;
	m_ActTileY = _TileY;

}

void CTerrain_Act::Mini_Render()
{
	if (m_vecActTile.empty())
		return;

	D3DXMATRIX	matWorld, matScale, matTrans;

	float	fRatioX = WINCX / float(TILECX * m_ActTileX);
	float	fRatioY = WINCY / float((TILECY / 2) * m_ActTileY);

	for (auto iter : m_vecActTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			iter->vPos.x,
			iter->vPos.y,
			0.f);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fRatioX, fRatioY);

		const TEXINFO*		pTexInfo = nullptr;
		if (iter->eType == TERRIAN_TYPE::TYPEEND)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain_Tool", L"Tile", iter->byDrawID);
		}
		else if (iter->eType == TERRIAN_TYPE::ACT1)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act1Terrain", L"Tile", iter->byDrawID);
		}
		else if (iter->eType == TERRIAN_TYPE::ACT2)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act2Terrain", L"Tile", iter->byDrawID);
		}
		else if (iter->eType == TERRIAN_TYPE::ACT3)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Act3Terrain", L"Tile", iter->byDrawID);
		}
		if (nullptr == pTexInfo)
			return;

		float	fX = pTexInfo->tImgInfo.Width / 2.f;
		float	fY = pTexInfo->tImgInfo.Height / 2.f;

		// 이미지에 행렬을 반영
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fX, fY, 0.f),
			nullptr,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

int CTerrain_Act::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t index = 0; index < m_vecActTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

void CTerrain_Act::Tile_Change(const D3DXVECTOR3 & vPos, TILE* _Tile)
{
	if (!_Tile)
		return;

	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecActTile[iIndex]->eType = _Tile->eType;
	m_vecActTile[iIndex]->byDrawID = _Tile->byDrawID;
	m_vecActTile[iIndex]->byOption = _Tile->byOption;
	m_vecActTile[iIndex]->fDamage = _Tile->fDamage;

	m_vecActTile[iIndex]->bCheckTile = true;
	m_PreType = _Tile->eType;
	m_PreID = _Tile->byDrawID;

}

void CTerrain_Act::Tile_LookChange(const D3DXVECTOR3 & vPos, TILE * _Tile)
{
	if (!_Tile)
		return;

	RECT	rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);

	int	ilEndX = rc.right - 10 + m_pMainView->GetScrollPos(0);

	if ((m_vecActTile.back()->vPos.x + (TILECX * 0.5)) < vPos.x ||
		(m_vecActTile.back()->vPos.y) < vPos.y ||
		ilEndX <= vPos.x)
	{
		if (m_CurIndex != -1)
		{
			if (m_vecActTile[m_CurIndex]->bCheckTile)
			{
				m_vecActTile[m_CurIndex]->eType = m_PreType;
				m_vecActTile[m_CurIndex]->byDrawID = m_PreID;
			}
			if (!m_vecActTile[m_CurIndex]->bCheckTile)
			{
				m_vecActTile[m_CurIndex]->eType = TERRIAN_TYPE::TYPEEND;
				m_vecActTile[m_CurIndex]->byDrawID = 0;
			}
		}

		m_CurIndex = -1;
		m_PreIndex = -1;
		return;
	}

	if (m_CurIndex == -1)
	{
		m_CurIndex = Get_TileIndex(vPos);
		m_PreIndex = m_CurIndex;
		m_PreType = m_vecActTile[m_CurIndex]->eType;
		m_PreID = m_vecActTile[m_CurIndex]->byDrawID;
		m_vecActTile[m_CurIndex]->eType = _Tile->eType;
		m_vecActTile[m_CurIndex]->byDrawID = _Tile->byDrawID;
		return;
	}
	else
	{
		m_CurIndex = Get_TileIndex(vPos);
	}
	
	if (m_CurIndex != m_PreIndex)
	{
		if (-1 == m_CurIndex)
			return;

		if (m_vecActTile[m_PreIndex]->bCheckTile)
		{
			m_vecActTile[m_PreIndex]->eType = m_PreType;
			m_vecActTile[m_PreIndex]->byDrawID = m_PreID;

			m_PreType = m_vecActTile[m_CurIndex]->eType;
			m_PreID = m_vecActTile[m_CurIndex]->byDrawID;

			m_vecActTile[m_CurIndex]->eType = _Tile->eType;
			m_vecActTile[m_CurIndex]->byDrawID = _Tile->byDrawID;

		}
		if (!m_vecActTile[m_PreIndex]->bCheckTile)
		{
			m_vecActTile[m_PreIndex]->eType = TERRIAN_TYPE::TYPEEND;
			m_vecActTile[m_PreIndex]->byDrawID = 0;

			m_PreType = m_vecActTile[m_CurIndex]->eType;
			m_PreID = m_vecActTile[m_CurIndex]->byDrawID;

			m_vecActTile[m_CurIndex]->eType = _Tile->eType;
			m_vecActTile[m_CurIndex]->byDrawID = _Tile->byDrawID;
		}
		

		m_PreIndex = m_CurIndex;
	}
	


}

bool CTerrain_Act::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3		vPoint[4]{

		{ m_vecActTile[iIndex]->vPos.x,m_vecActTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ m_vecActTile[iIndex]->vPos.x + (TILECX / 2.f),m_vecActTile[iIndex]->vPos.y, 0.f },
		{ m_vecActTile[iIndex]->vPos.x,m_vecActTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ m_vecActTile[iIndex]->vPos.x - (TILECX / 2.f),m_vecActTile[iIndex]->vPos.y , 0.f }
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

