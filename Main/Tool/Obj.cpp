#include "stdafx.h"
#include "Obj.h"
#include "TimeMgr.h"
#include "ToolView.h"
CObj::CObj()
{
}


CObj::~CObj()
{
}

void CObj::Move_Frame(void)
{
	/*
	//m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::Get_Instance()->Get_TimeDelta();

	//if (m_tFrame.fFrame > m_tFrame.fMax)
	//	m_tFrame.fFrame = 0.f;
	*/

	if (false == m_bPlay) return;
	m_fAcc += CTimeMgr::Get_Instance()->Get_TimeDelta();
	if (m_pCurAni->fSecondPerFrame < m_fAcc)
	{
		m_fAcc = 0.f;
		++m_pCurAni->iCurFrame;
		if(nullptr != m_pMainView)
			m_pMainView->Invalidate(FALSE);
		if (m_pCurAni->iCurFrame == m_pCurAni->iMaxFrame)
		{
			m_pCurAni->iCurFrame = 0;
			/*m_bPlay = false;
			return;*/
			/*if (m_pCurAni->bLoop)
			{

			}
			else
			{

			}*/
		}
		/*CString strDir;
		strDir.Format(_T("%d"), m_pCurAni->iCurFrame * 2);
		m_strStateKey += strDir;*/
		//CString strDir;
		//int iCurSel = ANI_COMBO_OBJ_DIR.GetCurSel();
		//strDir.Format(_T("%d"), iCurSel * 2);
		//strState += strDir;
	}
}
