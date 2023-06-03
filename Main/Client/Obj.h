#pragma once

#include "Include.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	const wstring&	Get_ObjKey()const { return m_wstrObjKey; }

	const INFO&		Get_Info(void)const { return m_tInfo; }
	
	void			Set_Pos(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos; }
	
	void			Set_Dir(const D3DXVECTOR3& vDir) 
	{ 
		m_tInfo.vDir = vDir;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	}

public:
	virtual HRESULT				Initialize(void)PURE;
	virtual int					Update(void)PURE;
	virtual void				Late_Update(void)PURE;
	virtual void				Render(void)PURE;
	virtual void				Release(void)PURE;

protected:
	void						Move_Frame(void);

public:

	static D3DXVECTOR3			m_vScroll;
	
	// Origin
	INFO						m_tInfo;
	wstring						m_wstrObjKey = L"";
	wstring						m_wstrStateKey = L"";
	FRAME						m_tFrame;


	// Add
	wstring						m_wstrName;
	STAT						m_tStat;
	OBJ_TYPE					m_eType = OBJ_TYPE::TYPEEND;
	OBJ_STATE					m_eState = OBJ_STATE::TYPEEND;
	OBJ_DIR						m_eDir = OBJ_DIR::TYPEEND;


	D3DXVECTOR3					m_vWorldPos = D3DXVECTOR3{};
	bool						m_bPlay = true;

	ANIMATION*					m_pCurAni = nullptr;
	float						m_fAcc;
	map<wstring, ANIMATION*>	m_mapAni;



};

