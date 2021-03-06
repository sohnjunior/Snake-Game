
// SnakeGameDlg.h: 헤더 파일

#include "LinkedList.h"

#pragma once

#define VK_Y	0x59
#define VK_N	0x4E
#define VK_W	0x57
#define VK_A	0x41
#define VK_S	0x53
#define VK_D	0x44	// define keyboard constant for readable code

#define MAP_SIZE	35	// map size (35 x 35)
#define PIXEL_SIZE	17	// pixel size to display

// CSnakeGameDlg 대화 상자
class CSnakeGameDlg : public CDialogEx
{
private:
	CImage	m_image;
	CDC		m_image_dc;		// DC object for client section - for DOUBLE BUFFERING...!

	POINT	m_food_pos;		// food coordinate

	int		m_curdir;		// snake current direction
	
	LinkedList	m_Snake;	// snake instance

	CTime	m_startTime;	// program start time

// 생성입니다.
public:
	CSnakeGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	void DrawMap();				// draw the map
	void GameOver(int type);	// game over
	void MakeFood();			// make food in random coordinate

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKEGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
