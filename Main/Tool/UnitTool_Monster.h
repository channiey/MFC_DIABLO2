#pragma once


// UnitTool_Monster dialog
#include "Include.h"
#include "ToolView.h"
#include "Unit.h"
#include "afxcmn.h"
#include <atlimage.h>
#include "afxwin.h"
#include "shellapi.h"
class UnitTool_Monster : public CDialogEx
{
	DECLARE_DYNAMIC(UnitTool_Monster)

public:
	UnitTool_Monster(CWnd* pParent = NULL);   // standard constructor
	virtual ~UnitTool_Monster();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIT_MONSTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual						BOOL OnInitDialog();

public:
	// User
	void						Init_Dlg(const CUnit* _pUnit = nullptr); // 모든 컨트롤 초기화

	void						Set_Editability(const bool& _bAlility); // 에딧 컨트롤 편집 가능 여부
	HRESULT						Load_DB(); // 파일에 저장되어있는 유닛들을 가져와 벡터와 리스트 박스에 푸시한다.
	const int					Get_FileNum(const CString& _strPath);

	CToolView*					m_pToolView = nullptr;
	CUnit*						m_pOriginUnit = nullptr;
	CUnit*						m_pCopyUnit = nullptr;


	// Unit List
	vector<CUnit*>				m_vecUnit;		// 유닛 리스트에 올라가있는 유닛 리스트
	CListBox					m_UnitListBox;
	int							m_iCurUnitListIndex = 0; // 현재 유닛 리스트 인덱스

	// Texture List


	// Control Field
	CString						EDIT_NAME;

	float						EDIT_TRANSFORM_POS_X;
	float						EDIT_TRANSFORM_POS_Y;
	float						EDIT_TRANSFORM_POS_Z;
	float						EDIT_TRANSFORM_ROT_X;
	float						EDIT_TRANSFORM_ROT_Y;
	float						EDIT_TRANSFORM_ROT_Z;
	float						EDIT_TRANSFORM_SCL_X;
	float						EDIT_TRANSFORM_SCL_Y;
	float						EDIT_TRANSFORM_SCL_Z;
	float						EDIT_STAT_HP;
	float						EDIT_STAT_AD;
	float						EDIT_STAT_SPEED;

	CSliderCtrl					SLIDE_CTRL_HP;
	CSliderCtrl					SLIDE_CTRL_AD;
	CSliderCtrl					SLIDE_CTRL_SPEED;

	CComboBox					m_ComboUnitType;

	// Control Method
	DECLARE_MESSAGE_MAP()


	// Btn5
	afx_msg void				OnBnClickedButtonMonsterNew();		// New
	afx_msg void				OnBnClickedButtonMonsterInit();		// Init
	afx_msg void				OnBnClickedButtonMonsterDelete();	// Delete
	afx_msg void				OnBnClickedButtonMonsterEdit();		// Edit
	afx_msg void				OnBnClickedButtonMonsterSave();		// Save			유닛이 변경된 사항을 저장한다.
	afx_msg void				OnBnClickedButtonMonsterSaveDb();	// Save DB		DB를 새로 저장한다.
	afx_msg void				OnBnClickedButtonMonsterTexLoad();	// Texture Load 텍스처를 로드할 파일을 탐색기로 연다.

	// Edit
	afx_msg void				OnEnChangeEditMonsterName();
	afx_msg void				OnEnChangeEditMonsterPosX();
	afx_msg void				OnEnChangeEditMonsterPosY();
	afx_msg void				OnEnChangeEditMonsterPosZ();
	afx_msg void				OnEnChangeEditMonsterRotX();
	afx_msg void				OnEnChangeEditMonsterRotY();
	afx_msg void				OnEnChangeEditMonsterRotZ();
	afx_msg void				OnEnChangeEditMonsterSclX();
	afx_msg void				OnEnChangeEditMonsterSclY();
	afx_msg void				OnEnChangeEditMonsterSclZ();

	afx_msg void				OnEnChangeEditMonsterStatHpValue();
	afx_msg void				OnEnChangeEditMonsterStatAdValue();
	afx_msg void				OnEnChangeEditMonsterStatSpeedValue();

	// Scroll
	afx_msg void				OnNMCustomdrawSliderMonsterHp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void				OnNMCustomdrawSliderMonsterAd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void				OnNMCustomdrawSliderMonsterSpeed(NMHDR *pNMHDR, LRESULT *pResult);

	// List 
	afx_msg void				OnLbnSelchangeListMonster();		// 유닛 리스트 항목 변경
	
	// Combo Box
	afx_msg void				OnCbnSelchangeComboMonsterType();
	
#pragma region Animation 
public:
	// Methods
	afx_msg void				OnDropFiles(HDROP hDropInfo);
	afx_msg void				OnLbnSelchangeListMonsterTexture(); // 텍스처 리스트 항목 변경
	afx_msg void				OnBnClickedButtonMonsterAniStop();
	afx_msg void				OnBnClickedButtonMonsterAniPush();
	afx_msg void				OnBnClickedButtonMonsterAniPlay(); // 미리보기
	afx_msg void				OnBnClickedButtonMonsterAniPlaybtn(); // 툴 뷰 
	afx_msg void				OnBnClickedButtonMonsterAniStop2();
	
	afx_msg void				OnEnChangeEditMonsterAniTime();
	afx_msg void				OnCbnSelchangeComboMonsterState();
	afx_msg void				OnCbnSelchangeComboMonsterDir();

	void						Update_Ani_Preview();
	void						Update_Ani();
	void						Empty_MapImg();
	// Fields

	map<CString, CImage*>			m_mapPngImg;	// 텍스처 이미지를 저장한다
	map<CString, CImage*>::iterator m_mapImgIter;
	CString							m_FilePath = L""; // 파일명을 지운 순수 상대 경로
	CListBox						m_TextureListBox;
	CListBox						m_AniListBox;
	CStatic							m_Picture;
	bool							m_bPreviewAniPlay = false;

	float							EDIT_ANI_TIME;
	CComboBox						ANI_COMBO_OBJ_STATE;
	CComboBox						ANI_COMBO_OBJ_DIR;
	bool							m_bAniPlay = true;

	float							m_fAcc = 0.f;

#pragma endregion
};
