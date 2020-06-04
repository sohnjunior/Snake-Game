#include "stdafx.h"
#include "LinkedList.h"



LinkedList::LinkedList()
{
	m_length = 0;
	m_pHead = NULL;
	m_pTail = NULL;
}

LinkedList::~LinkedList()
{
	Node* pCur = m_pHead;	// walker
	Node* pDel = NULL;		// deleted target

	while (pCur) {
		pDel = pCur;
		pCur = pCur->getNext();
		delete pDel;	// memory deallocate - all nodes of snake
	}
}

void LinkedList::InitSnake(int y, int x)
{
	// snake must has at least 3 node... head, body, tail
	// so, insert 3 node at list (w/ pos (y, x)) by this function before use SNAKE(list instance)!!

	// if list has node, remove that nodes...
	if (!IsEmpty()) {
		Node* pCur = m_pHead;	// walker
		Node* pDel = NULL;		// deleted target
		while (pCur) {
			pDel = pCur;
			pCur = pCur->getNext();
			delete pDel;	// memory deallocate - all nodes of snake
		}
		m_length = 0;
		m_pHead = NULL;
		m_pTail = NULL;
	}

	AppendNode(y, x);		// HEAD OF SNAKE !!
	AppendNode(y, x + 1);	// BODY OF SNAKE !!
	AppendNode(y, x + 2);	// TAIL OF SNAKE !!
}

void LinkedList::AppendNode(int y, int x)
{
	Node* NewNode = new Node(y, x);	// create new node

	// for initialize the snake
	if (IsEmpty()) {
		m_pHead = NewNode;
		m_pTail = NewNode;	// set new head & tail
	}
	// otherwise
	else {
		m_pTail->setNext(NewNode);
		m_pTail = NewNode;	// set new tail
	}

	m_length++;	// length increase
}

void LinkedList::InsertHead(int y, int x)
{
	Node* NewNode = new Node(y, x);	// create new node

	NewNode->setNext(m_pHead);	// NewNode is new head of the list
	m_pHead = NewNode;			// set new head
	m_length++;	// length increase
}

void LinkedList::DeleteTail()
{
	Node* NewTail = NULL;
	Node* pCur = m_pHead;

	// find the prev node of tail (new tail)
	while (pCur->getNext() != m_pTail)
		pCur = pCur->getNext();
	NewTail = pCur;
	NewTail->setNext(NULL);

	delete m_pTail;		// delete old tail
	m_pTail = NewTail;	// setting new tail
	m_length--;			// length decrease
}

void LinkedList::AppendTail(int direction)
{
	Node* NewNode = NULL;
	pair<int, int> tPOS = m_pTail->getPos();	// get current tail coordinate

	if (direction == VK_S)
		NewNode = new Node(tPOS.first + 1, tPOS.second);	// creates tail at bottom
	else if (direction == VK_A)
		NewNode = new Node(tPOS.first, tPOS.second + 1);	// creates tail at rightmost
	else if (direction == VK_W)
		NewNode = new Node(tPOS.first - 1, tPOS.second);	// creates tail at top
	else
		NewNode = new Node(tPOS.first, tPOS.second - 1);	// creates tail at leftmost

	m_pTail->setNext(NewNode);	// append new tail
	m_pTail = NewNode;			// setting new tail
	m_length++;					// length increase
}


int LinkedList::GetTailDir()
{
	int tail_dir = 0;

	Node* pTail = m_pTail;	// current tail
	Node* pPrev = m_pHead;	// prev node of tail
	while (pPrev->getNext() != m_pTail)
		pPrev = pPrev->getNext();	// find the prev node of tail

	pair<int, int> tail_pos = pTail->getPos();	// tail coordinate
	pair<int, int> prev_pos = pPrev->getPos();	// prev coordinate

	// if y is same, then tail is horizontal
	if (tail_pos.first == prev_pos.first) {
		if ((tail_pos.second - prev_pos.second) > 0)
			tail_dir = VK_D;	// tail is right side
		else
			tail_dir = VK_A;	// tail is left side
	}
	// if x is same, then tail is vertical
	else if (tail_pos.second == prev_pos.second) {
		if ((tail_pos.first - prev_pos.first) > 0)
			tail_dir = VK_S;	// tail is down side
		else
			tail_dir = VK_W;	// tail is up side	
	}

	return tail_dir;
}

bool LinkedList::IsExist(string type, pair<int, int> target)
{
	Node* pCur = NULL;

	if (type == "FOOD")
		pCur = m_pHead;				// FOOD : start with head
	else
		pCur = m_pHead->getNext();	// EXTHEAD : start with body

	pair<int, int> temp;
	bool found = false;

	while (pCur) {
		temp = pCur->getPos();
		if (temp.first == target.first && temp.second == target.second) {
			found = true;
			break;	// if matches node found, set flag to TRUE and return immediately
		}
		pCur = pCur->getNext();
	}

	return found;	// FOUND or NOT FOUND
}

void LinkedList::MoveUP()
{
	// delete old tail
	DeleteTail();

	// create a new head position
	pair<int, int> nHPOS;	// new pos
	pair<int, int> oHPOS;	// old pos
	oHPOS = m_pHead->getPos();
	nHPOS.first = oHPOS.first - 1;	// moving up!
	nHPOS.second = oHPOS.second;

	// insert new head
	InsertHead(nHPOS.first, nHPOS.second);
}

void LinkedList::MoveLeft()
{
	// delete old tail
	DeleteTail();

	// create a new head position
	pair<int, int> nHPOS;	// new pos
	pair<int, int> oHPOS;	// old pos
	oHPOS = m_pHead->getPos();
	nHPOS.first = oHPOS.first;	
	nHPOS.second = oHPOS.second - 1;	// moving left!

	// insert new head
	InsertHead(nHPOS.first, nHPOS.second);
}

void LinkedList::MoveDown()
{
	// delete old tail
	DeleteTail();

	// Create a new head position
	pair<int, int> nHPOS;	// new pos
	pair<int, int> oHPOS;	// old pos
	oHPOS = m_pHead->getPos();
	nHPOS.first = oHPOS.first + 1;	// moving down!
	nHPOS.second = oHPOS.second;

	// Insert new head
	InsertHead(nHPOS.first, nHPOS.second);
}

void LinkedList::MoveRight()
{
	// delete old tail
	DeleteTail();

	// Create a new head position
	pair<int, int> nHPOS;	// new pos
	pair<int, int> oHPOS;	// old pos
	oHPOS = m_pHead->getPos();
	nHPOS.first = oHPOS.first;	
	nHPOS.second = oHPOS.second + 1;	// moving right!

	// Insert new head
	InsertHead(nHPOS.first, nHPOS.second);
}
