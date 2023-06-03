// UnitTool_Monster.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool_Monster.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "TextureMgr.h"
#include "FileInfo.h"
#include "TimeMgr.h"

CString OBJ_TYPE_STRING[(UINT)OBJ_TYPE::TYPEEND]	{ L"Player", L"Monster", L"Npc", L"Item", L"Terrain", L"Enviornment", L"UI" };
CString OBJ_STATE_STRING[(UINT)OBJ_STATE::TYPEEND]	{ L"stand", L"walk", L"dash", L"attack", L"damage", L"skill", L"die" };
CString OBJ_DIR_STRING[(UINT)OBJ_DIR::TYPEEND]		{ L"UP", L"RIGHT_UP", L"RIGHT", L"RIGHT_DOWN", L"DOWN", L"LEFT_DONN", L"LEFT", L"LEFT_UP" };

// UnitTool_Monster dialog

IMPLEMENT_DYNAMIC(UnitTool_Monster, CDialogEx)

UnitTool_Monster::UnitTool_Monster(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNIT_MONSTER, pParent)
	, EDIT_TRANSFORM_POS_X(1)
	, EDIT_TRANSFORM_POS_Y(1)
	, EDIT_TRANSFORM_POS_Z(1)
	, EDIT_TRANSFORM_ROT_X(0)
	, EDIT_TRANSFORM_ROT_Y(0)
	, EDIT_TRANSFORM_ROT_Z(0)
	, EDIT_TRANSFORM_SCL_X(0)
	, EDIT_TRANSFORM_SCL_Y(0)
	, EDIT_TRANSFORM_SCL_Z(0)
	, EDIT_STAT_HP(0)
	, EDIT_STAT_AD(0)
	, EDIT_STAT_SPEED(0)
	, EDIT_NAME(_T(""))
	, EDIT_ANI_TIME(0)
{

}

UnitTool_Monster::~UnitTool_Monster()
{
	// 텍스처 관련 메모리 해제
	Empty_MapImg();

	// 유닛 관련 메모리 해제
	if (nullptr != m_pOriginUnit)
		Safe_Delete(m_pOriginUnit);

	for_each(m_vecUnit.begin(), m_vecUnit.end(), CDeleteObj());

	m_vecUnit.clear();
	m_vecUnit.shrink_to_fit();
}

void UnitTool_Monster::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MONSTER_POS_X, EDIT_TRANSFORM_POS_X);
	DDX_Text(pDX, IDC_EDIT_MONSTER_POS_Y, EDIT_TRANSFORM_POS_Y);
	DDX_Text(pDX, IDC_EDIT_MONSTER_POS_Z, EDIT_TRANSFORM_POS_Z);
	DDX_Text(pDX, IDC_EDIT_MONSTER_ROT_X, EDIT_TRANSFORM_ROT_X);
	DDX_Text(pDX, IDC_EDIT_MONSTER_ROT_Y, EDIT_TRANSFORM_ROT_Y);
	DDX_Text(pDX, IDC_EDIT_MONSTER_ROT_Z, EDIT_TRANSFORM_ROT_Z);
	DDX_Text(pDX, IDC_EDIT_MONSTER_SCL_X, EDIT_TRANSFORM_SCL_X);
	DDX_Text(pDX, IDC_EDIT_MONSTER_SCL_Y, EDIT_TRANSFORM_SCL_Y);
	DDX_Text(pDX, IDC_EDIT_MONSTER_SCL_Z, EDIT_TRANSFORM_SCL_Z);
	DDX_Text(pDX, IDC_EDIT_MONSTER_STAT_HP_VALUE, EDIT_STAT_HP);
	DDX_Text(pDX, IDC_EDIT_MONSTER_STAT_AD_VALUE, EDIT_STAT_AD);
	DDX_Text(pDX, IDC_EDIT_MONSTER_STAT_SPEED_VALUE, EDIT_STAT_SPEED);
	DDX_Control(pDX, IDC_SLIDER_MONSTER_HP, SLIDE_CTRL_HP);
	DDX_Control(pDX, IDC_SLIDER_MONSTER_AD, SLIDE_CTRL_AD);
	DDX_Control(pDX, IDC_SLIDER_MONSTER_SPEED, SLIDE_CTRL_SPEED);
	DDX_Text(pDX, IDC_EDIT_MONSTER_NAME, EDIT_NAME);
	DDX_Control(pDX, IDC_LIST4, m_TextureListBox);
	DDX_Control(pDX, IDC_STATIC_MONSTER_TEXTURE, m_Picture);
	DDX_Control(pDX, IDC_LIST_MONSTER, m_UnitListBox);
	DDX_Control(pDX, IDC_COMBO_MONSTER_TYPE, m_ComboUnitType);
	DDX_Text(pDX, IDC_EDIT_MONSTER_ANI_TIME, EDIT_ANI_TIME);
	DDX_Control(pDX, IDC_COMBO_MONSTER_STATE, ANI_COMBO_OBJ_STATE);
	DDX_Control(pDX, IDC_COMBO_MONSTER_DIR, ANI_COMBO_OBJ_DIR);
	DDX_Control(pDX, IDC_LIST1_MONSTER_ANI_DB, m_AniListBox);
}

