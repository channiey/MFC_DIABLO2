
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once


/* Together */
#include "Include.h"
#include "Terrain.h"
#include "MapTool_Tab2.h"

class CToolDoc;


/* HEEJUNE */
class CTerrain_Act;
class CMyForm;


/* CHAN */
#include "Unit.h"

class UnitTool_Monster;



class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();

public:
	CTerrain*			m_pTerrain;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

/*-----------------------------------------------------------------------------------------------*/


public: 

/* Together */

	// 탭을 바꿀 때 호출되는 함수 (중간탭은 디폴트 매개변수이므로, 중간탭이 없을경우 상단 탭만 매개변수로 보낸다).
	HRESULT	Change_Tab(const TOP_TAB_TYPE& _eTopTab, const MID_TAB_TYPE& _eMidTab = MID_TAB_TYPE::NONE); 

	TOP_TAB_TYPE m_eCurTopTab;
	MID_TAB_TYPE m_eCurMidTab;

	afx_msg void OnTimer(UINT_PTR nIDEvent);	// 애니메이션 시간 컨트롤을 위한 타임매니저 호출 함수

	CMyForm*		m_pFormView;

#pragma region MapTool

#pragma region Jun
public:

	CTerrain_Act*	 m_pTerrain_Act1_View = nullptr;
	CTerrain_Act*	 m_pTerrain_Act2_View = nullptr;
	CTerrain_Act*	 m_pTerrain_Act3_View = nullptr;
#pragma endregion

#pragma region Chan
	CMapTool_Tab2*	m_pMapToolTab = nullptr;

	void			Render_MapUnit(); // 맵툴에서 현재 선택된 유닛을 렌더링한다.
#pragma endregion




#pragma endregion


/* HEEJUNE */
#pragma region TileTool

	


#pragma endregion


/* CHAN */

#pragma region UnitTool

	CUnit*				m_pRenderUnit = nullptr;  
	UnitTool_Monster*	m_pDlgUnit;


#pragma endregion



};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// 일부가 아닌 전체 맵 타일을 미니맵에 출력해오기