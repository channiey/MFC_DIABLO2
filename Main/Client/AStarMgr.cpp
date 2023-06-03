#include "stdafx.h"
#include "AstarMgr.h"
#include "ObjMgr.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr() : m_iStartIdx(0)
{
}

CAstarMgr::~CAstarMgr()
{
	Release();
}

void CAstarMgr::Start_Astar(const D3DXVECTOR3 & vStart, const D3DXVECTOR3 & vGoal)
{

	Release();
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTile();
	vector<TILE*>& vecTilePathRender = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTilePathRender();
	dynamic_cast<CMyTerrain*>(pTerrain)->Init_PathTile();

	if (vecTile.empty())
		return;
	
	m_iStartIdx = Get_TileIndex(vStart);

	int iGoalIdx = Get_TileIndex(vGoal);

	if (0 > m_iStartIdx || 0 > iGoalIdx || (size_t)m_iStartIdx >= vecTile.size() || (size_t)iGoalIdx >= vecTile.size())
		return;

	if (m_iStartIdx == iGoalIdx)
		return;

	if (0 != vecTile[iGoalIdx]->byOption)
		return;

	vecTilePathRender[m_iStartIdx]->byDrawID = TILE_PATH_RENDER_OPEN;

	if (Make_Route(m_iStartIdx, iGoalIdx))
		Make_BestList(m_iStartIdx, iGoalIdx);
}

bool CAstarMgr::Make_Route(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTile();
	vector<list<TILE*>>& vecAdj = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecAdj();
	vector<TILE*>& vecTilePathRender = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTilePathRender();

	if (vecTile.empty() || vecAdj.empty())
		return false;

	if (!m_OpenList.empty())
		m_OpenList.pop_front();

	m_CloseList.push_back(iStartIdx);
	//vecTilePathRender[iStartIdx]->byDrawID = 2;

	for (auto& iter : vecAdj[iStartIdx])
	{
		if (iGoalIdx == iter->iIndex)
		{
			iter->iParentIdx = iStartIdx;
			return true;
		}

		if (false == Check_Close(iter->iIndex) &&
			false == Check_Open(iter->iIndex))
		{
			iter->iParentIdx = iStartIdx;
			m_OpenList.push_back(iter->iIndex);
			vecTilePathRender[iter->iIndex]->byDrawID = TILE_PATH_RENDER_OPEN;
		}

	}

	if (m_OpenList.empty())
		return false;

	int iOriginStart = m_iStartIdx;

	m_OpenList.sort([&vecTile, &iGoalIdx, &iOriginStart](int Dst, int Src) ->bool {

		D3DXVECTOR3		vPCost1 = vecTile[iOriginStart]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3		vPCost2 = vecTile[iOriginStart]->vPos - vecTile[Src]->vPos;

		D3DXVECTOR3		vGCost1 = vecTile[iGoalIdx]->vPos - vecTile[Dst]->vPos;
		D3DXVECTOR3		vGCost2 = vecTile[iGoalIdx]->vPos - vecTile[Src]->vPos;

		float	fCost1 = D3DXVec3Length(&vPCost1) + D3DXVec3Length(&vGCost1);
		float	fCost2 = D3DXVec3Length(&vPCost2) + D3DXVec3Length(&vGCost2);

		return fCost1 < fCost2;
	});

	return Make_Route(m_OpenList.front(), iGoalIdx);
}

void CAstarMgr::Make_BestList(int iStartIdx, int iGoalIdx)
{
	CObj* pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>& vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTile();
	vector<TILE*>& vecTilePathRender = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTilePathRender();

	if (vecTile.empty())
		return;

	m_BestList.push_front(vecTile[iGoalIdx]);
	vecTilePathRender[iGoalIdx]->byDrawID = TILE_PATH_RENDER_BEST;

	int iRouteIdx = vecTile[iGoalIdx]->iParentIdx;

	while (true)
	{
		if (iRouteIdx == iStartIdx)
			break;

		m_BestList.push_front(vecTile[iRouteIdx]);
		vecTilePathRender[iRouteIdx]->byDrawID = TILE_PATH_RENDER_BEST;

		iRouteIdx = vecTile[iRouteIdx]->iParentIdx;

	}
}

bool CAstarMgr::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return false;

	D3DXVECTOR3		vPoint[4]{

		{ vecTile[iIndex]->vPos.x,vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x + (TILECX / 2.f),vecTile[iIndex]->vPos.y, 0.f },
		{ vecTile[iIndex]->vPos.x,vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f },
		{ vecTile[iIndex]->vPos.x - (TILECX / 2.f),vecTile[iIndex]->vPos.y , 0.f }
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

int CAstarMgr::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	CObj*	pTerrain = CObjMgr::Get_Instance()->Get_Terrain();
	vector<TILE*>&	vecTile = dynamic_cast<CMyTerrain*>(pTerrain)->Get_VecTile();

	if (vecTile.empty())
		return -1;

	for (size_t index = 0; index < vecTile.size(); ++index)
	{
		if (Picking_Dot(vPos, index))
		{
			return index;
		}
	}

	return -1;
}

void CAstarMgr::Release()
{
	m_CloseList.clear();
	m_OpenList.clear();

	m_BestList.clear();
}

bool CAstarMgr::Check_Close(int iIndex)
{
	for (int& iCloseIdx : m_CloseList)
	{
		if (iIndex == iCloseIdx)
			return true;
	}

	return false;
}

bool CAstarMgr::Check_Open(int iIndex)
{
	for (int& iOpenIdx : m_OpenList)
	{
		if (iIndex == iOpenIdx)
			return true;
	}

	return false;
}
