#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Unit.h"
#include "MyTerrain.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release(); 
}

void CObjMgr::Add_Object(ID eID, CObj * pObject)
{
	if (nullptr == pObject || END <= eID)
		return; 

	m_listObject[eID].emplace_back(pObject); 
}

void CObjMgr::Update()
{
	for (int i = 0 ; i < END; ++i)
	{
		for (auto& iter = m_listObject[i].begin() ; iter != m_listObject[i].end(); )
		{
			int iEvent = (*iter)->Update();

			if (OBJ_DEAD == iEvent)
			{
				if(i != EFFECT)
					Safe_Delete(*iter);

				iter = m_listObject[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CObjMgr::Late_Update()
{
	for (int i = 0 ; i < END ; ++i)
	{
		for (auto& pObject : m_listObject[i])
			pObject->Late_Update();
	}
}

void CObjMgr::Render()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			pObject->Render();
	}

	CObj* pTerrain = Get_Terrain();

	if (nullptr == pTerrain) return;

	if (!static_cast<CMyTerrain*>(pTerrain)->Get_VecTilePathRender().empty())
		static_cast<CMyTerrain*>(pTerrain)->Render_PathTile();
}

void CObjMgr::Release()
{
	for (int i = 0; i < END; ++i)
	{
		for (auto& pObject : m_listObject[i])
			Safe_Delete(pObject); 

		m_listObject[i].clear();
	}
}

HRESULT CObjMgr::Load_Unit_Instances(const TCHAR*  _strPath)
{
	HANDLE	hFile = CreateFile(_strPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;
	CObj*	pUnit = nullptr;
	wstring OBJ_STATE_STRING[(UINT)OBJ_STATE::TYPEEND]{ L"stand", L"walk", L"dash", L"attack", L"damage", L"skill", L"die" };

	while (true)
	{
		pUnit = new CUnit;

		// 이름 문자열 세팅
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				pName = nullptr;
				Safe_Delete(pUnit);
				break;
			}

			pUnit->m_wstrName = pName;
			delete[]pName;
			pName = nullptr;
		}

		// 애니메이션 세팅
		{
			// Animation Set
			ANIMATION* pAni = new ANIMATION;

			pAni->bLoop = false;
			pAni->iCurFrame = 0;
			//pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND], pAni });
			pUnit->m_pCurAni = pAni;
		}

		// 나머지 멤버 변수 세팅
		{
			ReadFile(hFile, &(pUnit->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_tStat), sizeof(STAT), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eType), sizeof(OBJ_TYPE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eState), sizeof(OBJ_STATE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->fSecondPerFrame), sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->iMaxFrame), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_vWorldPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);

			pUnit->m_wstrObjKey = pUnit->m_wstrName;
			pUnit->m_wstrStateKey = OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] + L"_8";
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] , pUnit->m_pCurAni });
		}

		// 푸시백
		m_listObject[MONSTER].push_back(pUnit);
	}

	CloseHandle(hFile);

	return S_OK;
}
