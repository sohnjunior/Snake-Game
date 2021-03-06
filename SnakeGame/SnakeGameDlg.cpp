
// SnakeGameDlg.cpp: 구현 파일


#include "stdafx.h"
#include "SnakeGame.h"
#include "SnakeGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnakeGameDlg 대화 상자


CSnakeGameDlg::CSnakeGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAKEGAME_DIALOG, pParent)
{
	srand((unsigned int)time(NULL));	// random seed value
	m_food_pos = { 0, 0 };	// initialize the pos
	m_curdir = VK_A;		// initialize moving direction
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnakeGameDlg::DrawMap()
{
	// PURPOSE : create an image to show to user

	// set brush and pen for draw
	m_image_dc.SelectStockObject(DC_PEN);			// set line object
	m_image_dc.SelectStockObject(DC_BRUSH);			// set brush object
	m_image_dc.SetDCPenColor(RGB(48, 48, 48));		// set line color
	m_image_dc.SetDCBrushColor(RGB(0, 0, 0));		// set background color

	// draw game map
	for (int row = 0; row < MAP_SIZE; row++)
		for (int col = 0; col < MAP_SIZE; col++) {
			if (row == 0 || col == 0 || row == MAP_SIZE - 1 || col == MAP_SIZE - 1) {
				m_image_dc.SetDCBrushColor(RGB(48, 48, 48));	// set background color
				m_image_dc.Rectangle(col * PIXEL_SIZE, row * PIXEL_SIZE,
					col * PIXEL_SIZE + PIXEL_SIZE + 1, row * PIXEL_SIZE + PIXEL_SIZE + 1);
				m_image_dc.SetDCBrushColor(RGB(0, 0, 0));		// set background color
			}
			else {
				m_image_dc.Rectangle(col * PIXEL_SIZE, row * PIXEL_SIZE,
					col * PIXEL_SIZE + PIXEL_SIZE + 1, row * PIXEL_SIZE + PIXEL_SIZE + 1);
			}
		}
	
	// display executed time
	CTime curTime = CTime::GetCurrentTime();
	CTimeSpan diff = curTime - m_startTime;
	CString str = diff.Format(_T("SURVIVE : %H : %M : %S"));

	m_image_dc.SetBkColor(RGB(0, 0, 0));
	m_image_dc.SetTextColor(RGB(255, 255, 255));
	m_image_dc.TextOut(210, 610, str);

	// draw food
	POINT loc = m_food_pos;
	m_image_dc.SetDCBrushColor(RGB(255, 0, 0));	// set red color
	m_image_dc.Rectangle(loc.x * PIXEL_SIZE, loc.y * PIXEL_SIZE,
		loc.x * PIXEL_SIZE + PIXEL_SIZE + 1, loc.y * PIXEL_SIZE + PIXEL_SIZE + 1);
	
	// draw snake
	Node* pCur = m_Snake.getHead();
	while (pCur) {
		pair<int, int>	pos = pCur->getPos();	// get coordiante of node

		// HEAD OF SNAKE !! - Circle
		if (pCur == m_Snake.getHead()) {
			m_image_dc.SetDCBrushColor(RGB(0, 255, 0));	// set lime color
			m_image_dc.Ellipse(pos.second * PIXEL_SIZE, pos.first * PIXEL_SIZE,
				pos.second * PIXEL_SIZE + PIXEL_SIZE + 1, pos.first * PIXEL_SIZE + PIXEL_SIZE + 1);
		}
		// TAIL OF SNAKE !! - Circle
		else if (pCur == m_Snake.getTail()) {
			m_image_dc.SetDCBrushColor(RGB(0, 255, 255));	// set cyan color
			m_image_dc.Ellipse(pos.second * PIXEL_SIZE, pos.first * PIXEL_SIZE,
				pos.second * PIXEL_SIZE + PIXEL_SIZE + 1, pos.first * PIXEL_SIZE + PIXEL_SIZE + 1);
		}
		// BODY OF SNAKE !! - Rectangle
		else {
			m_image_dc.SetDCBrushColor(RGB(255, 255, 0));	// set yellow color
			m_image_dc.Rectangle(pos.second * PIXEL_SIZE, pos.first * PIXEL_SIZE,
				pos.second * PIXEL_SIZE + PIXEL_SIZE + 1, pos.first * PIXEL_SIZE + PIXEL_SIZE + 1);
		}

		pCur = pCur->getNext();
	}
}

void CSnakeGameDlg::GameOver(int type)
{
	// PURPOSE : Game Over
	// type :	 1 : 몸에 쿵! 0 : 벽에 쿵!
	
	KillTimer(1);	// turn off the timer

	CDC* pDC = GetDC();
	CString str;
	RECT rt = { 200, 200, 400, 400 };
	if(type)
		str.Format(L"Crash Body!\nGame Over\nRetry?(y/n)");
	else
		str.Format(L"Crash Wall!\nGame Over\nRetry?(y/n)");

	// draw text
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText(str, -1, &rt, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	ReleaseDC(pDC);

	// get the message from message queue
	bool restart = false;
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_KEYDOWN) {
			if (msg.wParam == VK_Y) {
				restart = true;
				break;
			}
			else if (msg.wParam == VK_N) {
				::SendMessage(GetSafeHwnd(), WM_CLOSE, NULL, NULL);	// exit program safely
				break;
			}
		}
	}

	if (restart) {
		// initialize moving direction
		m_curdir = VK_A;
		// initialize the snake at the middle of the map
		m_Snake.InitSnake(MAP_SIZE / 2, MAP_SIZE / 2);
		// initialize the start time
		m_startTime = CTime::GetCurrentTime();
		// create one food in random position
		MakeFood();
		DrawMap();				// draw the map
		SetTimer(1, 55, NULL);	// on 55ms update NO.1 timer
	}
}

