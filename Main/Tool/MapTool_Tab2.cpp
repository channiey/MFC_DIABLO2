// MapTool_Tab2.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool_Tab2.h"
#include "afxdialogex.h"
#include "TextureMgr.h"
#include "Terrain_Act.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "FileInfo.h"
#include "MyForm.h"
#include "Obj.h"
#include "Unit.h"

// CMapTool_Tab2 dialog




IMPLEMENT_DYNAMIC(CMapTool_Tab2, CDialogEx)

CMapTool_Tab2::CMapTool_Tab2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB2, pParent)
	, m_iTileDrawID(0)
	, m_strTileType(L"NONE")
	, m_fTileDmg(0)
	, m_iTileX(0)
	, m_iTileY(0)
	, m_pCurPrefabObj(nullptr)
	, m_eToolMode(MAPTOOL_MODE::MAP)
{
	       
}

CMapTool_Tab2::~CMapTool_Tab2()
{
}

void CMapTool_Tab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1_MAP, m_ListTile);
	DDX_Control(pDX, IDC_STATIC4_MAP, m_Texture_Tile);
	DDX_Control(pDX, IDC_COMBO1_MAP, m_ComboTile);

	DDX_Text(pDX, IDC_EDIT1_MAP, m_iTileDrawID);
	DDX_Text(pDX, IDC_EDIT2_MAP, m_strTileType);
	DDX_Text(pDX, IDC_EDIT3_MAP, m_fTileDmg);
	DDX_Text(pDX, IDC_EDIT4_MAP, m_iTileX);
	DDX_Text(pDX, IDC_EDIT5_MAP, m_iTileY);
	DDX_Control(pDX, IDC_COMBO2_MAP, m_Combo_SelecMap);
	DDX_Control(pDX, IDC_MAP_OBJ_COMBO_TYPE, m_cComboBox_Obj);
	DDX_Control(pDX, IDC_MAP_OBJ_LISTBOX_UNIT, m_cListCtrl_Obj);
	DDX_Control(pDX, IDC_RADIO1_MAP, m_Radio_Mini[0]);
	DDX_Control(pDX, IDC_RADIO2_MAP, m_Radio_Mini[1]);
	DDX_Control(pDX, IDC_SLIDER1_MAP, m_Slide_Row);
	DDX_Control(pDX, IDC_SLIDER2_MAP, m_Slide_Col);
}


BEGIN_MESSAGE_MAP(CMapTool_Tab2, CDialogEx)
	ON_WM_DESTROY()
	ON_LBN_SELCHANGE(IDC_LIST1_MAP, &CMapTool_Tab2::OnList_Tile)
	ON_CBN_SELCHANGE(IDC_COMBO1_MAP, &CMapTool_Tab2::OnCombo_ChangeAct)
	ON_BN_CLICKED(IDC_BUTTON1_MAP, &CMapTool_Tab2::OnButton_ReloadTile)
	ON_BN_CLICKED(IDC_BUTTON2_MAP, &CMapTool_Tab2::OnButton_CreateMap)
	ON_BN_CLICKED(IDC_BUTTON3_MAP, &CMapTool_Tab2::OnButton_SaveMap)
	ON_BN_CLICKED(IDC_BUTTON4_MAP, &CMapTool_Tab2::OnButton_LoadMap)
	ON_CBN_SELCHANGE(IDC_COMBO2_MAP, &CMapTool_Tab2::OnCombo_ChangeActMap)
	ON_LBN_DBLCLK(IDC_LIST1_MAP, &CMapTool_Tab2::OnList_TileReset)
	ON_CBN_SELCHANGE(IDC_MAP_OBJ_COMBO_TYPE, &CMapTool_Tab2::OnCbnSelchangeMapObjComboType)
	ON_BN_CLICKED(IDC_RADIO2_MAP, &CMapTool_Tab2::OnRadio_MiniView_Hide)
	ON_BN_CLICKED(IDC_RADIO1_MAP, &CMapTool_Tab2::OnRadio_MiniView_Show)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT4_MAP, &CMapTool_Tab2::OnEdit_ChangeRow)
	ON_EN_CHANGE(IDC_EDIT5_MAP, &CMapTool_Tab2::OnEdit_ChangeCol)
	ON_NOTIFY(NM_CLICK, IDC_MAP_OBJ_LISTBOX_UNIT, &CMapTool_Tab2::OnNMClickMapObjListboxUnit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMapTool_Tab2 message handlers


BOOL CMapTool_Tab2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//DoModal();

	CMainFrame*		pMainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
	m_pMainView = static_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 0));
	m_pFormView = static_cast<CMyForm*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

	if (nullptr == m_pMapTool_SubMap.GetSafeHwnd())
		m_pMapTool_SubMap.Create(IDD_MAPTOOL_SUBDLG);



