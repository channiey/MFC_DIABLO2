#pragma once

#include "MapTool_SubDlg.h"
#include "Include.h"
#include "afxwin.h"
#include "afxcmn.h"

// CMapTool_Tab2 dialog
class CMyForm;
class CToolView;
class CTerrain_Act;
class CTerrain_Act;
class CTerrain_Act;
class CObj;
class CUnit;

class CMapTool_Tab2 : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool_Tab2)

public:
	CMapTool_Tab2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapTool_Tab2();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

#pragma region ����

	MAPTOOL_MODE			m_eToolMode; // ���� ��Ʈ���ϰ� �ִ� ���� Map����, Tile���� Obj����

#pragma endregion

#pragma region Jun
public:
	//��
	afx_msg void			OnCombo_ChangeActMap();				// �޺� : �� Act���� �Լ��� Terrian ��ü ����
	afx_msg void			OnButton_SaveMap();					// ��ư : ���� ��Ʈ�� �ش��ϴ� �� ���̺�
	afx_msg void			OnButton_LoadMap();					// ��ư : ���� ��Ʈ�� �ش��ϴ� �ʼ��̺� ���� �ε�
	afx_msg void			OnButton_CreateMap();				// ��ư : Row, Col�� ���� Ÿ�ϸ� ����:
	afx_msg void			OnRadio_MiniView_Hide();			// ���� : �̴Ϻ� ����
	afx_msg void			OnRadio_MiniView_Show();			// ���� : �̴Ϻ� �ѱ�
	afx_msg void			OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); // ��ũ����Ʈ�ѷ� Row, Col ����
	afx_msg void			OnEdit_ChangeRow();					// ����â���� Row ��ũ����Ʈ�� ����
	afx_msg void			OnEdit_ChangeCol();					// ����â���� Col ��ũ����Ʈ�� ����


	//Ÿ��
	afx_msg void			OnList_Tile();						// ����Ʈ�ڽ� : ����Ʈ Ŭ���� �ش� ����Ʈ�� Ÿ�������� m_tSelectTile�� ����
	afx_msg void			OnButton_ReloadTile();				// ��ư : ����Ʈ�ڽ��� ����� Ÿ��DB���� �ٽ� �ҷ���
	afx_msg void			OnCombo_ChangeAct();				// �޺� : ��Ʈ ����� �ش� ��Ʈ Ÿ�Ϸ� ����
	afx_msg void			OnList_TileReset();					// ����Ʈ�ڽ� : ����Ŭ���� ���� ���õ� Ÿ�� ����

private:
	void					Load_TileList();					// ����� Ÿ��DB �Լ�
	void					Sort_TileList(TERRIAN_TYPE _eType); // Ÿ�ϸ���Ʈ�� ����� Sort�ϴ� �Լ�

public:
	CListBox				m_ListTile;
	CStatic					m_Texture_Tile;
	CComboBox				m_ComboTile;
	CComboBox				m_Combo_SelecMap;
	CButton					m_Radio_Mini[2];
	CSliderCtrl				m_Slide_Row;
	CSliderCtrl				m_Slide_Col;

public:
	CMapTool_SubDlg			m_pMapTool_SubMap;					// ������̾�α� ��ü

	CToolView*				m_pMainView = nullptr;
	CMyForm*				m_pFormView = nullptr;

	CTerrain_Act*			m_pTerrain_Act1 = nullptr;
	CTerrain_Act*			m_pTerrain_Act2 = nullptr;
	CTerrain_Act*			m_pTerrain_Act3 = nullptr;

	UINT					m_iTileX;							// �� Row
	UINT					m_iTileY;							// �� Col
	
	vector<TILE*>			m_vecTile;							// Ÿ��DB ����� ����
	TILE*					m_tSelectTile	= nullptr;			// ���� ���õ� Ÿ��
	UINT					m_iTileDrawID;						// DrawID�� �����ڽ�
	CString					m_strTileType;						// Ÿ�� Ÿ�Կ� �����ڽ�
	float					m_fTileDmg;							// Ÿ�� �������� �����ڽ�
	
	map<CString, CImage*>	m_TilePngImg;						// Ÿ���̹��� ����� ��

#pragma endregion

#pragma region Chan

public:						
	// Field - Control
	CComboBox				m_cComboBox_Obj;
	CListCtrl				m_cListCtrl_Obj;
	CImageList				m_cImgList_Obj[(UINT)OBJ_TYPE::TYPEEND];							// ����Ʈ ��Ʈ�� ������ ����Ʈ

	// Field - General
	vector<CObj*>			m_vecObjPrefabs[(UINT)OBJ_TYPE::TYPEEND];							// ���� ������Ʈ ������ ����
	vector<CObj*>			m_vecObjInstances[(UINT)OBJ_TYPE::TYPEEND];							// �ν��Ͻ��� ������Ʈ ����
	
	CObj*					m_pCurPrefabObj;													// ���� ���õ� ���� ������

public:						
	// Methods - Control
	afx_msg void			OnCbnSelchangeMapObjComboType();									// ������Ʈ Ÿ�� �޺� �ڽ� ���� �׸� �����
	afx_msg void			OnNMClickMapObjListboxUnit(NMHDR *pNMHDR, LRESULT *pResult);		// ����Ʈ ��Ʈ���� Ŭ�� �̺�Ʈ ó�� (�ش� �׸��� m_pCurPrefabObj�� ����)
	afx_msg void			OnTimer				(UINT_PTR nIDEvent);							// �ִϸ��̼� ����� Ÿ�̸�

	// Methods - General
	HRESULT					Load_Unit_Prefabs	(const CString& _strPath);						// ������Ʈ ���Ͽ� ����Ǿ� �ִ� '������'�� �ε��Ѵ�.
	HRESULT					Load_Unit_Instances	(const CString& _strPath);									// �� ���Ͽ� ����Ǿ� �ִ� '�ν��Ͻ�'�� �ε��Ѵ�.
	HRESULT					Save_Unit_Instances	();							// ������Ʈ '�ν��Ͻ�'�� �����Ѵ�.
	
	const UINT				Get_IconID			(const CObj* const _pUnit) const;				// ����Ʈ ��Ʈ�ѿ� ����� ������ ID ���� ��ȣ�� �����Ͽ� ��ȯ
	HRESULT					Set_ListCtrl		(const OBJ_TYPE& _eType);						// �Ű������� ���� ������Ʈ Ÿ�Կ� �°� ����Ʈ ��Ʈ���� ������Ʈ �Ѵ�.
	CObj* const				Instantiate			(const CUnit* const _pPrefab = nullptr, const D3DXVECTOR3& _vWorld = D3DXVECTOR3{});	// �������� �Ѱ��ָ� �ν��Ͻ�ȭ�� ������Ʈ�� ��ȯ�Ѵ�.
	void					Mouse_Move			();												// ToolView���� ���콺�� �����Ͻ� ȣ��ȴ�.
	void					Change_Mode			(const MAPTOOL_MODE& _eMode);					// ���� ��带 �����Ѵ�(��, Ÿ��, ������Ʈ)
	TILE*					Get_TileInMousePos	();												// ���� ���콺 �����ǿ� �ش��ϴ� Ÿ���� ���´�.
#pragma endregion

};
