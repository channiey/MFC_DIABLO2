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
	virtual void Release() override;



};

