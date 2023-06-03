#include "stdafx.h"
#include "Unit.h"
#include "TextureMgr.h"
#include "SingleTexture.h"
#include "MultiTexture.h"
#include "Device.h"
#include "ToolView.h"
#include "MainFrm.h"
CUnit::CUnit()
{
}


CUnit::~CUnit()
{
	Release();
}

HRESULT CUnit::Initialize()
{
	m_tInfo.vPos		= { 0.f, 0.f, 0.f };
	m_tInfo.vRot		= { 0.f, 0.f, 0.f };
	m_tInfo.vSize		= { 3.f, 3.f, 1.f };

	m_strName = L"New_Unit";
	m_tStat.iHP			= int(UNIT_MAX_HP * 0.5f);
	m_tStat.iAD			= int(UNIT_MAX_AD * 0.5f);
	m_tStat.iSpeed		= int(UNIT_MAX_SPEED * 0.5f);

	m_eType				= OBJ_TYPE::TYPEEND;
	m_eState			= OBJ_STATE::TYPEEND;
	m_eDir				= OBJ_DIR::TYPEEND; 
	
	m_strObjKey			= L"";
	m_strStateKey		= L"";

	m_bPlay				= true;



	return E_NOTIMPL;
}

int CUnit::Update()
{
	return 0;
}

void CUnit::Late_Update()
{
}

void CUnit::Render()
{
	D3DXMATRIX matScale, matRotZ, matTrans;

	D3DXVECTOR3 vPos = m_tInfo.vPos + m_vWorldPos; // �÷��̾� ���� �����ǿ�, ���� ������ ���� �����ش�.
	
	vPos.x -= m_pMainView->GetScrollPos(0);
	vPos.y -= m_pMainView->GetScrollPos(1);

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z);
	D3DXMatrixRotationZ(&matRotZ, m_tInfo.vRot.z);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	//const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Stand", 0);
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_strObjKey, m_strStateKey, m_pCurAni->iCurFrame);

	if (nullptr == pTexInfo) return;

	float	fX = pTexInfo->tImgInfo.Width / 2.f;
	float	fY = pTexInfo->tImgInfo.Height / 2.f;

	// �̹����� ����� �ݿ�
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,							// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0 �������� ���
		&D3DXVECTOR3(fX, fY, 0.f),			// ����� �̹����� �߽��࿡ ���� vector3 �ּ�, null�� ��� �̹����� 0, 0�� �߽� ��ǥ
		nullptr,							// ��ġ ��ǥ�� ���� vector3 �ּ�, null�� ��� ��ũ�� ���� 0, 0��ǥ ���
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
}

void CUnit::Release()
{

	for_each(m_mapAni.begin(), m_mapAni.end(),
		[](auto& MyPair)
	{
		Safe_Delete(MyPair.second);
	});


	m_mapAni.clear();
}

void CUnit::Tool_Render(const D3DXVECTOR3& _vWorld)
{
	D3DXMATRIX matScale, matRotZ, matTrans;

	D3DXVECTOR3 vPos = m_tInfo.vPos + _vWorld; // �÷��̾� ���� �����ǿ�, ���� ������ ���� �����ش�.

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, m_tInfo.vSize.z);
	D3DXMatrixRotationZ(&matRotZ, m_tInfo.vRot.z);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	//const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", L"Stand", 0);
	const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_strObjKey, m_strStateKey, m_pCurAni->iCurFrame);

	if (nullptr == pTexInfo) return;

	float	fX = pTexInfo->tImgInfo.Width / 2.f;
	float	fY = pTexInfo->tImgInfo.Height / 2.f;

	// �̹����� ����� �ݿ�
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,							// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0 �������� ���
		&D3DXVECTOR3(fX, fY, 0.f),			// ����� �̹����� �߽��࿡ ���� vector3 �ּ�, null�� ��� �̹����� 0, 0�� �߽� ��ǥ
		nullptr,							// ��ġ ��ǥ�� ���� vector3 �ּ�, null�� ��� ��ũ�� ���� 0, 0��ǥ ���
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� ���� ����
}
