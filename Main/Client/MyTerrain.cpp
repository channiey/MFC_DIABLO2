#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CMyTerrain::CMyTerrain()
	: m_fRealRow(0.f), m_fRealCol(0.f)
{
}

CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	return S_OK;
}

HRESULT CMyTerrain::Set_Act(TERRIAN_TYPE _eType)
{
	switch (_eType)
	{
	case TERRIAN_TYPE::ACT1:
	{
		if (FAILED(LoadTile(L"../Data/Save_Act1_Map.dat")))
			return E_FAIL;

		m_wstrObjKey = L"Tile";
		m_wstrStateKey = L"Act1";
	}
	break;
	case TERRIAN_TYPE::ACT2:
	{
		if (FAILED(LoadTile(L"../Data/Save_Act2_Map.dat")))
			return E_FAIL;

		m_wstrObjKey = L"Tile";
		m_wstrStateKey = L"Act2";
	}
	break;
	case TERRIAN_TYPE::ACT3:
	{
		if (FAILED(LoadTile(L"../Data/Save_Act3_Map.dat")))
			return E_FAIL;

		m_wstrObjKey = L"Tile";
		m_wstrStateKey = L"Act3";
	}
	break;
	default:
		break;
	}

	if (FAILED(Ready_Adjacency()))
		return E_FAIL;

	if (FAILED(Ready_PathRender()))
		return E_FAIL;


	return S_OK;
}


int CMyTerrain::Update(void)
{
	Move_Frame();

	if (KEYBOARD_SCROLL)
	{
		if (0.f >= __super::m_vScroll.x)
		{
			if (GetAsyncKeyState('A'))
				__super::m_vScroll.x += SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

			if (GetAsyncKeyState('D'))
				__super::m_vScroll.x -= SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

			if (0.f < __super::m_vScroll.x)
				__super::m_vScroll.x = 0.f;
		}

		if (0.f >= __super::m_vScroll.y)
		{
			if (GetAsyncKeyState('W'))
				__super::m_vScroll.y += SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

			if (GetAsyncKeyState('S'))
				__super::m_vScroll.y -= SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

			if (0.f < __super::m_vScroll.y)
				__super::m_vScroll.y = 0.f;
		}
		return 0;
	}
	if (0.f >= __super::m_vScroll.x)
	{
		if (10.f > ::Get_Mouse().x)
			__super::m_vScroll.x += SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (WINCX - 10 < ::Get_Mouse().x)
			__super::m_vScroll.x -= SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (0.f < __super::m_vScroll.x)
			__super::m_vScroll.x = 0.f;
	}

	if (0.f >= __super::m_vScroll.y)
	{
		if (10.f > ::Get_Mouse().y)
			__super::m_vScroll.y += SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (WINCY - 10 < ::Get_Mouse().y)
			__super::m_vScroll.y -= SCROLL_SPEED * CTimeMgr::Get_Instance()->Get_TimeDelta();

		if (0.f < __super::m_vScroll.y)
			__super::m_vScroll.y = 0.f;
	}

	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	D3DXMATRIX		matWorld,matTrans;

	int iScrollX = (int)__super::m_vScroll.x;


	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iCullEndX = WINCX / (TILECX) + 3;
	int		iCullEndY = WINCY / (TILECY / 2) + 3;

	for (int i = iCullY; i < iCullY + iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullEndX; ++j)
		{
			int		iIndex = i * m_iRow + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + m_vScroll.x,
				m_vecTile[iIndex]->vPos.y + m_vScroll.y,
				m_vecTile[iIndex]->vPos.z);

			matWorld = matTrans;

			if (m_vecTile[iIndex]->eType == TERRIAN_TYPE::TYPEEND)
				continue;

			switch (m_vecTile[iIndex]->eType)
			{
			case TERRIAN_TYPE::ACT1:
				m_wstrStateKey = L"Act1";
				break;
			case TERRIAN_TYPE::ACT2:
				m_wstrStateKey = L"Act2";
				break;
			case TERRIAN_TYPE::ACT3:
				m_wstrStateKey = L"Act3";
				break;
			default:
				break;
			}


			const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), m_vecTile[iIndex]->byDrawID);
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

void CMyTerrain::Render_PathTile()
{
	D3DXMATRIX		matWorld, matTrans;

	int iScrollX = (int)__super::m_vScroll.x;


	int		iCullX = int(-m_vScroll.x) / TILECX;
	int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	int		iCullEndX = WINCX / (TILECX)+3;
	int		iCullEndY = WINCY / (TILECY / 2) + 3;

	for (int i = iCullY; i < iCullY + iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullX + iCullEndX; ++j)
		{
			int		iIndex = i * m_iRow + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTilePathRender.size())
				continue;

			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matTrans,
				m_vecTilePathRender[iIndex]->vPos.x + m_vScroll.x,
				m_vecTilePathRender[iIndex]->vPos.y + m_vScroll.y,
				m_vecTilePathRender[iIndex]->vPos.z);

			matWorld = matTrans;

			if (m_vecTile[iIndex]->eType == TERRIAN_TYPE::TYPEEND)
				continue;

		/*	switch (m_vecTile[iIndex]->eType)
			{
			case TERRIAN_TYPE::ACT1:
				m_wstrStateKey = L"Act1";
				break;
			case TERRIAN_TYPE::ACT2:
				m_wstrStateKey = L"Act2";
				break;
			case TERRIAN_TYPE::ACT3:
				m_wstrStateKey = L"Act3";
				break;
			default:
				break;
			}*/


			if (m_vecTilePathRender[iIndex]->byDrawID == TILE_PATH_RENDER_CLOSE) continue;

			const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Tile", L"Tool", m_vecTilePathRender[iIndex]->byDrawID);

			//const TEXINFO*		pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), m_vecTile[iIndex]->byDrawID);
			if (nullptr == pTexInfo)
				continue;


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