#pragma region Jun
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Act1/Tile_%d.png", TEX_MULTI, L"Act1Terrain", L"Tile", 326)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Act2/Tile%d.png", TEX_MULTI, L"Act2Terrain", L"Tile", 32)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(L"../Texture/Tile/Act3/Tile_%d.png", TEX_MULTI, L"Act3Terrain", L"Tile", 289)))
	{
		AfxMessageBox(L"TileTexture Create Failed");
		return E_FAIL;
	}

	Load_TileList();

	
	m_Combo_SelecMap.AddString(_T("Act 1"));
	m_Combo_SelecMap.AddString(_T("Act 2"));
	m_Combo_SelecMap.AddString(_T("Act 3"));
	if (m_ComboTile.GetCount() > 0)
	{
		m_ComboTile.SetCurSel((int)TERRIAN_TYPE::ACT1);
	}

	m_ComboTile.AddString(_T("Act 1"));
	m_ComboTile.AddString(_T("Act 2"));
	m_ComboTile.AddString(_T("Act 3"));

	if (m_ComboTile.GetCount() > 0)
	{
		m_ComboTile.SetCurSel((int)TERRIAN_TYPE::ACT1);
	}
	if (m_ComboTile.GetCurSel() == (int)TERRIAN_TYPE::ACT1)
	{
		TCHAR szPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Texture\\Tile\\Act1\\");
		CString strPath = szPath;

		CFileFind finder;
		BOOL bWorking = finder.FindFile(strPath + _T("*.png"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

		
			if (!finder.IsDots() && !finder.IsDirectory())
			{
				CString filePath = finder.GetFilePath();
				CString fileName = L"Act1_" + finder.GetFileName();
				m_ListTile.AddString(fileName);

				auto	iter = m_TilePngImg.find(fileName);

				if (iter == m_TilePngImg.end())
				{
					CImage*	pPngImg = new CImage;
					pPngImg->Load(filePath);

					m_TilePngImg.insert({ fileName, pPngImg });
				}
			}
		}
		finder.Close();

		Sort_TileList(TERRIAN_TYPE::ACT1);
	}

	m_Radio_Mini[0].SetCheck(TRUE);

	m_Slide_Row.SetRange(0, 200);
	m_Slide_Row.SetPos(0);
	m_Slide_Row.SetLineSize(5);
	m_Slide_Row.SetPageSize(10);

	m_Slide_Col.SetRange(0, 200);
	m_Slide_Col.SetPos(0);
	m_Slide_Col.SetLineSize(5);
	m_Slide_Col.SetPageSize(10);


#pragma endregion

#pragma region Chan

	// 0. Tool View Set

	if (nullptr != m_pMainView)
		m_pMainView->m_pMapToolTab = this;

	// 1. Load Unit Prefabs

	if (FAILED(Load_Unit_Prefabs(L"../Data/01.Object/Player_Monster_Pivot_Origin_Recent.dat")))
	{
		AfxMessageBox(L"Failed Load Prefabs");
		return E_FAIL;
	}

	// 3. Set ComboBox
	{
		CString OBJ_TYPE_STRING[(UINT)OBJ_TYPE::TYPEEND]{ L"Player", L"Monster", L"Npc", L"Item", L"Terrain", L"Enviornment", L"UI" };

		//m_cComboBox_Obj.AddString(_T("Entire"));
		for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
			m_cComboBox_Obj.AddString(OBJ_TYPE_STRING[i]);
		
		m_cComboBox_Obj.SetCurSel(0);
	}

	// 4. Set ListControl
	{
		const int iIcoSize = 48;

		// ������ �̹����� �̹��� ����Ʈ�� �����Ѵ�.
		for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i) // ������Ʈ Ÿ�� ����ŭ�� �̹��� ����Ʈ�� �����Ѵ�.
		{
			m_cImgList_Obj[i].Create(iIcoSize, iIcoSize, ILC_COLOR32, m_vecObjPrefabs[i].size(), 5);

			for (size_t j = 0; j < m_vecObjPrefabs[i].size(); ++j) // ������Ʈ Ÿ�Ժ��� �ϳ��� ������ ������ ���̵� ������ ��, �̹��� ����Ʈ�� �����Ѵ�.
			{
				UINT iIconID = Get_IconID(m_vecObjPrefabs[i][j]); // �� ���̵� �ؿ� Add �Լ��� �Ű������� ����Ѵ�.

				m_cImgList_Obj[i].Add(AfxGetApp()->LoadIconW(iIconID)); // ���� �̹��� ����			
			}
		}

		// �ʱ� ����Ʈ ��Ʈ���� �����Ѵ�.
		if (FAILED(Set_ListCtrl((OBJ_TYPE)m_cComboBox_Obj.GetCurSel())))
		{
			AfxMessageBox(L"Fail the Setting List Ctrl");
			return E_FAIL;
		}
	}

	SetTimer(ID_TIMER, 0.001, NULL);
#pragma endregion

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapTool_Tab2::OnDestroy()
{
	CDialogEx::OnDestroy();

#pragma region Jun
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	for_each(m_TilePngImg.begin(), m_TilePngImg.end(),
		[](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});
	m_TilePngImg.clear();

	Safe_Delete(m_pTerrain_Act1);
	Safe_Delete(m_pTerrain_Act2);
	Safe_Delete(m_pTerrain_Act3);
#pragma endregion


#pragma region Chan
	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (auto& pObject : m_vecObjInstances[i])
			Safe_Delete(pObject);

		m_vecObjInstances[i].clear();
	}

	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (auto& pObject : m_vecObjPrefabs[i])
			Safe_Delete(pObject);

		m_vecObjPrefabs[i].clear();
	}
#pragma endregion

}

#pragma region Jun

void CMapTool_Tab2::OnRadio_MiniView_Hide()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	m_pMapTool_SubMap.ShowWindow(SW_HIDE);
}

void CMapTool_Tab2::OnRadio_MiniView_Show()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	m_pMapTool_SubMap.ShowWindow(SW_SHOW);
}

void CMapTool_Tab2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	Change_Mode(MAPTOOL_MODE::MAP);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	UpdateData(TRUE);

	if (IDC_SLIDER1_MAP == pScrollBar->GetDlgCtrlID())
	{
		int iPos = m_Slide_Row.GetPos();

		m_iTileX = iPos;
	}
	if (IDC_SLIDER2_MAP == pScrollBar->GetDlgCtrlID())
	{
		int iPos = m_Slide_Col.GetPos();

		m_iTileY = iPos;
	}

	

	UpdateData(FALSE);
}

void CMapTool_Tab2::OnEdit_ChangeRow()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	UpdateData(TRUE);

	m_Slide_Row.SetPos(m_iTileX);

	UpdateData(FALSE);
}

void CMapTool_Tab2::OnEdit_ChangeCol()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	UpdateData(TRUE);

	m_Slide_Col.SetPos(m_iTileY);

	UpdateData(FALSE);
}

