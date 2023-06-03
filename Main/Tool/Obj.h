#pragma once

#include "Include.h"
class CToolView;

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	virtual HRESULT		Initialize()							PURE;
	virtual int			Update()								PURE;
	virtual void		Late_Update()							PURE;
	virtual void		Render()								PURE;
	virtual	void		Tool_Render(const D3DXVECTOR3& _vWorld)							PURE;
	virtual void		Release()								PURE;

public:

	// ���� ��������� public���� �δ� ���� ���� ���� ������ �׼��� �޼���� ������� �ʵ��� �Ѵ�.
	// Ŭ���̾�Ʈ���� ����� ���� ����� ���ܵд�.

	/*CString				Get_ObjKey() const		{ return m_strObjKey; }
	CString				Get_StateKey() const	{ return m_strStateKey; }
	INFO				Get_Info(void) const	{ return m_tInfo; }
	STAT				Get_Stat() const		{ return m_tStat; }
	FRAME				Get_Frame() const		{ return m_tFrame; }
	OBJ_TYPE			Get_Type() const		{ return m_eType; }
	OBJ_STATE			Get_State() const		{ return m_eState; }*/


	void				Move_Frame(void); // �ִϸ��̼� ����

public:
	CString				m_strName;
	INFO				m_tInfo; 
	STAT				m_tStat;
	//FRAME				m_tFrame;
	OBJ_TYPE			m_eType			= OBJ_TYPE::TYPEEND;
	OBJ_STATE			m_eState		= OBJ_STATE::TYPEEND;
	OBJ_DIR				m_eDir			= OBJ_DIR::TYPEEND;

	CString				m_strObjKey		= L""; 
	CString				m_strStateKey	= L""; 
	
	D3DXVECTOR3			m_vWorldPos = D3DXVECTOR3{};
	bool				m_bPlay = true;

	map<CString, ANIMATION*>	m_mapAni;
	ANIMATION*			m_pCurAni = nullptr;
	float				m_fAcc;

	CToolView*			m_pMainView = nullptr;

};

