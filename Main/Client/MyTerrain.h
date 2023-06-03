#pragma once
#include "Obj.h"
class CMyTerrain :	public CObj
{
public:
	CMyTerrain();
	virtual ~CMyTerrain();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT			Initialize(void)	override;
	virtual int				Update(void)		override;
	virtual void			Late_Update(void)	override;
	virtual void			Render(void)		override;
	virtual void			Release(void)		override;

public:
	vector<TILE*>&			Get_VecTile()				{ return m_vecTile; }
	vector<list<TILE*>>&	Get_VecAdj()				{ return m_vecAdj; }
	vector<TILE*>&			Get_VecTilePathRender()		{ return m_vecTilePathRender; }

	UINT					Get_TerrianRow()			{ return m_iRow; }
	UINT					Get_TerrianCol()			{ return m_iCol; }

	HRESULT					Set_Act(TERRIAN_TYPE _eType);

public:
	void					Render_PathTile();
	void					Init_PathTile();

private:
	HRESULT					LoadTile(const TCHAR* pTilePath);
	HRESULT					Ready_Adjacency();
	HRESULT					Ready_PathRender();

private:
	vector<list<TILE*>>		m_vecAdj;

	vector<TILE*>			m_vecTile;
	vector<TILE*>			m_vecTilePathRender;


	float					m_fRealRow;
	float					m_fRealCol;
	int					m_iRow;
	int					m_iCol;
};

