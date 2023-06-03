#pragma once


// UnitTool_Item dialog

class UnitTool_Item : public CDialogEx
{
	DECLARE_DYNAMIC(UnitTool_Item)

public:
	UnitTool_Item(CWnd* pParent = NULL);   // standard constructor
	virtual ~UnitTool_Item();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIT_ITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
