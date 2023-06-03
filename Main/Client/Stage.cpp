#include "stdafx.h"
#include "Stage.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "MyTerrain.h"
#include "Player.h"
CStage::CStage()
{
}
CStage::~CStage()
{
	Release_Scene();
}
HRESULT CStage::Ready_Scene()
{
#pragma region PUBLIC
#pragma endregion

#pragma region HEEJUN
	CObj*	pTerrian = new CMyTerrain;
	if (nullptr == pTerrian)
		return E_FAIL;
	static_cast<CMyTerrain*>(pTerrian)->Set_Act(TERRIAN_TYPE::ACT1);

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::TERRAIN, pTerrian);
#pragma endregion

#pragma region CHAN

	if (FAILED(CObjMgr::Get_Instance()->Load_Unit_Instances(L"../Data/01.Object/map/Map_Instances.dat")))
		return E_FAIL;

	CObj* pPlayer = new CPlayer;
	if (nullptr == pPlayer)
		return E_FAIL;
	pPlayer->Initialize();

	CObjMgr::Get_Instance()->Add_Object(CObjMgr::PLAYER, pPlayer);
#pragma endregion

	return S_OK;
}
void CStage::Update_Scene()
{
	CObjMgr::Get_Instance()->Update();
}
void CStage::Late_Update_Scene()
{
	CObjMgr::Get_Instance()->Late_Update();
}
void CStage::Render_Scene()
{
	CObjMgr::Get_Instance()->Render();
}
void CStage::Release_Scene()
{
	//CObjMgr::Get_Instance()->Release();
}