BOOL UnitTool_Monster::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 다이얼로그 컨트롤들을 세팅한다.

	SLIDE_CTRL_HP.SetRange(0, (int)UNIT_MAX_HP);
	SLIDE_CTRL_AD.SetRange(0, (int)UNIT_MAX_AD);
	SLIDE_CTRL_SPEED.SetRange(0, (int)UNIT_MAX_SPEED);

	CMainFrame*		pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pToolView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));

	m_pToolView->m_pDlgUnit = this;


	EDIT_ANI_TIME = 0.1f;


	DragAcceptFiles(TRUE);
	UpdateData(FALSE);

	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
		m_ComboUnitType.AddString(OBJ_TYPE_STRING[i]);

	for (int i = 0; i < (UINT)OBJ_STATE::TYPEEND; ++i)
		ANI_COMBO_OBJ_STATE.AddString(OBJ_STATE_STRING[i]);

	for (int i = 0; i < (UINT)OBJ_DIR::TYPEEND; ++i)
		ANI_COMBO_OBJ_DIR.AddString(OBJ_DIR_STRING[i]);

	// 저장된 DB를 불러온다
	if (FAILED(Load_DB()))
	{
		AfxMessageBox(L"Load Unit DB Failed");
		return E_FAIL;
	}

	

	if (!m_vecUnit.empty())
		m_UnitListBox.SetCurSel(0);

	m_pToolView->Invalidate(FALSE);


	return TRUE;  
}



BEGIN_MESSAGE_MAP(UnitTool_Monster, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_POS_X, &UnitTool_Monster::OnEnChangeEditMonsterPosX)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_POS_Y, &UnitTool_Monster::OnEnChangeEditMonsterPosY)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_POS_Z, &UnitTool_Monster::OnEnChangeEditMonsterPosZ)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_ROT_X, &UnitTool_Monster::OnEnChangeEditMonsterRotX)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_ROT_Y, &UnitTool_Monster::OnEnChangeEditMonsterRotY)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_ROT_Z, &UnitTool_Monster::OnEnChangeEditMonsterRotZ)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_SCL_X, &UnitTool_Monster::OnEnChangeEditMonsterSclX)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_SCL_Y, &UnitTool_Monster::OnEnChangeEditMonsterSclY)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_SCL_Z, &UnitTool_Monster::OnEnChangeEditMonsterSclZ)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_STAT_HP_VALUE, &UnitTool_Monster::OnEnChangeEditMonsterStatHpValue)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_STAT_AD_VALUE, &UnitTool_Monster::OnEnChangeEditMonsterStatAdValue)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_STAT_SPEED_VALUE, &UnitTool_Monster::OnEnChangeEditMonsterStatSpeedValue)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MONSTER_HP, &UnitTool_Monster::OnNMCustomdrawSliderMonsterHp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MONSTER_AD, &UnitTool_Monster::OnNMCustomdrawSliderMonsterAd)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_MONSTER_SPEED, &UnitTool_Monster::OnNMCustomdrawSliderMonsterSpeed)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_NAME, &UnitTool_Monster::OnEnChangeEditMonsterName)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_INIT, &UnitTool_Monster::OnBnClickedButtonMonsterInit)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_NEW, &UnitTool_Monster::OnBnClickedButtonMonsterNew)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_DELETE, &UnitTool_Monster::OnBnClickedButtonMonsterDelete)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_EDIT, &UnitTool_Monster::OnBnClickedButtonMonsterEdit)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_SAVE, &UnitTool_Monster::OnBnClickedButtonMonsterSave)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_TEX_LOAD, &UnitTool_Monster::OnBnClickedButtonMonsterTexLoad)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_SAVE_DB, &UnitTool_Monster::OnBnClickedButtonMonsterSaveDb)

	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST_MONSTER_TEXTURE, &UnitTool_Monster::OnLbnSelchangeListMonsterTexture)
	ON_LBN_SELCHANGE(IDC_LIST_MONSTER, &UnitTool_Monster::OnLbnSelchangeListMonster)

	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_TYPE, &UnitTool_Monster::OnCbnSelchangeComboMonsterType)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ANI_PLAY, &UnitTool_Monster::OnBnClickedButtonMonsterAniPlay)
	ON_EN_CHANGE(IDC_EDIT_MONSTER_ANI_TIME, &UnitTool_Monster::OnEnChangeEditMonsterAniTime)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ANI_STOP, &UnitTool_Monster::OnBnClickedButtonMonsterAniStop)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ANI_PUSH, &UnitTool_Monster::OnBnClickedButtonMonsterAniPush)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_STATE, &UnitTool_Monster::OnCbnSelchangeComboMonsterState)
	ON_CBN_SELCHANGE(IDC_COMBO_MONSTER_DIR, &UnitTool_Monster::OnCbnSelchangeComboMonsterDir)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ANI_PLAYBTN, &UnitTool_Monster::OnBnClickedButtonMonsterAniPlaybtn)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER_ANI_STOP_2, &UnitTool_Monster::OnBnClickedButtonMonsterAniStop2)
