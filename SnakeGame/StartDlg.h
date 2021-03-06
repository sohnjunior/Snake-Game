#pragma once


// StartDlg 대화 상자

class StartDlg : public CDialogEx
{
	DECLARE_DYNAMIC(StartDlg)

public:
	StartDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~StartDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedExit();
};