void CMyTerrain::Init_PathTile()
{
	for (int i = 0; i < m_vecTilePathRender.size(); ++i)
	{
		if (nullptr == m_vecTilePathRender[i]) continue;
			m_vecTilePathRender[i]->byDrawID = TILE_PATH_RENDER_CLOSE;
	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();


	for_each(m_vecTilePathRender.begin(), m_vecTilePathRender.end(), CDeleteObj());
	m_vecTilePathRender.clear();
	m_vecTilePathRender.shrink_to_fit();
}

HRESULT CMyTerrain::LoadTile(const TCHAR* pTilePath)
{
	HANDLE	hFile = CreateFile(pTilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	TILE*	pTile = nullptr;

	int		iCount = 0;
	UINT	iRow = 0;
	UINT	iCol = 0;
	DWORD	dwByte = 0;

	while (true)
	{
		if (iCount == 0)
		{
			ReadFile(hFile, &iRow, sizeof(UINT), &dwByte, nullptr);
		}
		else if (iCount == 1)
		{
			ReadFile(hFile, &iCol, sizeof(UINT), &dwByte, nullptr);
		}
		else if (iCount >= 2)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			m_vecTile.push_back(pTile);
		}

		++iCount;
	}

	m_iRow = iRow;
	m_iCol = iCol;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMyTerrain::Ready_Adjacency()
{
	
	if (m_vecTile.empty())
		return E_FAIL;

	m_vecAdj.resize(m_vecTile.size());

	for (int i = 0; i < m_iCol; ++i)
	{
		for (int j = 0; j < m_iRow; ++j)
		{
			int iIndex = i * m_iRow + j;

			/*
			//аб
			if (0 != (iIndex % (m_iRow * 2)))
			{
				if (m_vecTile[iIndex - 1]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - 1]);

				}
			}
			//╩С
			if ((0 != i))
			{
				if (m_vecTile[iIndex - (m_iRow * 2)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (m_iRow * 2)]);

				}
			}
			//©Л
			if (((m_iRow * 2) - 1) != (iIndex % (m_iRow * 2)))
			{
				if (m_vecTile[iIndex + 1]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + 1]);

				}
			}
			//го
			if ((m_iCol - 1 != i))
			{
				if (m_vecTile[iIndex + (m_iRow * 2)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (m_iRow * 2)]);

				}
			}
			*/

			//аб╩С
			if ((0 != i) && 0 != (iIndex % (m_iRow * 2)))
			{
				if ((0 != i % 2) && m_vecTile[iIndex - m_iRow]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - m_iRow]);
				}
				else if ((0 == i % 2) && m_vecTile[iIndex - (m_iRow + 1)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (m_iRow + 1)]);
				}
			}
			//©Л╩С
			if ((0 != i) && ((m_iRow * 2) - 1) != (iIndex % (m_iRow * 2)))
			{
				if ((0 != i % 2) && m_vecTile[iIndex - (m_iRow - 1)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - (m_iRow - 1)]);
				}
				else if ((0 == i % 2) && m_vecTile[iIndex - m_iRow]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex - m_iRow]);
				}
			}
			//абго
			if ((m_iCol - 1 != i) && 0 != (iIndex % (m_iRow * 2)))
			{
				if ((0 != i % 2) && m_vecTile[iIndex + m_iRow]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + m_iRow]);
				}
				else if ((0 == i % 2) && m_vecTile[iIndex + (m_iRow - 1)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (m_iRow - 1)]);
				}
			}
			//аб©Л
			if ((m_iCol - 1 != i) && ((m_iRow * 2) - 1) != (iIndex % (m_iRow * 2)))
			{
				if ((0 != i % 2) && m_vecTile[iIndex + (m_iRow + 1)]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + (m_iRow + 1)]);
				}
				else if ((0 == i % 2) && m_vecTile[iIndex + m_iRow]->byOption == NONETILE)
				{
					m_vecAdj[iIndex].push_back(m_vecTile[iIndex + m_iRow]);
				}
			}

		}

	}

	return S_OK;
	


}

HRESULT CMyTerrain::Ready_PathRender()
{
	if (m_vecTile.empty())
		return E_FAIL;

	const int iSize = m_vecTile.size();

	m_vecTilePathRender.reserve(iSize);

	for (int i = 0; i < iSize; ++i)
	{
		TILE* pTile = new TILE;

		if (nullptr == pTile) continue;

		pTile->vPos = m_vecTile[i]->vPos;
		pTile->vSize = m_vecTile[i]->vSize;
		pTile->byDrawID = TILE_PATH_RENDER_CLOSE;
		m_vecTilePathRender.push_back(pTile);
	}

	return E_NOTIMPL;
}