END_MESSAGE_MAP()


#pragma region 트랜스폼, 스탯 컨트롤 함수


void UnitTool_Monster::OnEnChangeEditMonsterPosX()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vPos.x = EDIT_TRANSFORM_POS_X;


	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnEnChangeEditMonsterPosY()
{
	if (nullptr == m_pCopyUnit) return;

	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vPos.y = EDIT_TRANSFORM_POS_Y;


	m_pToolView->Invalidate(FALSE);

}


void UnitTool_Monster::OnEnChangeEditMonsterPosZ()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vPos.z = EDIT_TRANSFORM_POS_Z;


	m_pToolView->Invalidate(FALSE);

}


void UnitTool_Monster::OnEnChangeEditMonsterRotX()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vRot.x = EDIT_TRANSFORM_ROT_X;


	m_pToolView->Invalidate(FALSE);

}


void UnitTool_Monster::OnEnChangeEditMonsterRotY()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vRot.y = EDIT_TRANSFORM_ROT_Y;


	m_pToolView->Invalidate(FALSE);

}


void UnitTool_Monster::OnEnChangeEditMonsterRotZ()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vRot.z = D3DXToRadian(EDIT_TRANSFORM_ROT_Z);


	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnEnChangeEditMonsterSclX()
{
	if (nullptr == m_pCopyUnit) return;



	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vSize.x = EDIT_TRANSFORM_SCL_X;


	m_pToolView->Invalidate(FALSE);


}


void UnitTool_Monster::OnEnChangeEditMonsterSclY()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vSize.y = EDIT_TRANSFORM_SCL_Y;


	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnEnChangeEditMonsterSclZ()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tInfo.vSize.z = EDIT_TRANSFORM_SCL_Z;


	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnEnChangeEditMonsterStatHpValue()
{
	if (nullptr == m_pCopyUnit) return;

	UpdateData(TRUE);

	m_pCopyUnit->m_tStat.iHP = (int)EDIT_STAT_HP;

	SLIDE_CTRL_HP.SetPos((int)EDIT_STAT_HP);
	
	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnEnChangeEditMonsterStatAdValue()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tStat.iAD = (int)EDIT_STAT_AD;
	
	SLIDE_CTRL_AD.SetPos(int(EDIT_STAT_AD));

	m_pToolView->Invalidate(FALSE);

}


