#pragma once
#include <iostream>
using namespace std;

#define VK_W	0x57
#define VK_A	0x41
#define VK_S	0x53
#define VK_D	0x44	// define keyboard constant for readable code

class Node
{
private:
	pair<int, int>	m_pos;	// entity position (y, x)
	Node*	m_pNext;		// next node pointer

public:
	Node(int y, int x) {
		m_pos.first = y;	// row
		m_pos.second = x;	// col
		m_pNext = NULL;		// set next pointer to NULL
	}

	pair<int, int>	getPos() { return m_pos; }		// get pos of node
	void	setPos(pair<int, int> pos) { 
		m_pos.first = pos.first;
		m_pos.second = pos.second;
	}	// set pos of node
	Node*	getNext() { return m_pNext; }			// get next node
	void	setNext(Node* ptr) { m_pNext = ptr; }	// set next node 
};

class LinkedList
{
private:
	int		m_length;	// snake length
	Node*	m_pHead;	// head of snake
	Node*	m_pTail;	// tail of snake

public:
	LinkedList();	// constructor
	~LinkedList();	// destructor

	bool IsEmpty() { return (m_pHead == NULL) ? true : false; }	// whether list is empty (ONLY use for initialize the list)
	Node* getHead() { return m_pHead; }	// return head node
	Node* getTail() { return m_pTail; }	// return tail nodes

	void InitSnake(int y, int x);	// initialize the snake
	void AppendNode(int y, int x);	// insert node at the end of the list
	void InsertHead(int y, int x);	// insert node at head of the list - for new head node
	void DeleteTail();				// delete old tail
	void AppendTail(int direction);	// append tail according to the direction of the snake

	int	GetTailDir();	// get current tail direction
	bool IsExist(string type, pair<int, int> target);	// whether pos is already exist (exclude or include head node) - for check BODY & FOOD CRASH!!

	void MoveUP();		// moving up	: W 
	void MoveLeft();	// moving left	: A
	void MoveDown();	// moving down	: S
	void MoveRight();	// moving right	: D
};