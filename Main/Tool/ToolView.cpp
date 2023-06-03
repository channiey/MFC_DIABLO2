
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"
#include "MyForm.h"
#include "UnitTool_Tab1.h"
#include "TimeMgr.h"
#include "Terrain_Act.h"


#include "UnitTool_Monster.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	CScrollView::OnInitialUpdate();

	//// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 함수
	//// MM_TEXT : 픽셀 단위로 조정하겠다는 옵션
	//// 스크롤 가로 사이즈, 스크롤 세로 사이즈

	if (FAILED(CTextureMgr::Get_Instance()->Create_TexturePath()))
	{
		AfxMessageBox(L"TexturePath Create Failed");
		return;
	}


	SetScrollSizes(MM_TEXT, CSize(WINCX, WINCY));


	//// AfxGetMainWnd : 현재의 메인 창의 주소를 반환하는 함수
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 창의 렉트 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 매개 변수대로 렉트의 정보를 지정하는 함수
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : 현재 view창의 RECT 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float		fRowFrm = float(rcWnd.right - rcMainView.right);
	float		fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	//SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	 // (배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기및 위치 지정 옵션)
	pMainFrm->SetWindowPos(nullptr,  // 순서 변경을 안하기 때문에 NULL
							0, 
							0, 
							int(WINCX + fRowFrm),
							int(WINCY + fColFrm), 
							SWP_NOZORDER);	// 현재 순서를 유지하겠다는 옵션


	if (FAILED(CDevice::Get_Instance()->Initialize()))
	{
		AfxMessageBox(L"Device Create Failed");
		return;
	}


	// 시간 관련
	CTimeMgr::Get_Instance()->Initialize();
	SetTimer(ID_TIMER, 0.001, NULL);

	m_eCurTopTab = TOP_TAB_TYPE::UNIT;
	m_eCurMidTab = MID_TAB_TYPE::MONSTER;

	// 폼뷰 가져오기
	m_pFormView = dynamic_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 1));
}

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	RECT	rc{};
	GetClientRect(&rc);
	int CX = rc.right - rc.left;
	int CY = rc.bottom - rc.top;

	CDevice::Get_Instance()->Render_Begin();

	switch (m_eCurTopTab)
	{
		case TOP_TAB_TYPE::UNIT:
		{
			// Draw Line
			{
				// 라인 굵기 설정
				CDevice::Get_Instance()->Get_Line()->SetWidth(2);

				// 라인으로 사용할 정점 세팅
				D3DXVECTOR2 vList1[] =
				{
					D3DXVECTOR2(CX * 0.5f, 0),
					D3DXVECTOR2(CX * 0.5f, WINCY),
				};
				D3DXVECTOR2 vList2[] =
				{

					D3DXVECTOR2(0, CY * 0.5f),
					D3DXVECTOR2((float)CX, CY * 0.5f),
				};

				CDevice::Get_Instance()->Get_Line()->Begin(); // 라인 그리기 시작 (Render_Begin()처럼)

				CDevice::Get_Instance()->Get_Line()->Draw(vList1, 2, D3DCOLOR_XRGB(255, 0, 0));
				CDevice::Get_Instance()->Get_Line()->Draw(vList2, 2, D3DCOLOR_XRGB(255, 0, 0));

				CDevice::Get_Instance()->Get_Line()->End();// 라인 그리기 종료 (Render_End()처럼)

				if (nullptr == m_pRenderUnit) break;
				D3DXVECTOR3 vCenter{ CX * 0.5f, CY * 0.5f, 0.f };
				m_pRenderUnit->Tool_Render(vCenter);
			}
			break;
		case TOP_TAB_TYPE::TILE:
		{
			// HEEJUNE
			RECT	rc{};
			GetClientRect(&rc);
			int CX = rc.right - rc.left;
			int CY = rc.bottom - rc.top;

			{
				// 라인 굵기 설정
				CDevice::Get_Instance()->Get_Line()->SetWidth(2);

				// 라인으로 사용할 정점 세팅
				D3DXVECTOR2 vList1[] =
				{
					D3DXVECTOR2(CX * 0.5f, 0),
					D3DXVECTOR2(CX * 0.5f, WINCY),
				};
				D3DXVECTOR2 vList2[] =
				{
					D3DXVECTOR2(0, CY * 0.5f),
					D3DXVECTOR2(float(CX), CY * 0.5f),
				};

				CDevice::Get_Instance()->Get_Line()->Begin(); // 라인 그리기 시작 (Render_Begin()처럼)

				CDevice::Get_Instance()->Get_Line()->Draw(vList1, 2, D3DCOLOR_XRGB(255, 0, 0));
				CDevice::Get_Instance()->Get_Line()->Draw(vList2, 2, D3DCOLOR_XRGB(255, 0, 0));

				CDevice::Get_Instance()->Get_Line()->End(); // 라인 그리기 종료 (Render_End()처럼)
			}
			{
				D3DXVECTOR3		vCenter{ CX * 0.5f, CY * 0.5f, 0.f };
				m_pFormView->m_pTileTool_Tab3->Tool_Render(vCenter);
			}
			
		}
		break;
		case TOP_TAB_TYPE::MAP:
		{
			TERRIAN_TYPE eTerrian_Type = static_cast<TERRIAN_TYPE>(m_pFormView->m_pMapTool_Tab2->m_Combo_SelecMap.GetCurSel());
			switch (eTerrian_Type)
			{
			case TERRIAN_TYPE::ACT1:
			{
				if (m_pTerrain_Act1_View != nullptr)
				{
					int iTileX = m_pFormView->m_pMapTool_Tab2->m_iTileX;
					int iTileY = m_pFormView->m_pMapTool_Tab2->m_iTileY;
					SetScrollSizes(MM_TEXT, CSize(iTileX * TILECX, iTileY * TILECY / 2));

					m_pTerrain_Act1_View->Render();
				}
				Render_MapUnit();
			}
				break;
			case TERRIAN_TYPE::ACT2:
			{
				if (m_pTerrain_Act2_View != nullptr)
				{
					int iTileX = m_pFormView->m_pMapTool_Tab2->m_iTileX;
					int iTileY = m_pFormView->m_pMapTool_Tab2->m_iTileY;
					SetScrollSizes(MM_TEXT, CSize(iTileX * TILECX, iTileY * TILECY / 2));

					m_pTerrain_Act2_View->Render();
				}
				Render_MapUnit();
			}
				break;
			case TERRIAN_TYPE::ACT3:
			{
				if (m_pTerrain_Act3_View != nullptr)
				{
					int iTileX = m_pFormView->m_pMapTool_Tab2->m_iTileX;
					int iTileY = m_pFormView->m_pMapTool_Tab2->m_iTileY;
					SetScrollSizes(MM_TEXT, CSize(iTileX * TILECX, iTileY * TILECY / 2));

					m_pTerrain_Act3_View->Render();
				}
				Render_MapUnit();
			}
				break;

			default:
				break;
			}
		}
		break;
		default:
			break;
		}
	}

	CDevice::Get_Instance()->Render_End();
}
void CToolView::OnDestroy()
{

	CScrollView::OnDestroy();




	/* HEEJUNE */

	/* CHAN */
	CTimeMgr::Get_Instance()->Destroy_Instance();


	CTextureMgr::Get_Instance()->Destroy_Instance();
	
	CDevice::Get_Instance()->Destroy_Instance();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

#pragma region 안봐
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


// CToolView 그리기
#pragma endregion 안봐

void CToolView::Render_MapUnit()
{
	if (MAPTOOL_MODE::OBJ == m_pMapToolTab->m_eToolMode && m_pMapToolTab->m_pCurPrefabObj != nullptr)
	{
		CObj* pObj = m_pMapToolTab->m_pCurPrefabObj;

		//D3DXVECTOR3 vLocalPos{ (pObj->m_tInfo.vPos.x * -0.5f), (pObj->m_tInfo.vPos.y * -0.5f),  pObj->m_tInfo.vPos.z };
		D3DXVECTOR3 vLocalPos{ -(pObj->m_tInfo.vPos.x), -(pObj->m_tInfo.vPos.y),  pObj->m_tInfo.vPos.z };
		D3DXVECTOR3 vWorldPos = Get_Mouse() + vLocalPos;
		m_pMapToolTab->m_pCurPrefabObj->Tool_Render(vWorldPos);
	}

	RECT	rc{};
	GetClientRect(&rc);
	int CX = rc.right - rc.left;
	int CY = rc.bottom - rc.top;

	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (auto& iter : m_pMapToolTab->m_vecObjInstances[i])
		{
			if (nullptr == iter) continue;

			/*D3DXVECTOR3 vCenter{ CX * 0.5f, CY * 0.5f, 0.f };
			iter->m_vWorldPos = vCenter;*/
			iter->Render();
		}
	}
}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonDown(nFlags, point);

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		switch (m_eCurTopTab)
		{
		case TOP_TAB_TYPE::UNIT:
			break;
		case TOP_TAB_TYPE::TILE: 
			break;
		case TOP_TAB_TYPE::MAP:
		{
			if (MAPTOOL_MODE::OBJ == m_pMapToolTab->m_eToolMode && m_pMapToolTab->m_pCurPrefabObj != nullptr)
			{
				m_pMapToolTab->Instantiate();
				Invalidate(FALSE);
			}
			TILE* pChangeTile = m_pFormView->m_pMapTool_Tab2->m_tSelectTile;

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (m_pTerrain_Act1_View != nullptr)
				{
					m_pTerrain_Act1_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act2_View != nullptr)
				{
					m_pTerrain_Act2_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act3_View != nullptr)
				{
					m_pTerrain_Act3_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				Invalidate(FALSE);
				
				m_pFormView->m_pMapTool_Tab2->m_pMapTool_SubMap.m_pMiniView->Invalidate(FALSE);
			}
		}
		break;
		default:
			break;
		}



		return;
	}
}

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		switch (m_eCurTopTab)
		{
		case TOP_TAB_TYPE::UNIT:
			switch (m_eCurMidTab)
			{
			case MID_TAB_TYPE::PLAYER:
			{

			}
			break;
			case MID_TAB_TYPE::MONSTER:
			{

			}
			break;
			case MID_TAB_TYPE::ITEM:
			{

			}
			break;
			default:
				break;
			}
			break;
		case TOP_TAB_TYPE::TILE:
		{
			// HEEJUNE
		}
		break;
		case TOP_TAB_TYPE::MAP:
		{
			
			Invalidate(FALSE);

			TILE* pChangeTile = m_pFormView->m_pMapTool_Tab2->m_tSelectTile;

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				if (m_pTerrain_Act1_View != nullptr)
				{
					m_pTerrain_Act1_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act2_View != nullptr)
				{
					m_pTerrain_Act2_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act3_View != nullptr)
				{
					m_pTerrain_Act3_View->Tile_Change({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				Invalidate(FALSE);
				m_pFormView->m_pMapTool_Tab2->m_pMapTool_SubMap.m_pMiniView->Invalidate(FALSE);
			}
			else
			{
				if (m_pTerrain_Act1_View != nullptr)
				{
					m_pTerrain_Act1_View->Tile_LookChange({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act2_View != nullptr)
				{
					m_pTerrain_Act2_View->Tile_LookChange({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				else if (m_pTerrain_Act3_View != nullptr)
				{
					m_pTerrain_Act3_View->Tile_LookChange({ float(point.x + GetScrollPos(0)), float(point.y + GetScrollPos(1)), 0.f }, pChangeTile);
				}
				Invalidate(FALSE);
			}
		}
		break;
		default:
			break;
		}



		return;
	}
}

HRESULT CToolView::Change_Tab(const TOP_TAB_TYPE & _eTopTab, const MID_TAB_TYPE & _eMidTab)
{
	if(TOP_TAB_TYPE::TYPEEND == _eTopTab || MID_TAB_TYPE::TYPEEND == _eMidTab)
		return E_FAIL;

	m_eCurTopTab = _eTopTab;
	m_eCurMidTab = _eMidTab;

	switch (m_eCurTopTab)
	{
	case TOP_TAB_TYPE::UNIT:
	{
		switch (m_eCurMidTab)
		{
		case MID_TAB_TYPE::PLAYER:
		{

		}
		break;
		case MID_TAB_TYPE::MONSTER:
		{

		}
		break;
		case MID_TAB_TYPE::ITEM:
		{

		}
		break;
		default:
			break;
		}
	}
		break;
	case TOP_TAB_TYPE::TILE:
	{
		// HEEJUNE
	}
	break;
	case TOP_TAB_TYPE::MAP:
	{

	}
	break;
	default:
		break;
	}
	Invalidate(FALSE);
	return S_OK;
}


void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	CTimeMgr::Get_Instance()->Update();

	if (m_pDlgUnit->m_bPreviewAniPlay)
		m_pDlgUnit->Update_Ani_Preview();

	if (m_pDlgUnit->m_bAniPlay)
		m_pDlgUnit->Update_Ani();

	CScrollView::OnTimer(nIDEvent);
}