void UnitTool_Monster::OnEnChangeEditMonsterStatSpeedValue()
{
	if (nullptr == m_pCopyUnit) return;


	UpdateData(TRUE);

	m_pCopyUnit->m_tStat.iSpeed = (int)EDIT_STAT_SPEED;

	SLIDE_CTRL_SPEED.SetPos(int(EDIT_STAT_SPEED));

	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnNMCustomdrawSliderMonsterHp(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (nullptr == m_pCopyUnit) return;


	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int iScroll = SLIDE_CTRL_HP.GetPos();
	
	EDIT_STAT_HP = (float)iScroll;

	m_pCopyUnit->m_tStat.iHP = (int)EDIT_STAT_HP;

	UpdateData(FALSE);

	*pResult = 0;
}


void UnitTool_Monster::OnNMCustomdrawSliderMonsterAd(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (nullptr == m_pCopyUnit) return;


	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int iScroll = SLIDE_CTRL_AD.GetPos();
	
	EDIT_STAT_AD = (float)iScroll;

	m_pCopyUnit->m_tStat.iAD = (int)EDIT_STAT_AD;

	UpdateData(FALSE);

	*pResult = 0;
}


void UnitTool_Monster::OnNMCustomdrawSliderMonsterSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (nullptr == m_pCopyUnit) return;


	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	int iScroll = SLIDE_CTRL_SPEED.GetPos();

	EDIT_STAT_SPEED = (float)iScroll;

	m_pCopyUnit->m_tStat.iSpeed = (int)EDIT_STAT_SPEED;

	UpdateData(FALSE);

	*pResult = 0;
}


void UnitTool_Monster::OnEnChangeEditMonsterName()
{
	if (nullptr == m_pCopyUnit) return;

	UpdateData(TRUE);
	m_pCopyUnit->m_strName = EDIT_NAME;

	m_UnitListBox.DeleteString(m_iCurUnitListIndex);
	m_UnitListBox.InsertString(m_iCurUnitListIndex, m_pCopyUnit->m_strName);



}

#pragma endregion


void UnitTool_Monster::Init_Dlg(const CUnit* _pUnit)
{
	// Edit 세팅
	EDIT_NAME = _pUnit->m_strName;

	EDIT_TRANSFORM_POS_X = _pUnit->m_tInfo.vPos.x;
	EDIT_TRANSFORM_POS_Y = _pUnit->m_tInfo.vPos.y;
	EDIT_TRANSFORM_POS_Z = _pUnit->m_tInfo.vPos.z;

	EDIT_TRANSFORM_ROT_X = _pUnit->m_tInfo.vRot.x;
	EDIT_TRANSFORM_ROT_Y = _pUnit->m_tInfo.vRot.y;
	EDIT_TRANSFORM_ROT_Z = _pUnit->m_tInfo.vRot.z;

	EDIT_TRANSFORM_SCL_X = _pUnit->m_tInfo.vSize.x;
	EDIT_TRANSFORM_SCL_Y = _pUnit->m_tInfo.vSize.y;
	EDIT_TRANSFORM_SCL_Z = _pUnit->m_tInfo.vSize.z;

	// 슬라이드 세팅
	SLIDE_CTRL_HP.SetPos(_pUnit->m_tStat.iHP);
	SLIDE_CTRL_AD.SetPos(_pUnit->m_tStat.iAD);
	SLIDE_CTRL_SPEED.SetPos(_pUnit->m_tStat.iSpeed);
	EDIT_STAT_HP		= (float)_pUnit->m_tStat.iHP;
	EDIT_STAT_AD		= (float)_pUnit->m_tStat.iAD;
	EDIT_STAT_SPEED		= (float)_pUnit->m_tStat.iSpeed;

	if (_pUnit->m_eType == OBJ_TYPE::TYPEEND)
		m_ComboUnitType.SetCurSel(-1);
	else
		m_ComboUnitType.SetCurSel((UINT)_pUnit->m_eType);

	// 텍스처 세팅
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});

	m_mapPngImg.clear();

	// 텍스처 리스트 박스 지우기
	m_TextureListBox.ResetContent();
	m_AniListBox.ResetContent();

	m_bPreviewAniPlay = false;

	//m_bAniPlay = false;

	UpdateData(FALSE);
}

void UnitTool_Monster::Set_Editability(const bool & _bAlility)
{
	
}


