#pragma once
#include "Obj.h"

class CToolView;
class CTerrain_Act2 :
	public CObj
{
public:
	CTerrain_Act2();
	virtual ~CTerrain_Act2();

	// Inherited via CObj
	virtual HRESULT Initialize() override;
	virtual int		Update() override;
	virtual void	Late_Update() override;
	virtual void	Render() override;
	virtual void	Release() override;
	virtual	void	Tool_Render(const D3DXVECTOR3& _vWorld)	override;

public:
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);
	void		Create_Terrian(int _TileX, int _TileY);

public:
	int		Get_TileIndex(const D3DXVECTOR3& vPos);
	void	Tile_Change(const D3DXVECTOR3& vPos, TILE* _Tile);
	bool	Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

public:
	vector<TILE*>		m_vecAct2Tile;
	CToolView*			m_pMainView = nullptr;

	UINT				m_Act2TileX;
	UINT				m_Act2TileY;
};

