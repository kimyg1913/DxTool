// CMenuDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "DxTool.h"
#include "CMenuDialog.h"
#include "afxdialogex.h"


// CMenuDialog 대화 상자

IMPLEMENT_DYNAMIC(CMenuDialog, CDialog)

CMenuDialog::CMenuDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CMenuDialog, pParent)
{

}

CMenuDialog::~CMenuDialog()
{
}

void CMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMenuDialog, CDialog)
END_MESSAGE_MAP()


// CMenuDialog 메시지 처리기