void UnitTool_Monster::OnBnClickedButtonMonsterInit() // 다이얼로그에서 초기화 버튼을 눌렀을 때
{
	if (nullptr == m_pCopyUnit)
	{
		AfxMessageBox(L"Not Create Unit");
		return;
	}


	m_pCopyUnit->Initialize();
	Init_Dlg(m_pCopyUnit);
	m_UnitListBox.DeleteString(m_iCurUnitListIndex);
	m_UnitListBox.InsertString(m_iCurUnitListIndex, m_pCopyUnit->m_strName);

	m_pToolView->Invalidate(FALSE);
}

void UnitTool_Monster::OnBnClickedButtonMonsterNew()
{
	// 새로운 객체 생성
	if (nullptr != m_pOriginUnit)
		Safe_Delete(m_pOriginUnit);

	m_pOriginUnit = new CUnit();
	m_pOriginUnit->Initialize();

	// 편집용으로 사용할 객체 복사 생성
	m_pCopyUnit = new CUnit(*m_pOriginUnit);
	m_pCopyUnit->Initialize();
	m_pCopyUnit->m_pCurAni = new ANIMATION;
	// 툴뷰에 유닛 세팅'
	m_pToolView->m_pRenderUnit = m_pCopyUnit;

	// 리스트 및 벡터 추가
	/*m_UnitListBox.SetCurSel(m_vecUnit.size());
	m_iCurUnitListIndex = m_vecUnit.size();*/

	m_UnitListBox.AddString(m_pCopyUnit->m_strName);
	m_vecUnit.push_back(m_pCopyUnit);

	// 다이얼로그 초기화
	Init_Dlg(m_pCopyUnit);

	UpdateData(FALSE);

	m_pToolView->Invalidate();
}


void UnitTool_Monster::OnBnClickedButtonMonsterDelete()
{
	for (vector<CUnit*>::iterator iter = m_vecUnit.begin(); iter != m_vecUnit.end(); ++iter)
	{
		if (*iter == m_pCopyUnit)
		{
			m_vecUnit.erase(iter);
			m_UnitListBox.DeleteString(m_iCurUnitListIndex);

			Safe_Delete(m_pOriginUnit);
			m_pOriginUnit = nullptr;

			Safe_Delete(m_pCopyUnit);
			m_pCopyUnit = nullptr;
			
			m_pToolView->m_pRenderUnit = nullptr;

			m_pToolView->Invalidate(FALSE);
			return;
		}
	}
}


void UnitTool_Monster::OnBnClickedButtonMonsterEdit()
{
	if (nullptr == m_pCopyUnit)
	{
		AfxMessageBox(L"Not Create Unit");
		return;
	}
}


void UnitTool_Monster::OnBnClickedButtonMonsterSave()
{
	//C:\Users\ChanYoon\Desktop\Programming\Tasks\Team\Jusin_TeamAssignment_D2D_2\ProjectFIle\Main\Data\01.Object

	// 원본 유닛에 편집된 유닛을 대입한다.

	if (nullptr == m_pCopyUnit)
	{
		AfxMessageBox(L"Not Create Unit");
		return;
	}

	m_pOriginUnit = m_pCopyUnit;
}

