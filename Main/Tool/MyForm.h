#pragma once

#include "UnitTool.h"
#include "MapTool.h"
#include "afxcmn.h"

#include "UnitTool_Tab1.h"
#include "MapTool_Tab2.h"
#include "TileTool_Tab3.h"

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

public:
	CFont				m_Font;
	
	CTabCtrl			m_tab; 

	CUnitTool_Tab1*		m_pUnitTool_Tab1;
	CTileTool_Tab3*		m_pTileTool_Tab3;
	CMapTool_Tab2*		m_pMapTool_Tab2;

public:
	virtual void		OnInitialUpdate();
	afx_msg void		OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void		OnDestroy();
	afx_msg void		OnSize(UINT nType, int cx, int cy);
};


