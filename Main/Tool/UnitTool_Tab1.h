#pragma once
#include "afxcmn.h"
#include "UnitTool_Player.h"
#include "UnitTool_Monster.h"
#include "UnitTool_Item.h"
#include "Include.h"
// CUnitTool_Tab1 dialog

class CUnitTool_Tab1 : public CDialogEx
{
	DECLARE_DYNAMIC(CUnitTool_Tab1)

public:
	CUnitTool_Tab1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnitTool_Tab1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	UnitTool_Player* pDlgPlayer;
	UnitTool_Monster* pDlgMonster;
	UnitTool_Item* pDlgItme;

	afx_msg void OnSelchangeTabUnit(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();


	afx_msg void OnSize(UINT nType, int cx, int cy);
};