HRESULT UnitTool_Monster::Load_DB()
{
	HANDLE	hFile = CreateFile(L"../Data/01.Object/Player_Monster_Pivot_Origin_Recent.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//Player_Monster_Pivot_Origin_Recent.dat
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;
	CUnit*	pUnit = nullptr;
	while (true)
	{
		pUnit = new CUnit;
		
		// 문자열 관련 세팅
		{
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				pName = nullptr;
				Safe_Delete(pUnit);
				break;
			}

			pUnit->m_strName = pName;
			delete[]pName;
			pName = nullptr;
		}

		// 애니메이션 관련 세팅
		{
			// Animation Set
			ANIMATION* pAni = new ANIMATION;

			pAni->bLoop = false;
			pAni->iCurFrame = 0;
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND], pAni });
			pUnit->m_pCurAni = pAni;
		}

		ReadFile(hFile, &(pUnit->m_tInfo)				, sizeof(INFO)		, &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_tStat)				, sizeof(STAT)		, &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_eType)				, sizeof(OBJ_TYPE)	, &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_eState)				, sizeof(OBJ_STATE)	, &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_pCurAni->fSecondPerFrame), sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_pCurAni->iMaxFrame), sizeof(int), &dwByte, nullptr);
		/*ReadFile(hFile, &(pUnit->m_strObjKey)			, sizeof(CString)	, &dwByte, nullptr);
		ReadFile(hFile, &(pUnit->m_strStateKey)			, sizeof(CString)	, &dwByte, nullptr);*/

		// Animation Set
		
		pUnit->m_strObjKey = pUnit->m_strName;
		pUnit->m_strStateKey = OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] + L"_8";
		pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] , pUnit->m_pCurAni });

		// Path Set
		CString strAniPath = L"../Texture/Object";

		if (OBJ_TYPE::PLAYER == pUnit->m_eType)
			strAniPath += L"/Player/";
		else if (OBJ_TYPE::MONSTER == pUnit->m_eType)
			strAniPath += L"/Monster/";

		strAniPath += pUnit->m_strObjKey;
		strAniPath += L"/";
		strAniPath += pUnit->m_strStateKey;
		
		int iFileNum = Get_FileNum(strAniPath); // 폴더에 있는 파일의 개수를 구한다.
		
		strAniPath += L"/%d.png";

		if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(strAniPath, TEX_MULTI, pUnit->m_strObjKey, pUnit->m_strStateKey, iFileNum)))
		{
			AfxMessageBox(L"Texture Create Failed");
			return E_FAIL;
		}

	
		

		m_vecUnit.push_back(pUnit);
		m_UnitListBox.AddString(pUnit->m_strName);
	}

	if (!m_vecUnit.empty())
	{
		m_pCopyUnit = *(m_vecUnit.begin());
		m_bAniPlay = true;
		m_pToolView->m_pRenderUnit = m_pCopyUnit;
		Init_Dlg(m_pCopyUnit);
	}
	CloseHandle(hFile);
	
	return S_OK;
}

const int UnitTool_Monster::Get_FileNum(const CString& _strPath)
{

	int count = 0;
	CFileFind finder; 

	BOOL bworking = finder.FindFile(_strPath + "/*.*");

	while (bworking)
	{
		bworking = finder.FindNextFile();
		if (finder.IsDots())
		{
			continue;
		}
		count++;
	}
	finder.Close();

	return count;
}

void UnitTool_Monster::OnBnClickedButtonMonsterSaveDb()
{
	// 다이얼로그 생성
	CFileDialog		Dlg(FALSE,	// TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정	
		L"dat", // defaule 파일 확장자명
		L"*.dat", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복파일 저장 시 경고메세지 띄우기)
		L"Data Files(*.dat) | *.dat||",  // 대화 상자에 표시될 파일 형식 '콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||'
		this); // 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"..\\Data\\01.Object");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		DWORD	dwStrByte = 0;

		for (auto& iter : m_vecUnit)
		{
			dwStrByte = sizeof(TCHAR) * (iter->m_strName.GetLength() + 1);
			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, iter->m_strName.GetString(), dwStrByte, &dwByte, nullptr);

			WriteFile(hFile, &(iter->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_tStat), sizeof(STAT), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_eType), sizeof(OBJ_TYPE), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_eState), sizeof(OBJ_STATE), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_pCurAni->fSecondPerFrame), sizeof(float), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_pCurAni->iMaxFrame), sizeof(int), &dwByte, nullptr);

		/*	WriteFile(hFile, &(iter->m_strObjKey), sizeof(CString), &dwByte, nullptr);
			WriteFile(hFile, &(iter->m_strStateKey), sizeof(CString), &dwByte, nullptr);*/
		}

		CloseHandle(hFile);
	}
}