void CMapTool_Tab2::OnList_Tile()
{
	Change_Mode(MAPTOOL_MODE::TILE);


	UpdateData(TRUE);

	int	iSelect = m_ListTile.GetCurSel();

	if (LB_ERR == iSelect)
		return;

	for (auto& iter : m_vecTile)
	{
		TERRIAN_TYPE tCurAct = static_cast<TERRIAN_TYPE>(m_ComboTile.GetCurSel());

		switch (tCurAct)
		{
		case TERRIAN_TYPE::ACT1:
			if (iter->eType == TERRIAN_TYPE::ACT1 &&
				iter->byDrawID == iSelect)
			{
				m_tSelectTile = iter;

				int	iSelect = m_ListTile.GetCurSel();

				if (LB_ERR == iSelect)
					return;
				
				CString strSelectName;
				m_ListTile.GetText(iSelect, strSelectName);

				auto Imgiter = m_TilePngImg.find(strSelectName);

				if (Imgiter == m_TilePngImg.end())
					return;

				CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
				m_Texture_Tile.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
				CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
				dc = m_Texture_Tile.GetDC(); //���� ��Ʈ���� DC�� ��´�.
				(*Imgiter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
			}
			break;
		case TERRIAN_TYPE::ACT2:
			if (iter->eType == TERRIAN_TYPE::ACT2 &&
				iter->byDrawID == iSelect)
			{
				m_tSelectTile = iter;

				int	iSelect = m_ListTile.GetCurSel();

				if (LB_ERR == iSelect)
					return;

				CString strSelectName;
				m_ListTile.GetText(iSelect, strSelectName);

				auto Imgiter = m_TilePngImg.find(strSelectName);

				if (Imgiter == m_TilePngImg.end())
					return;

				CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
				m_Texture_Tile.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
				CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
				dc = m_Texture_Tile.GetDC(); //���� ��Ʈ���� DC�� ��´�.
				(*Imgiter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
			}
			break;
		case TERRIAN_TYPE::ACT3:
			if (iter->eType == TERRIAN_TYPE::ACT3 &&
				iter->byDrawID == iSelect)
			{
				m_tSelectTile = iter;

				int	iSelect = m_ListTile.GetCurSel();

				if (LB_ERR == iSelect)
					return;

				CString strSelectName;
				m_ListTile.GetText(iSelect, strSelectName);

				auto Imgiter = m_TilePngImg.find(strSelectName);

				if (Imgiter == m_TilePngImg.end())
					return;

				CRect rect;//���� ��Ʈ���� ũ�⸦ ������ CRect ��ü
				m_Texture_Tile.GetWindowRect(rect);//GetWindowRect�� ����ؼ� ���� ��Ʈ���� ũ�⸦ �޴´�.
				CDC* dc; //���� ��Ʈ���� DC�� ������  CDC ������
				dc = m_Texture_Tile.GetDC(); //���� ��Ʈ���� DC�� ��´�.
				(*Imgiter->second).StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);//�̹����� ���� ��Ʈ�� ũ��� ����
			}
			break;
		default:
			break;
		}

	}

	m_iTileDrawID = m_tSelectTile->byDrawID;
	byte btOption = m_tSelectTile->byOption;
	if (btOption == NONETILE)
	{
		m_strTileType = L"NONE";
	}
	else if (btOption == BLOCKTILE)
	{
		m_strTileType = L"BLOCK";
	}
	else if (btOption == DAMAGETILE)
	{
		m_strTileType = L"DAMAGE";
	}
	else if (((btOption & BLOCKTILE) == BLOCKTILE) &&
		((btOption & DAMAGETILE) == DAMAGETILE))
	{
		m_strTileType = L"BLOCK+DMG";
	}
	m_fTileDmg = m_tSelectTile->fDamage;


	UpdateData(FALSE);
}

void CMapTool_Tab2::Load_TileList()
{
	Change_Mode(MAPTOOL_MODE::TILE);


	UpdateData(TRUE);

	if (m_vecTile.empty())
	{
		HANDLE	hFile = CreateFile(L"../Data/Save_Tile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD	dwByte = 0;

		while (true)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			m_vecTile.push_back(pTile);

			
		}

		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}

void CMapTool_Tab2::OnCombo_ChangeAct()
{
	Change_Mode(MAPTOOL_MODE::TILE);


	UpdateData(TRUE);

	m_ListTile.ResetContent();

	if (m_ComboTile.GetCurSel() == (int)TERRIAN_TYPE::ACT1)
	{
		TCHAR szPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Texture\\Tile\\Act1\\");
		CString strPath = szPath;

		CFileFind finder;
		BOOL bWorking = finder.FindFile(strPath + _T("*.png"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			if (!finder.IsDots() && !finder.IsDirectory())
			{
				CString filePath = finder.GetFilePath();
				CString fileName = L"Act1_" + finder.GetFileName();
				m_ListTile.AddString(fileName);

				auto	iter = m_TilePngImg.find(fileName);

				if (iter == m_TilePngImg.end())
				{
					CImage*	pPngImg = new CImage;
					pPngImg->Load(filePath);

					m_TilePngImg.insert({ fileName, pPngImg });
				}
			}
		}
		finder.Close();

		Sort_TileList(TERRIAN_TYPE::ACT1);
	}
	else if (m_ComboTile.GetCurSel() == (int)TERRIAN_TYPE::ACT2)
	{
		TCHAR szPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Texture\\Tile\\Act2\\");
		CString strPath = szPath;

		CFileFind finder;
		BOOL bWorking = finder.FindFile(strPath + _T("*.png"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			if (!finder.IsDots() && !finder.IsDirectory())
			{
				CString filePath = finder.GetFilePath();
				CString fileName = L"Act2_" + finder.GetFileName();
				m_ListTile.AddString(fileName);

				auto	iter = m_TilePngImg.find(fileName);

				if (iter == m_TilePngImg.end())
				{
					CImage*	pPngImg = new CImage;
					pPngImg->Load(filePath);

					m_TilePngImg.insert({ fileName, pPngImg });
				}
			}
		}
		finder.Close();

		Sort_TileList(TERRIAN_TYPE::ACT2);
	}
	else if (m_ComboTile.GetCurSel() == (int)TERRIAN_TYPE::ACT3)
	{
		TCHAR szPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Texture\\Tile\\Act3\\");
		CString strPath = szPath;

		CFileFind finder;
		BOOL bWorking = finder.FindFile(strPath + _T("*.png"));
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (!finder.IsDots() && !finder.IsDirectory())
			{
				CString filePath = finder.GetFilePath();
				CString fileName = L"Act3_" + finder.GetFileName();
				m_ListTile.AddString(fileName);

				auto	iter = m_TilePngImg.find(fileName);

				if (iter == m_TilePngImg.end())
				{
					CImage*	pPngImg = new CImage;
					pPngImg->Load(filePath);

					m_TilePngImg.insert({ fileName, pPngImg });
				}
			}
		}
		finder.Close();

		Sort_TileList(TERRIAN_TYPE::ACT3);
	}

	UpdateData(FALSE);
}

void CMapTool_Tab2::Sort_TileList(TERRIAN_TYPE _eType)
{
	Change_Mode(MAPTOOL_MODE::TILE);


	UpdateData(TRUE);

	switch (_eType)
	{
	case TERRIAN_TYPE::ACT1:
	{
		vector<CString> fileList;
		for (int i = 0; i < m_ListTile.GetCount(); ++i)
		{
			CString fileName;
			m_ListTile.GetText(i, fileName);
			fileList.push_back(fileName);
		}

		sort(fileList.begin(), fileList.end(), [](const CString& str1, const CString& str2) {
			CString numStr1 = str1.Mid(10, str1.GetLength() - (4 * sizeof(TCHAR)));
			int num1 = _wtoi(numStr1);

			CString numStr2 = str2.Mid(10, str2.GetLength() - (4 * sizeof(TCHAR)));
			int num2 = _wtoi(numStr2);

			return num1 < num2;
		});

		m_ListTile.ResetContent();
		for (const CString& fileName : fileList)
		{
			m_ListTile.AddString(fileName);
		}
	}
	break;
	case TERRIAN_TYPE::ACT2:
	{
		vector<CString> fileList;
		for (int i = 0; i < m_ListTile.GetCount(); ++i)
		{
			CString fileName;
			m_ListTile.GetText(i, fileName);
			fileList.push_back(fileName);
		}

		sort(fileList.begin(), fileList.end(), [](const CString& str1, const CString& str2) {
			CString numStr1 = str1.Mid(9, str1.GetLength() - (4 * sizeof(TCHAR)));
			int num1 = _wtoi(numStr1);

			CString numStr2 = str2.Mid(9, str2.GetLength() - (4 * sizeof(TCHAR)));
			int num2 = _wtoi(numStr2);

			return num1 < num2;
		});

		m_ListTile.ResetContent();

		for (const CString& fileName : fileList)
		{
			m_ListTile.AddString(fileName);
		}
	}
	break;
	case TERRIAN_TYPE::ACT3:
	{
		vector<CString> fileList;
		for (int i = 0; i < m_ListTile.GetCount(); ++i)
		{
			CString fileName;
			m_ListTile.GetText(i, fileName);
			fileList.push_back(fileName);
		}

		sort(fileList.begin(), fileList.end(), [](const CString& str1, const CString& str2) {
			CString numStr1 = str1.Mid(10, str1.GetLength() - (4 * sizeof(TCHAR)));
			int num1 = _wtoi(numStr1);

			CString numStr2 = str2.Mid(10, str2.GetLength() - (4 * sizeof(TCHAR)));
			int num2 = _wtoi(numStr2);

			return num1 < num2;
		});

		m_ListTile.ResetContent();
		for (const CString& fileName : fileList)
		{
			m_ListTile.AddString(fileName);
		}
	}
	break;
	default:
		break;
	}

	UpdateData(FALSE);
}


void CMapTool_Tab2::OnButton_ReloadTile()
{
	Change_Mode(MAPTOOL_MODE::TILE);


	UpdateData(TRUE);

	if (!m_vecTile.empty())
	{
		for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
		m_vecTile.clear();
		m_vecTile.shrink_to_fit();
	}
	if (m_vecTile.empty())
	{
		HANDLE	hFile = CreateFile(L"../Data/Save_Tile.dat",
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD	dwByte = 0;

		while (true)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			m_vecTile.push_back(pTile);


		}

		CloseHandle(hFile);
	}


	UpdateData(FALSE);
}

void CMapTool_Tab2::OnList_TileReset()
{
	Change_Mode(MAPTOOL_MODE::TILE);


	m_ListTile.SetCurSel(-1);
	m_tSelectTile = nullptr;
}


// Map
void CMapTool_Tab2::OnCombo_ChangeActMap()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	UpdateData(TRUE);

	TERRIAN_TYPE eTerrianType = static_cast<TERRIAN_TYPE>(m_Combo_SelecMap.GetCurSel());

	switch (eTerrianType)
	{
	case TERRIAN_TYPE::ACT1:
		if (m_pTerrain_Act1 != nullptr)
		{
			m_pMainView->m_pTerrain_Act1_View = m_pTerrain_Act1;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
			m_iTileX = m_pTerrain_Act1->m_ActTileX;
			m_iTileY = m_pTerrain_Act1->m_ActTileY;
			m_Slide_Row.SetPos(m_iTileX);
			m_Slide_Col.SetPos(m_iTileY);
		}
		break;
	case TERRIAN_TYPE::ACT2:
		if (m_pTerrain_Act2 != nullptr)
		{
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act2_View = m_pTerrain_Act2;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
			m_iTileX = m_pTerrain_Act2->m_ActTileX;
			m_iTileY = m_pTerrain_Act2->m_ActTileY;
			m_Slide_Row.SetPos(m_iTileX);
			m_Slide_Col.SetPos(m_iTileY);
		}
		break;
	case TERRIAN_TYPE::ACT3:
		if (m_pTerrain_Act3 != nullptr)
		{
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = m_pTerrain_Act3;
			m_iTileX = m_pTerrain_Act3->m_ActTileX;
			m_iTileY = m_pTerrain_Act3->m_ActTileY;
			m_Slide_Row.SetPos(m_iTileX);
			m_Slide_Col.SetPos(m_iTileY);
		}
		break;
	default:
		break;
	}


	m_pMainView->Invalidate(FALSE);
	m_pFormView->m_pMapTool_Tab2->m_pMapTool_SubMap.m_pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMapTool_Tab2::OnButton_CreateMap()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	UpdateData(TRUE);

	TERRIAN_TYPE eTerrianType = static_cast<TERRIAN_TYPE>(m_Combo_SelecMap.GetCurSel());

	switch (eTerrianType)
	{
	case TERRIAN_TYPE::ACT1:
	{
		if (m_pTerrain_Act1 == nullptr)
		{
			m_pTerrain_Act1 = new CTerrain_Act;
			m_pTerrain_Act1->Initialize();
			m_pTerrain_Act1->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act1_View = m_pTerrain_Act1;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
		}
		else
		{
			Safe_Delete(m_pTerrain_Act1);
			m_pTerrain_Act1 = new CTerrain_Act;
			m_pTerrain_Act1->Initialize();
			m_pTerrain_Act1->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act1_View = m_pTerrain_Act1;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
		}
	}
		break;
	case TERRIAN_TYPE::ACT2:
	{
		if (m_pTerrain_Act2 == nullptr)
		{
			m_pTerrain_Act2 = new CTerrain_Act;
			m_pTerrain_Act2->Initialize();
			m_pTerrain_Act2->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act2_View = m_pTerrain_Act2;
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
		}
		else
		{
			Safe_Delete(m_pTerrain_Act2);
			m_pTerrain_Act2 = new CTerrain_Act;
			m_pTerrain_Act2->Initialize();
			m_pTerrain_Act2->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act2_View = m_pTerrain_Act2;
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act3_View = nullptr;
		}
	}
		break;
	case TERRIAN_TYPE::ACT3:
	{
		if (m_pTerrain_Act3 == nullptr)
		{
			m_pTerrain_Act3 = new CTerrain_Act;
			m_pTerrain_Act3->Initialize();
			m_pTerrain_Act3->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act3_View = m_pTerrain_Act3;
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
		}
		else
		{
			Safe_Delete(m_pTerrain_Act3);
			m_pTerrain_Act3 = new CTerrain_Act;
			m_pTerrain_Act3->Initialize();
			m_pTerrain_Act3->Create_Terrian(m_iTileX, m_iTileY);
			m_pMainView->m_pTerrain_Act3_View = m_pTerrain_Act3;
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			m_pMainView->m_pTerrain_Act2_View = nullptr;
		}
	}
		break;
	default:
		break;
	}

	m_pMainView->Invalidate(FALSE);
	m_pFormView->m_pMapTool_Tab2->m_pMapTool_SubMap.m_pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
}

void CMapTool_Tab2::OnButton_SaveMap()
{
	Change_Mode(MAPTOOL_MODE::MAP);

	UpdateData(TRUE);

	TERRIAN_TYPE eTerrianType = static_cast<TERRIAN_TYPE>(m_Combo_SelecMap.GetCurSel());

	switch (eTerrianType)
	{
	case TERRIAN_TYPE::ACT1:
	{
		if (m_pTerrain_Act1 == nullptr)
			return;

		CFileDialog		Dlg(FALSE,	// TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����	
			L"dat", // defaule ���� Ȯ���ڸ�
			L"Save_Act1_Map.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ����� ���� �� ���޼��� ����)
			L"Data Files(*.dat) | *.dat||",  // ��ȭ ���ڿ� ǥ�õ� ���� ���� '�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||'
			this); // �θ� ������ �ּ�

		TCHAR	szPath[MAX_PATH] = L"";

		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Data");
		Dlg.m_ofn.lpstrInitialDir = szPath;

		if (IDOK == Dlg.DoModal())
		{
			CString		strTemp = Dlg.GetPathName().GetString();
			const TCHAR* pGetPath = strTemp.GetString();

			HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (INVALID_HANDLE_VALUE == hFile)
				return;

			DWORD	dwByte = 0;

			WriteFile(hFile, &m_iTileX, sizeof(UINT), &dwByte, nullptr);
			WriteFile(hFile, &m_iTileY, sizeof(UINT), &dwByte, nullptr);
			for (auto& iter : m_pTerrain_Act1->m_vecActTile)
				WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);
		
			CloseHandle(hFile);
		}
	}
		break;
	case TERRIAN_TYPE::ACT2:
	{
		if (m_pTerrain_Act2 == nullptr)
			return;

		CFileDialog		Dlg(FALSE,	// TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����	
			L"dat", // defaule ���� Ȯ���ڸ�
			L"Save_Act2_Map.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ����� ���� �� ���޼��� ����)
			L"Data Files(*.dat) | *.dat||",  // ��ȭ ���ڿ� ǥ�õ� ���� ���� '�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||'
			this); // �θ� ������ �ּ�

		TCHAR	szPath[MAX_PATH] = L"";

		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Data");
		Dlg.m_ofn.lpstrInitialDir = szPath;

		if (IDOK == Dlg.DoModal())
		{
			CString		strTemp = Dlg.GetPathName().GetString();
			const TCHAR* pGetPath = strTemp.GetString();

			HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (INVALID_HANDLE_VALUE == hFile)
				return;

			DWORD	dwByte = 0;

			WriteFile(hFile, &m_iTileX, sizeof(UINT), &dwByte, nullptr);
			WriteFile(hFile, &m_iTileY, sizeof(UINT), &dwByte, nullptr);
			for (auto& iter : m_pTerrain_Act2->m_vecActTile)
				WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

			CloseHandle(hFile);
		}
	}
		break;
	case TERRIAN_TYPE::ACT3:
	{
		if (m_pTerrain_Act3 == nullptr)
			return;

		CFileDialog		Dlg(FALSE,	// TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����	
			L"dat", // defaule ���� Ȯ���ڸ�
			L"Save_Act3_Map.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ����� ���� �� ���޼��� ����)
			L"Data Files(*.dat) | *.dat||",  // ��ȭ ���ڿ� ǥ�õ� ���� ���� '�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||'
			this); // �θ� ������ �ּ�

		TCHAR	szPath[MAX_PATH] = L"";

		GetCurrentDirectory(MAX_PATH, szPath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\Data");
		Dlg.m_ofn.lpstrInitialDir = szPath;

		if (IDOK == Dlg.DoModal())
		{
			CString		strTemp = Dlg.GetPathName().GetString();
			const TCHAR* pGetPath = strTemp.GetString();

			HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (INVALID_HANDLE_VALUE == hFile)
				return;

			DWORD	dwByte = 0;

			WriteFile(hFile, &m_iTileX, sizeof(UINT), &dwByte, nullptr);
			WriteFile(hFile, &m_iTileY, sizeof(UINT), &dwByte, nullptr);
			for (auto& iter : m_pTerrain_Act3->m_vecActTile)
				WriteFile(hFile, iter, sizeof(TILE), &dwByte, nullptr);

			CloseHandle(hFile);
		}
	}
		break;
	default:
		break;
	}

	Save_Unit_Instances();

	UpdateData(FALSE);
}

void CMapTool_Tab2::OnButton_LoadMap()
{
	Change_Mode(MAPTOOL_MODE::MAP);


	UpdateData(TRUE);

	TERRIAN_TYPE eTerrianType = static_cast<TERRIAN_TYPE>(m_Combo_SelecMap.GetCurSel());

	switch (eTerrianType)
	{
	case TERRIAN_TYPE::ACT1:
	{
		if (m_pTerrain_Act1 == nullptr)
		{
			m_pTerrain_Act1 = new CTerrain_Act;
			m_pTerrain_Act1->Initialize();
		}
		else
		{
			m_pMainView->m_pTerrain_Act1_View = nullptr;
			Safe_Delete(m_pTerrain_Act1);
			m_pTerrain_Act1 = new CTerrain_Act;
			m_pTerrain_Act1->Initialize();
		}
		{
			CFileDialog		Dlg(TRUE,
				L"dat",
				L"Map_Scene1.dat",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				L"Data Files(*.dat) | *.dat||",
				this);
			TCHAR	szPath[MAX_PATH] = L"";

			GetCurrentDirectory(MAX_PATH, szPath);

			PathRemoveFileSpec(szPath);

			lstrcat(szPath, L"\\Data");
			Dlg.m_ofn.lpstrInitialDir = szPath;

			if (IDOK == Dlg.DoModal())
			{
				CString		strTemp = Dlg.GetPathName().GetString();
				const TCHAR* pGetPath = strTemp.GetString();

				HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
					0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return;

				int		iCount = 0;
				UINT	iTileX = 0;
				UINT	iTileY = 0;
				DWORD	dwByte = 0;

				while (true)
				{
					if (iCount == 0)
					{
						ReadFile(hFile, &iTileX, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount == 1)
					{
						ReadFile(hFile, &iTileY, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount >= 2)
					{
						TILE* pTile = new TILE;

						ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);


						if (0 == dwByte)
						{
							Safe_Delete(pTile);
							break;
						}

						m_pTerrain_Act1->m_vecActTile.push_back(pTile);
					}

					iCount++;
				}

				m_iTileX = iTileX;
				m_iTileY = iTileY;
				m_Slide_Row.SetPos(m_iTileX);
				m_Slide_Col.SetPos(m_iTileY);

				CloseHandle(hFile);

				if (FAILED(Load_Unit_Instances(L"../Data/01.Object/map/Map_Instances.dat")))
				{
					AfxMessageBox(L"Failed Load Instances");
					return;
				}
			}
			m_pTerrain_Act1->m_ActTileX = m_iTileX;
			m_pTerrain_Act1->m_ActTileY = m_iTileY;
			m_pMainView->m_pTerrain_Act1_View = m_pTerrain_Act1;
		}
	}
		break;
	case TERRIAN_TYPE::ACT2:
	{
		if (m_pTerrain_Act2 == nullptr)
		{
			m_pTerrain_Act2 = new CTerrain_Act;
			m_pTerrain_Act2->Initialize();
		}
		else
		{
			m_pMainView->m_pTerrain_Act2_View = nullptr;
			Safe_Delete(m_pTerrain_Act2);
			m_pTerrain_Act2 = new CTerrain_Act;
			m_pTerrain_Act2->Initialize();
		}
		{
			CFileDialog		Dlg(TRUE,
				L"dat",
				L"Map_Scene1.dat",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				L"Data Files(*.dat) | *.dat||",
				this);

			TCHAR	szPath[MAX_PATH] = L"";

			GetCurrentDirectory(MAX_PATH, szPath);

			PathRemoveFileSpec(szPath);

			lstrcat(szPath, L"\\Data");
			Dlg.m_ofn.lpstrInitialDir = szPath;

			if (IDOK == Dlg.DoModal())
			{
				CString		strTemp = Dlg.GetPathName().GetString();
				const TCHAR* pGetPath = strTemp.GetString();

				HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
					0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return;

				int		iCount = 0;
				UINT	iTileX = 0;
				UINT	iTileY = 0;
				DWORD	dwByte = 0;

				while (true)
				{
					if (iCount == 0)
					{
						ReadFile(hFile, &iTileX, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount == 1)
					{
						ReadFile(hFile, &iTileY, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount >= 2)
					{
						TILE* pTile = new TILE;

						ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);


						if (0 == dwByte)
						{
							Safe_Delete(pTile);
							break;
						}

						m_pTerrain_Act2->m_vecActTile.push_back(pTile);
					}

					iCount++;
				}

				m_iTileX = iTileX;
				m_iTileY = iTileY;
				m_Slide_Row.SetPos(m_iTileX);
				m_Slide_Col.SetPos(m_iTileY);

				CloseHandle(hFile);

				if (FAILED(Load_Unit_Instances(L"../Data/01.Object/map/Map_Instances.dat")))
				{
					AfxMessageBox(L"Failed Load Instances");
					return;
				}
			}
			m_pTerrain_Act2->m_ActTileX = m_iTileX;
			m_pTerrain_Act2->m_ActTileY = m_iTileY;
			m_pMainView->m_pTerrain_Act2_View = m_pTerrain_Act2;
		}
	}
		break;
	case TERRIAN_TYPE::ACT3:
	{
		if (m_pTerrain_Act3 == nullptr)
		{
			m_pTerrain_Act3 = new CTerrain_Act;
			m_pTerrain_Act3->Initialize();
		}
		else
		{
			m_pMainView->m_pTerrain_Act3_View = nullptr;
			Safe_Delete(m_pTerrain_Act3);
			m_pTerrain_Act3 = new CTerrain_Act;
			m_pTerrain_Act3->Initialize();
		}
		{
			CFileDialog		Dlg(TRUE,
				L"dat",
				L"Map_Scene1.dat",
				OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				L"Data Files(*.dat) | *.dat||",
				this);

			TCHAR	szPath[MAX_PATH] = L"";

			GetCurrentDirectory(MAX_PATH, szPath);

			PathRemoveFileSpec(szPath);

			lstrcat(szPath, L"\\Data");
			Dlg.m_ofn.lpstrInitialDir = szPath;

			if (IDOK == Dlg.DoModal())
			{
				CString		strTemp = Dlg.GetPathName().GetString();
				const TCHAR* pGetPath = strTemp.GetString();

				HANDLE hFile = CreateFile(pGetPath, GENERIC_READ,
					0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (INVALID_HANDLE_VALUE == hFile)
					return;

				int		iCount = 0;
				UINT	iTileX = 0;
				UINT	iTileY = 0;
				DWORD	dwByte = 0;

				while (true)
				{
					if (iCount == 0)
					{
						ReadFile(hFile, &iTileX, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount == 1)
					{
						ReadFile(hFile, &iTileY, sizeof(UINT), &dwByte, nullptr);
					}
					else if (iCount >= 2)
					{
						TILE* pTile = new TILE;

						ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

						if (0 == dwByte)
						{
							Safe_Delete(pTile);
							break;
						}

						m_pTerrain_Act3->m_vecActTile.push_back(pTile);
					}

					iCount++;
				}

				m_iTileX = iTileX;
				m_iTileY = iTileY;
				m_Slide_Row.SetPos(m_iTileX);
				m_Slide_Col.SetPos(m_iTileY);

				CloseHandle(hFile);

				if (FAILED(Load_Unit_Instances(L"../Data/01.Object/map/Map_Instances.dat")))
				{
					AfxMessageBox(L"Failed Load Instances");
					return;
				}
			}
		}

		m_pTerrain_Act3->m_ActTileX = m_iTileX;
		m_pTerrain_Act3->m_ActTileY = m_iTileY;
		m_pMainView->m_pTerrain_Act3_View = m_pTerrain_Act3;
	}
		break;
	default:
		AfxMessageBox(L"Act is None! Select Act.");
		break;
	}

	m_pMainView->Invalidate(FALSE);
	m_pFormView->m_pMapTool_Tab2->m_pMapTool_SubMap.m_pMiniView->Invalidate(FALSE);
	UpdateData(FALSE);
}

#pragma endregion

#pragma region Chan

HRESULT CMapTool_Tab2::Load_Unit_Prefabs(const CString & _strPath)
{
	HANDLE	hFile = CreateFile(_strPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;
	CUnit*	pUnit = nullptr;
	CString OBJ_STATE_STRING[(UINT)OBJ_STATE::TYPEEND]{ L"stand", L"walk", L"dash", L"attack", L"damage", L"skill", L"die" };

	while (true)
	{
		pUnit = new CUnit;

		// �̸� ���ڿ� ����
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

		// �ִϸ��̼� ����
		{
			// Animation Set
			ANIMATION* pAni = new ANIMATION;

			pAni->bLoop = false;
			pAni->iCurFrame = 0;
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND], pAni });
			pUnit->m_pCurAni = pAni;
		}

		// ������ ��� ���� ����
		{
			ReadFile(hFile, &(pUnit->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_tStat), sizeof(STAT), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eType), sizeof(OBJ_TYPE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eState), sizeof(OBJ_STATE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->fSecondPerFrame), sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->iMaxFrame), sizeof(int), &dwByte, nullptr);
	
			pUnit->m_strObjKey = pUnit->m_strName;
			pUnit->m_strStateKey = OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] + L"_8";
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] , pUnit->m_pCurAni });
		}

		// Ǫ�ù�
		m_vecObjPrefabs[(UINT)pUnit->m_eType].push_back(pUnit);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMapTool_Tab2::Load_Unit_Instances(const CString& _strPath)
{

	HANDLE	hFile = CreateFile(_strPath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	DWORD	dwStrByte = 0;
	CUnit*	pUnit = nullptr;
	CString OBJ_STATE_STRING[(UINT)OBJ_STATE::TYPEEND]{ L"stand", L"walk", L"dash", L"attack", L"damage", L"skill", L"die" };

	while (true)
	{
		pUnit = new CUnit;

		// �̸� ���ڿ� ����
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

		// �ִϸ��̼� ����
		{
			// Animation Set
			ANIMATION* pAni = new ANIMATION;

			pAni->bLoop = false;
			pAni->iCurFrame = 0;
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND], pAni });
			pUnit->m_pCurAni = pAni;
		}

		// ������ ��� ���� ����
		{
			ReadFile(hFile, &(pUnit->m_tInfo), sizeof(INFO), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_tStat), sizeof(STAT), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eType), sizeof(OBJ_TYPE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_eState), sizeof(OBJ_STATE), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->fSecondPerFrame), sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_pCurAni->iMaxFrame), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(pUnit->m_vWorldPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);

			pUnit->m_strObjKey = pUnit->m_strName;
			pUnit->m_strStateKey = OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] + L"_8";
			pUnit->m_mapAni.insert({ OBJ_STATE_STRING[(UINT)OBJ_STATE::STAND] , pUnit->m_pCurAni });
		}

		// Ǫ�ù�
		pUnit->m_pMainView = m_pMainView;
		pUnit->m_bPlay = true;
		m_vecObjInstances[(UINT)pUnit->m_eType].push_back(pUnit);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMapTool_Tab2::Save_Unit_Instances()
{
	// ���̾�α� ����
	CFileDialog		Dlg(FALSE,	// TRUE(����), FALSE(�ٸ� �̸����� ����) ��� ����	
		L"dat", // defaule ���� Ȯ���ڸ�
		L"*.dat", // ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, // OFN_HIDEREADONLY(�б����� üũ�ڽ� ����), OFN_OVERWRITEPROMPT(�ߺ����� ���� �� ���޼��� ����)
		L"Data Files(*.dat) | *.dat||",  // ��ȭ ���ڿ� ǥ�õ� ���� ���� '�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||'
		this); // �θ� ������ �ּ�

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);
	PathRemoveFileSpec(szPath);
	lstrcat(szPath, L"..\\Data\\01.Object\\map");
	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString		strTemp = Dlg.GetPathName().GetString();
		const TCHAR* pGetPath = strTemp.GetString();

		HANDLE hFile = CreateFile(pGetPath, GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		DWORD dwByte = 0;
		DWORD	dwStrByte = 0;

		for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
		{
			for (auto& iter : m_vecObjInstances[i])
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
				WriteFile(hFile, &(iter->m_vWorldPos), sizeof(D3DXVECTOR3), &dwByte, nullptr);
				/*	WriteFile(hFile, &(iter->m_strObjKey), sizeof(CString), &dwByte, nullptr);
				WriteFile(hFile, &(iter->m_strStateKey), sizeof(CString), &dwByte, nullptr);*/
			}

		}

		CloseHandle(hFile);
	}
	return E_NOTIMPL;
}

