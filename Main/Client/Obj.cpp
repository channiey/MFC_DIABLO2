#include "stdafx.h"
#include "Obj.h"
#include "TimeMgr.h"

D3DXVECTOR3 CObj::m_vScroll{};

CObj::CObj()
	: m_fAcc(0.f)
{
	/*ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vLook = { 1.f, 0.f, 0.f };
	ZeroMemory(&m_tFrame, sizeof(FRAME));*/
}


CObj::~CObj()
{
}


void CObj::Move_Frame(void)
{
	if (nullptr == m_pCurAni)
		return;

	// Origin
	/*m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (m_tFrame.fFrame >= m_tFrame.fMax)
		m_tFrame.fFrame = 0.f;*/

	// Add

	if (false == m_bPlay) return;

	m_fAcc += CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (m_pCurAni->fSecondPerFrame < m_fAcc)
	{
		m_fAcc = 0.f;

		++m_pCurAni->iCurFrame;

		if (m_pCurAni->iCurFrame == m_pCurAni->iMaxFrame)
			m_pCurAni->iCurFrame = 0;
	}
}
