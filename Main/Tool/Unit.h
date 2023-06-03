#pragma once
#include "Obj.h"


class CUnit :
	public CObj
{
public:
	CUnit();
	virtual ~CUnit();

	// Inherited via CObj
	virtual HRESULT Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Tool_Render(const D3DXVECTOR3& _vWorld) override;
	virtual void Release() override;



};