void CMapTool_Tab2::OnCbnSelchangeMapObjComboType()
{
	Change_Mode(MAPTOOL_MODE::OBJ);

	
	m_pCurPrefabObj = nullptr;
	
	UpdateData(TRUE);

	Set_ListCtrl((OBJ_TYPE)m_cComboBox_Obj.GetCurSel());
}

const UINT CMapTool_Tab2::Get_IconID(const CObj * const _pUnit) const
{
	UINT iID			= 0;
	CFileFind			Find;
	UINT				iCount = 0;

	// 1. m_strObjKey�� ���� ��� ����

	CString OBJ_TYPE_STRING[(UINT)OBJ_TYPE::TYPEEND]{ L"Player", L"Monster", L"Npc", L"Item", L"Terrain", L"Enviornment", L"UI" };

	CString strFilePath = L"../Texture/Object/";

	strFilePath += OBJ_TYPE_STRING[(UINT)_pUnit->m_eType] + L"/*.*";
	//L"../Texture/Object/Player/*.*"

	BOOL	bContinue = Find.FindFile(strFilePath);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;
		else
		{
			if (Find.IsSystem())
				continue;

			const CString strObjKey = Find.GetFileName();
			if (_pUnit->m_strObjKey == strObjKey)
			{
				// ���̵� �����ؼ� ����
				// ������ȣ(������ ID ������) : 9 / ������Ʈ Ÿ��(0~9) / ������Ʈ ��ȣ(000 ~ 009)
				
				CString strID = L"";
				strID.Format(L"%d%d00%d", 9, (UINT)_pUnit->m_eType, iCount);

				iID = _ttoi(strID);
				return iID;

			}
			else
			{
				++iCount;
				continue;
			}
		}
	}
	return iID;
}

