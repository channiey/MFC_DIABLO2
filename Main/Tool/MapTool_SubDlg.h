#pragma once

#include "Include.h"

#include "MiniView.h"

// CMapTool_SubDlg dialog

class CMapTool_SubDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapTool_SubDlg)

public:
	CMapTool_SubDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapTool_SubDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL_SUBDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

public:
	CMiniView*	m_pMiniView;
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
