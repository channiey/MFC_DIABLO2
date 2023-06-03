// UnitTool_Player.cpp : implementation file
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool_Player.h"
#include "afxdialogex.h"


// UnitTool_Player dialog

IMPLEMENT_DYNAMIC(UnitTool_Player, CDialogEx)

UnitTool_Player::UnitTool_Player(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNIT_PLAYER, pParent)
{

}

UnitTool_Player::~UnitTool_Player()
{
}

void UnitTool_Player::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(UnitTool_Player, CDialogEx)
END_MESSAGE_MAP()


// UnitTool_Player message handlers