HRESULT CMapTool_Tab2::Set_ListCtrl(const OBJ_TYPE& _eType)
{
	// ����Ʈ ��Ʈ�ѿ� �ִ� ���� �������� ��� �����Ѵ�.
	m_cListCtrl_Obj.DeleteAllItems();

	// ����Ʈ ��Ʈ�ѿ� �̹��� ����Ʈ�� Ÿ�Կ� �°� ��ü�Ѵ�.
	m_cListCtrl_Obj.SetImageList(&m_cImgList_Obj[(UINT)_eType], LVSIL_NORMAL); // ������ �𸣰����� LVSIL_NORMAL �̿ܿ� �ٸ� �ɼ��� ������ �ȵ�

	// ����Ʈ ��Ʈ�ѿ� ������Ʈ �̸��� �߰��Ѵ�.
	for (size_t i = 0; i < m_vecObjPrefabs[(UINT)_eType].size(); ++i)
		m_cListCtrl_Obj.InsertItem(i, m_vecObjPrefabs[(UINT)_eType][i]->m_strObjKey, i);

	return S_OK;
}

CObj * const CMapTool_Tab2::Instantiate(const CUnit * const _pPrefab, const D3DXVECTOR3 & _vWorld)
{
	CObj* pInstance = nullptr;

	// Ŭ��
	if (nullptr == _pPrefab)
	{
		if (nullptr == m_pCurPrefabObj)
			return nullptr;
		else
			pInstance = new CUnit(*(static_cast<CUnit*>(m_pCurPrefabObj)));
	}
	else
		pInstance = new CUnit(*_pPrefab);

	// ���� ����
	if (nullptr != m_pCurPrefabObj->m_pCurAni)
	{
		ANIMATION* pAni = new ANIMATION(*(m_pCurPrefabObj->m_pCurAni)); // ������ ������ ���� ����
		pInstance->m_pCurAni = pAni;
		pInstance->m_bPlay = true;

		pInstance->m_mapAni.clear();
		pInstance->m_mapAni.insert({L"stand_8", pInstance->m_pCurAni });
	}

	// ���� ������ ���� (���� Ŀ�� ��ġ�� �ش��ϴ� Ÿ���� ������ ������ ���� ���������� �������ش�).

	TILE* pTile = Get_TileInMousePos();
	
	if (nullptr != pTile && !pTile->bCheckUnit && (pTile->byOption == 0))
	{
		pTile->bCheckUnit = true;
		pInstance->m_vWorldPos = pTile->vPos;
		pInstance->m_pMainView = m_pMainView;
		
		m_vecObjInstances[(UINT)pInstance->m_eType].push_back(pInstance);
		return pInstance;
	
	}
	return nullptr;

}