void UnitTool_Monster::OnDropFiles(HDROP hDropInfo)
{
	Empty_MapImg();

	UpdateData(TRUE);

	CDialogEx::OnDropFiles(hDropInfo);

	TCHAR		szFilePath[MAX_PATH] = L"";
	TCHAR		szFileName[MIN_STR] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0); 


	// 파일명을 지운 순수 상대 경로 추출 -> 유닛에게 전달
	{
		DragQueryFile(hDropInfo, 0, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath); // 상대경로로 만들어준다.
		CString	strFileName = PathFindFileName(strRelative); // 파일 이름만 추출하는 함수

		TCHAR	szFilePath[MAX_PATH] = L"";
		TCHAR	szFileName[MIN_STR] = L"";

		lstrcpy(szFilePath, strRelative.GetString());
		PathRemoveFileSpec(szFilePath);

		m_FilePath = szFilePath; // 파일명을 지운 순수 상대 경로 추출

		//m_pCopyUnit->m_strRelativePath = m_FilePath;
	}


	// 텍스처 리스트에 추가 
	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString strRelative = CFileInfo::ConvertRelativePath(szFilePath); // 상대경로로 만들어준다.
		CString	strFileName = PathFindFileName(strRelative); // 파일 이름만 추출하는 함수

		lstrcpy(szFileName, strFileName.GetString());
		PathRemoveExtension(szFileName); // 파일 이름에서 확장자를 지우는 함수

		strFileName = szFileName; // 대입함으로써 문자열에서 스트링으로 변환

								  // 해당 이미지의 파일명(확장자 제외)을 키로, 이미지를 밸류로 맵에 추가한다

		auto	iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*	pPngImg = new CImage;
			pPngImg->Load(strRelative);

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_TextureListBox.AddString(szFileName); // 리스트에 추가
		}

	}
	m_mapImgIter = m_mapPngImg.begin();

	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_Picture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*(m_mapImgIter->second)).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정

	ReleaseDC(dc);
	UpdateData(FALSE);
}


void UnitTool_Monster::OnLbnSelchangeListMonsterTexture()
{
	UpdateData(TRUE);

	CString			strSelectName;

	int	iSelect = m_TextureListBox.GetCurSel(); // 리스트 컨트롤 내에서 현재 선택된 항목의 이름에서 정수 값만 가져온다.

	if (LB_ERR == iSelect)
		return;

	m_TextureListBox.GetText(iSelect, strSelectName); // 선택된 항목의 이름을 가져온다.

	auto iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	//m_Picture.SetBitmap(*(iter->second));

	CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_Picture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_Picture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
	(*(iter->second)).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
	ReleaseDC(dc);

	
	m_pToolView->Invalidate(FALSE);

	UpdateData(FALSE);
}


void UnitTool_Monster::OnBnClickedButtonMonsterTexLoad()
{
	//ShellExecute(NULL, L"open", L"explorer", L"\\Texture\\Object", NULL, SW_SHOWNORMAL);

	return;
	CFileDialog		Dlg(FALSE,	// TRUE(열기), FALSE(다른 이름으로 저장) 모드 지정	
		L"png", // defaule 파일 확장자명
		L"*.png", // 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(읽기전용 체크박스 숨김), OFN_OVERWRITEPROMPT(중복파일 저장 시 경고메세지 띄우기)
		L"Data Files(*.png) | *.png||",  // 대화 상자에 표시될 파일 형식 '콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||'
		this); // 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"\\Texture\\Object");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CloseHandle(hFile);
	}
}


void UnitTool_Monster::OnLbnSelchangeListMonster()
{
	m_iCurUnitListIndex = m_UnitListBox.GetCurSel();

	if (0 > m_iCurUnitListIndex)
	{
		AfxMessageBox(L"Index Range Error");
		return;
	}

	if (nullptr == m_vecUnit[m_iCurUnitListIndex])
	{
		AfxMessageBox(L"None Selected Unit In Unit List");
		return;
	}
	m_pCopyUnit = m_vecUnit[m_iCurUnitListIndex];

	m_pToolView->m_pRenderUnit = m_pCopyUnit;

	Init_Dlg(m_pCopyUnit); // 선택된 항목의 유닛으로 다이얼로그를 세팅한다.
	
	m_pToolView->Invalidate(FALSE);
}



void UnitTool_Monster::OnCbnSelchangeComboMonsterType()
{
	m_pCopyUnit->m_eType = (OBJ_TYPE)m_ComboUnitType.GetCurSel();
}


void UnitTool_Monster::OnBnClickedButtonMonsterAniPlay()
{
	if (m_mapPngImg.empty())
	{
		AfxMessageBox(L"Sprite List is Empty");
		return;
	}
	m_bPreviewAniPlay = true;
	if (m_mapImgIter == m_mapPngImg.end())
		m_mapImgIter = m_mapPngImg.begin();
}

