#pragma once

#include "Include.h"

class CAstarMgr
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

public:
	list<TILE*>&	Get_BestList() { return m_BestList; }

public:
	void		Start_Astar(const D3DXVECTOR3& vStart, const D3DXVECTOR3& vGoal);

	bool		Make_Route(int iStartIdx, int iGoalIdx);
	void		Make_BestList(int iStartIdx, int iGoalIdx);

public:
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	void		Release();

private:
	bool		Check_Close(int iIndex);
	bool		Check_Open(int iIndex);

private:
	list<int>		m_OpenList;
	list<int>		m_CloseList;
	list<TILE*>		m_BestList;

	int				m_iStartIdx;
};