void CMapTool_Tab2::Mouse_Move()
{
	//if (MAPTOOL_MODE::OBJ == m_eToolMode && m_pCurPrefabObj != nullptr)
	//{
	//	//m_pCurPrefabObj->Tool_Render(Get_Mouse());
	//	m_pMainView->Invalidate(FALSE);
	//}
}

void CMapTool_Tab2::Change_Mode(const MAPTOOL_MODE & _eMode)
{
	if (MAPTOOL_MODE::TILE == m_eToolMode && MAPTOOL_MODE::TILE != _eMode) // Ÿ�� �������� �ٸ� �������� �Ѿ�� ���
	{
		m_ListTile.SetCurSel(-1);
		m_tSelectTile = nullptr;
	}

	m_eToolMode = _eMode;
	m_pMainView->Invalidate(FALSE);
}

TILE * CMapTool_Tab2::Get_TileInMousePos()
{
	TERRIAN_TYPE eTerrian_Type = static_cast<TERRIAN_TYPE>(m_pFormView->m_pMapTool_Tab2->m_Combo_SelecMap.GetCurSel());
	int iIndex = 0;
	switch (eTerrian_Type)
	{
	case TERRIAN_TYPE::ACT1:
	{
		iIndex = m_pMainView->m_pTerrain_Act1_View->Get_TileIndex({ float(Get_Mouse().x + GetScrollPos(0)), float(Get_Mouse().y + GetScrollPos(1)), 0.f });
		return m_pMainView->m_pTerrain_Act1_View->m_vecActTile[iIndex];
	}
	break;
	case TERRIAN_TYPE::ACT2:
	{
		iIndex = m_pMainView->m_pTerrain_Act2_View->Get_TileIndex({ float(Get_Mouse().x + GetScrollPos(0)), float(Get_Mouse().y + GetScrollPos(1)), 0.f });
		return m_pMainView->m_pTerrain_Act2_View->m_vecActTile[iIndex];
	}
	break;
	case TERRIAN_TYPE::ACT3:
	{
		iIndex = m_pMainView->m_pTerrain_Act3_View->Get_TileIndex({ float(Get_Mouse().x + GetScrollPos(0)), float(Get_Mouse().y + GetScrollPos(1)), 0.f });
		return m_pMainView->m_pTerrain_Act3_View->m_vecActTile[iIndex];
	}
	break;
	default:
		break;
	}

	return nullptr;
}

