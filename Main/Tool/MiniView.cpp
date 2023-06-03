// MiniView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MiniView.h"
#include "Device.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain_Act.h"
#include "MyForm.h"

// CMiniView

IMPLEMENT_DYNCREATE(CMiniView, CView)

CMiniView::CMiniView()
{

}

CMiniView::~CMiniView()
{
}

BEGIN_MESSAGE_MAP(CMiniView, CView)
END_MESSAGE_MAP()


// CMiniView 그리기입니다.

void CMiniView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	CMyForm*		pMainView = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	CTerrain_Act*	pTerrain = nullptr;

	if (pMainView->m_pMapTool_Tab2->m_Combo_SelecMap.GetCurSel() == (int)TERRIAN_TYPE::ACT1)
		pTerrain = pMainView->m_pMapTool_Tab2->m_pTerrain_Act1;
	else if (pMainView->m_pMapTool_Tab2->m_Combo_SelecMap.GetCurSel() == (int)TERRIAN_TYPE::ACT2)
		pTerrain = pMainView->m_pMapTool_Tab2->m_pTerrain_Act2;
	else if (pMainView->m_pMapTool_Tab2->m_Combo_SelecMap.GetCurSel() == (int)TERRIAN_TYPE::ACT3)
		pTerrain = pMainView->m_pMapTool_Tab2->m_pTerrain_Act3;

	CDevice::Get_Instance()->Render_Begin();

	if (pTerrain != nullptr)
	{
		pTerrain->Mini_Render();
	}
	
	CDevice::Get_Instance()->Render_End(m_hWnd);

}


// CMiniView 진단입니다.

#ifdef _DEBUG
void CMiniView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniView 메시지 처리기입니다.