void UnitTool_Monster::Update_Ani_Preview()
{
	// 현재 맵에 저장되어 있는 이미지를 초당 프레임수에 1번 렌더한다.
	m_fAcc += CTimeMgr::Get_Instance()->Get_TimeDelta();

	if (m_fAcc > EDIT_ANI_TIME)
	{
		m_fAcc = 0.f;

		if (m_mapImgIter != m_mapPngImg.end())
			++m_mapImgIter;

		if (m_mapImgIter == m_mapPngImg.end())
		{
			CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
			m_Picture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
			CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
			dc = m_Picture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
			(*(m_mapPngImg.begin()->second)).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
			ReleaseDC(dc);
			m_bPreviewAniPlay = false;
			return;
		}
		CRect rect;//픽쳐 컨트롤의 크기를 저장할 CRect 객체
		m_Picture.GetWindowRect(rect);//GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
		CDC* dc; //픽쳐 컨트롤의 DC를 가져올  CDC 포인터
		dc = m_Picture.GetDC(); //픽쳐 컨트롤의 DC를 얻는다.
		(*(m_mapImgIter->second)).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//이미지를 픽쳐 컨트롤 크기로 조정
		ReleaseDC(dc);
	}
}

void UnitTool_Monster::Empty_MapImg()
{
	for_each(m_mapPngImg.begin(), m_mapPngImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});

	m_mapPngImg.clear();
}


void UnitTool_Monster::OnEnChangeEditMonsterAniTime()
{
	UpdateData(TRUE);
}


void UnitTool_Monster::OnBnClickedButtonMonsterAniStop()
{
	m_bPreviewAniPlay = false;
}


void UnitTool_Monster::OnBnClickedButtonMonsterAniPush()
{
	if (m_mapPngImg.empty())
	{
		AfxMessageBox(L"Sprite List is Empty");
		return;
	}

	if (nullptr == m_pCopyUnit)
	{
		AfxMessageBox(L"Not Create Unit");
		return;
	}

	ANIMATION* pAni = new ANIMATION;


	pAni->bLoop = true;
	pAni->fSecondPerFrame = EDIT_ANI_TIME;
	pAni->iCurFrame = 0;
	pAni->iMaxFrame = m_mapPngImg.size() - 1;

	m_pCopyUnit->m_mapAni.insert({ OBJ_STATE_STRING[ANI_COMBO_OBJ_STATE.GetCurSel()], pAni });
	m_pCopyUnit->m_pCurAni = pAni;

	m_AniListBox.AddString(OBJ_STATE_STRING[ANI_COMBO_OBJ_STATE.GetCurSel()]);

	CString strPath = m_FilePath + L"\\%d.png";

	CString	strStateDir = OBJ_STATE_STRING[(UINT)ANI_COMBO_OBJ_STATE.GetCurSel()] + L"_";
	CString strDir;
	int iCurSel = ANI_COMBO_OBJ_DIR.GetCurSel();
	strDir.Format(_T("%d"), iCurSel * 2);
	strStateDir += strDir;

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(strPath, TEX_MULTI, m_pCopyUnit->m_strName, strStateDir, m_mapPngImg.size())))
	{
		AfxMessageBox(L"Texture Create Failed");
		return;
	}

	m_pCopyUnit->m_strObjKey = m_pCopyUnit->m_strName;
	m_pCopyUnit->m_strStateKey = strStateDir;
	m_pToolView->Invalidate(FALSE);
}


void UnitTool_Monster::OnCbnSelchangeComboMonsterState()
{
	UpdateData(TRUE);
}


void UnitTool_Monster::OnCbnSelchangeComboMonsterDir()
{
	UpdateData(TRUE);
}


void UnitTool_Monster::OnBnClickedButtonMonsterAniPlaybtn()
{
	m_pCopyUnit->m_bPlay = true;
	m_bAniPlay = true;
}

void UnitTool_Monster::Update_Ani()
{
	m_pCopyUnit->Move_Frame();
	m_pToolView->Invalidate(FALSE);

}

void UnitTool_Monster::OnBnClickedButtonMonsterAniStop2()
{
	m_pCopyUnit->m_bPlay = false;
	m_pCopyUnit->m_pCurAni->iCurFrame = 0;
	m_bAniPlay = false;
	m_pToolView->Invalidate(FALSE);
}
