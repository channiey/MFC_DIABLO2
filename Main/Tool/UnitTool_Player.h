#pragma once


// UnitTool_Player dialog

class UnitTool_Player : public CDialogEx
{
	DECLARE_DYNAMIC(UnitTool_Player)

public:
	UnitTool_Player(CWnd* pParent = NULL);   // standard constructor
	virtual ~UnitTool_Player();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIT_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
