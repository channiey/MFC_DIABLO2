// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"
#include "UnitTool_Tab1.h"
#include "MapTool_Tab2.h"
#include "MainFrm.h"
#include "ToolView.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnSelchangeTab1)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	CRect rc;

	this->GetClientRect(&rc);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Font.CreatePointFont(180, L"궁서");

	CRect rect{};

	m_tab.GetClientRect(rect);
	m_tab.InsertItem(0, L"Unit Tool");
	m_tab.InsertItem(1, L"Map Tool");
	m_tab.InsertItem(2, L"Tile Tool");

	m_tab.SetCurSel(0);

	m_pUnitTool_Tab1 = new CUnitTool_Tab1;
	m_pUnitTool_Tab1->Create(IDD_DIALOG_TAB1, &m_tab);
	m_pUnitTool_Tab1->MoveWindow(0, TAB_HEIGHT, rect.Width(), rect.Height() - TAB_HEIGHT);
	m_pUnitTool_Tab1->ShowWindow(SW_SHOW);


	m_pMapTool_Tab2 = new CMapTool_Tab2;
	m_pMapTool_Tab2->Create(IDD_DIALOG_TAB2, &m_tab);
	m_pMapTool_Tab2->MoveWindow(0, TAB_HEIGHT, rect.Width(), rect.Height() - TAB_HEIGHT);
	m_pMapTool_Tab2->ShowWindow(SW_HIDE);

	m_pTileTool_Tab3 = new CTileTool_Tab3;
	m_pTileTool_Tab3->Create(IDD_TILETOOL, &m_tab);
	m_pTileTool_Tab3->MoveWindow(0, TAB_HEIGHT, rect.Width(), rect.Height() - TAB_HEIGHT);
	m_pTileTool_Tab3->ShowWindow(SW_HIDE);


	//GetParentSplitter(this, FALSE)->SetColumnInfo(0, WINCX , 10);

	//GetParentSplitter(this, FALSE)->RecalcLayout();

}


#pragma region 안봐
#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
#pragma endregion 안봐




void CMyForm::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{


	if (IDC_TAB1 == pNMHDR->idFrom) 
	{ 
		int select = m_tab.GetCurSel();

		CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView*		pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	
		pToolView->Change_Tab((TOP_TAB_TYPE)select);

		switch (select) 
		{
		case 0:
			m_pUnitTool_Tab1->ShowWindow(SW_SHOW);
			m_pMapTool_Tab2->ShowWindow(SW_HIDE);
			m_pMapTool_Tab2->m_pMapTool_SubMap.ShowWindow(SW_HIDE);
			m_pTileTool_Tab3->ShowWindow(SW_HIDE);
			break;
		case 1:
			m_pUnitTool_Tab1->ShowWindow(SW_HIDE);
			m_pMapTool_Tab2->ShowWindow(SW_SHOW);
			m_pMapTool_Tab2->m_pMapTool_SubMap.ShowWindow(SW_SHOW);
			m_pTileTool_Tab3->ShowWindow(SW_HIDE);
			break;
		case 2:
			m_pUnitTool_Tab1->ShowWindow(SW_HIDE);
			m_pMapTool_Tab2->ShowWindow(SW_HIDE);
			m_pMapTool_Tab2->m_pMapTool_SubMap.ShowWindow(SW_HIDE);
			m_pTileTool_Tab3->ShowWindow(SW_SHOW);
			break;
		}
	}
	*pResult = 0;
}


void CMyForm::OnDestroy()
{
	CFormView::OnDestroy();

	m_pUnitTool_Tab1->DestroyWindow();
	Safe_Delete(m_pUnitTool_Tab1);

	m_pMapTool_Tab2->DestroyWindow();
	Safe_Delete(m_pMapTool_Tab2);

	m_pTileTool_Tab3->DestroyWindow();
	Safe_Delete(m_pTileTool_Tab3);
	// TODO: Add your message handler code here
}


void CMyForm::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (::IsWindow(m_hWnd))
	{
		CRect rectClient{};
		GetClientRect(&rectClient);

		// 컨트롤들의 ID를 사용하여 위치와 크기를 조정
		CWnd* pCtrl = GetDlgItem(IDC_TAB1);
		if (pCtrl != nullptr)
		{
			pCtrl->MoveWindow(rectClient);

		}

		if (m_pUnitTool_Tab1 != nullptr)
		{
			m_pUnitTool_Tab1->MoveWindow(0, TAB_HEIGHT, rectClient.Width(), rectClient.Height());
		}

		if (m_pMapTool_Tab2 != nullptr)
		{
			m_pMapTool_Tab2->MoveWindow(0, TAB_HEIGHT, rectClient.Width(), rectClient.Height());
		}

		if (m_pTileTool_Tab3 != nullptr)
		{
			m_pTileTool_Tab3->MoveWindow(0, TAB_HEIGHT, rectClient.Width(), rectClient.Height());
		}
	}
	// TODO: Add your message handler code here
}
