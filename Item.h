
#ifndef		_ITEM_H
#define		_ITEM_H

#include "head.h"

//struct POINT 
//{
//	int x, y;
//	POINT(int _x = 0, int _y = 0) : x(_x), y(_y) {}
//};

class CItem
{
public:
			CItem(/*HWND hWnd*/);
			~CItem();

private:
	//HWND	m_hWnd;
	int		m_nIndex;    // id	
	POINT	m_nPos;	  // λ��
	int		m_nWidth; // ��
	int		m_nHigh;  // ��
	int		m_nType;  // item����
	E_COLOR m_eColor;
	DWORD	m_dColor; // ��ɫ
public:

	void	InitItem(POINT p, E_COLOR color,int index = 0, int w=40, int h=40);
	void	SetPos(const POINT &p);
	POINT	GetPos();
	void	SetIndex(int index);
	int		GetIndex();
	void	SetColor(DWORD color);
	DWORD	GetColor();
	E_COLOR	GetEColor();
	DWORD CItem::GetColorbyEnum( E_COLOR color );
	//void	FillColor();
//-----------------------------------------------
};

#endif