void CMapTool_Tab2::OnNMClickMapObjListboxUnit(NMHDR *pNMHDR, LRESULT *pResult)
{
	// List Ctrl���� Ŭ�� �̺�Ʈ�� �߻��� ��� Ŭ���� �������� �ε����� �޾ƿ´�.
	// �׸��� �ƴ� �� ������ Ŭ���ߴٸ� -1�� ��ȯ�Ѵ�.
	
	Change_Mode(MAPTOOL_MODE::OBJ);

	int iIndex = 0, iObjType = 0;

	// 1. ����Ʈ ��Ʈ�ѿ��� ���õ� �ε����� �����´�.
	{
		LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		iIndex = pNMListView->iItem;

		if (0 > iIndex)
		{
			m_pCurPrefabObj = nullptr; // �ش� �������� �����̹Ƿ� �޸𸮸� �����ؼ��� �ȵȴ�.
			return;
		}
	}

	// 2. ���õ� �ε����� �������� �����´�.
	{
		iObjType = m_cComboBox_Obj.GetCurSel();

		if ((UINT)OBJ_TYPE::TYPEEND < iObjType || int(m_vecObjPrefabs[iObjType].size() - 1) < iIndex)
		{
			AfxMessageBox(L"Index Range Error");
			return;
		}

		if (nullptr != m_vecObjPrefabs[iObjType][iIndex])
		{
			m_pCurPrefabObj = m_vecObjPrefabs[iObjType][iIndex];
			m_pCurPrefabObj->m_bPlay = false;
		}
	}

	*pResult = 0;
}

#pragma endregion



































void CMapTool_Tab2::OnTimer(UINT_PTR nIDEvent)
{
	for (int i = 0; i < (UINT)OBJ_TYPE::TYPEEND; ++i)
	{
		for (auto& iter : m_vecObjInstances[i])
			if(nullptr != iter)
				iter->Move_Frame();
	}
	

	CDialogEx::OnTimer(nIDEvent);
}
