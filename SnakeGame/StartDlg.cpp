// StartDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SnakeGame.h"
#include "StartDlg.h"
#include "afxdialogex.h"


// StartDlg 대화 상자

IMPLEMENT_DYNAMIC(StartDlg, CDialogEx)

StartDlg::StartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STARTDLG, pParent)
{
}

StartDlg::~StartDlg()
{
}

void StartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(StartDlg, CDialogEx)
END_MESSAGE_MAP()


// StartDlg 메시지 처리기

