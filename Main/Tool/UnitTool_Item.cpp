// UnitTool_Item.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool_Item.h"
#include "afxdialogex.h"


// UnitTool_Item dialog

IMPLEMENT_DYNAMIC(UnitTool_Item, CDialogEx)

UnitTool_Item::UnitTool_Item(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNIT_ITEM, pParent)
{

}

UnitTool_Item::~UnitTool_Item()
{
}

void UnitTool_Item::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UnitTool_Item, CDialogEx)
END_MESSAGE_MAP()


// UnitTool_Item message handlers
