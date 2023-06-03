// MapTool_SubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool_SubDlg.h"
#include "afxdialogex.h"
#include "MiniView.h"

// CMapTool_SubDlg dialog

IMPLEMENT_DYNAMIC(CMapTool_SubDlg, CDialog)

CMapTool_SubDlg::CMapTool_SubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL_SUBDLG, pParent)
{

}

CMapTool_SubDlg::~CMapTool_SubDlg()
{
}

void CMapTool_SubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapTool_SubDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMapTool_SubDlg message handlers


BOOL CMapTool_SubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMiniView = new CMiniView;
	m_pMiniView->Create(NULL, L"", WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(20, 40, 540, 290), this, 50001);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMapTool_SubDlg::OnDestroy()
{
	CDialog::OnDestroy();

	Safe_Delete(m_pMiniView);
}


void CMapTool_SubDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rectView;
	m_pMiniView->GetClientRect(&rectView);

	if (rectView.PtInRect(point))
	{
		return;
	}


	CDialog::OnLButtonDown(nFlags, point);
}