void CSnakeGameDlg::MakeFood()
{
	// PURPOSE : prevent snake's position from overlapping
	pair<int, int> pos;

	while (1)
	{
		pos.first = rand() % (MAP_SIZE - 2) + 1;
		pos.second = rand() % (MAP_SIZE - 2) + 1;	// generate random food position

		// if food is not overlap with snake, BUT CAN EXIST IN SNAKE HEAD !! 
		if (!m_Snake.IsExist("FOOD", pos))
			break;
	}

	m_food_pos.y = pos.first;
	m_food_pos.x = pos.second;	// set food pos
}

void CSnakeGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSnakeGameDlg 메시지 처리기

BOOL CSnakeGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// set progam title
	SetWindowText(_T("SNAKE GAME ver2"));

	// initialize the snake at the middle of the map
	m_Snake.InitSnake(MAP_SIZE/2, MAP_SIZE/2);

	// create image and attatch to MEMORY DC !!
	m_image.Create(700, 700, 32);
	m_image_dc.Attach(m_image.GetDC());

	// create one food in random position
	MakeFood();

	m_startTime = CTime::GetCurrentTime();	// get start time
	DrawMap();						// draw the map
	SetTimer(1, 55, NULL);			// on 55ms update NO.1 timer

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSnakeGameDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_image.Draw(dc, 0, 0);
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSnakeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnakeGameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);			
	KillTimer(2);			// off the timer
	m_image_dc.Detach();	// detatch the momory dc
	m_image.ReleaseDC();	// release the CImage	
}


void CSnakeGameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// PURPOSE :	dispose keyboard input from user - determine the direction of snake
	//				change the direction only if possible case is input
	// nChar :		user input key

	switch (nChar)
	{
	case VK_W:	// want to moving up
		if (m_curdir != VK_S)
			m_curdir = VK_W;	// only if the current direction is not DOWN
		break;
	case VK_A:	// want to moving left
		if (m_curdir != VK_D)
			m_curdir = VK_A;	// only if the current direction is not LEFT
		break;
	case VK_S:	// want to moving down
		if (m_curdir != VK_W)
			m_curdir = VK_S;	// only if the current direction is not UP
		break;
	case VK_D:	// want to moving right
		if (m_curdir != VK_A)
			m_curdir = VK_D;	// only if the current direction is not RIGHT
		break;
	default:
		break;
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSnakeGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	//	PURPOSE : NO.1 Timer Action
	CClientDC dc(this);
	
	if (nIDEvent == 1) {
		// Update the position of the snake according to m_curdir (current snake's head direction)
		if (m_curdir == VK_W)	m_Snake.MoveUP();
		else if (m_curdir == VK_A)	m_Snake.MoveLeft();
		else if (m_curdir == VK_S)	m_Snake.MoveDown();
		else if (m_curdir == VK_D)	m_Snake.MoveRight();

		pair<int, int> hPOS = m_Snake.getHead()->getPos();	// get head node's updated coordinate (y, x)

		// If not out of range
		if ((0 < hPOS.first && hPOS.first < MAP_SIZE - 1) && (0 < hPOS.second && hPOS.second < MAP_SIZE - 1))
		{
			// compare the position of the head of the snake and the food to judge whether or not you ate it
			bool eat = false;	// Eat or Not
			if (hPOS.first == m_food_pos.y && hPOS.second == m_food_pos.x) {
				eat = true;						// if food position matches the snake's head
				int tail_dir = m_Snake.GetTailDir();	// get current tail direction
				m_Snake.AppendTail(tail_dir);	// INCREASE SNAKE SIZE !! (by append tail)
				MakeFood();						// create new food
			}

			// If you do not feed, check if you have hit your body.
			// If you bump into your body, '몸에 쿵!' occur
			if (!eat) {
				// If there is a node with the same coordinate value as the head node, the body is hit
				if (m_Snake.IsExist("EXTHEAD", hPOS))
					GameOver(1);
			}

			// draw updated map!
			DrawMap();					// update the map
			m_image.Draw(dc, 0, 0);		// display !!
		}
		// get out of range, '벽에 쿵!' error occurred
		else
			GameOver(0);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}
