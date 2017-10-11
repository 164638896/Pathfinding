#include "Item.h"

CItem::CItem()//HWND hWnd):m_hWnd(hWnd)
{

	m_nIndex = 0;   //Index	
	m_nPos.x = 0;	//位置
	m_nPos.y = 0;
	m_nWidth = 0;  // 宽
	m_nHigh  = 0;  // 高
	m_nType  = 0;  // item类型
	m_dColor = 0;  // 颜色
}

 CItem::~CItem()
{

}

void CItem::InitItem( POINT p, E_COLOR color, int index, int w, int h)
{
	m_nPos	 = p;
	m_eColor = color;
	m_nIndex = index;
	m_nWidth = w;  // 宽
	m_nHigh  = h;  // 高
	m_dColor = this->GetColorbyEnum(m_eColor);
}

void CItem::SetPos(const POINT &p)
{
	m_nPos = p;
}

POINT CItem::GetPos()
{
	return m_nPos;
}

void CItem::SetIndex(int index)
{
	m_nIndex = index;
}

int CItem::GetIndex()
{
	return m_nIndex;
}

void CItem::SetColor(DWORD color)
{
	m_dColor = color;
}

DWORD CItem::GetColor()
{
	m_dColor = this->GetColorbyEnum(m_eColor);
	return m_dColor;
}

E_COLOR CItem::GetEColor()
{
	return m_eColor;
}

DWORD CItem::GetColorbyEnum( E_COLOR color )
{
	switch(color)
	{
	case	E_GRAY:
		return RGB(128,128,128);
	case	E_RED:
		return RGB(255,0,0);
	case	E_GREEN:
		return RGB(0,255,0);
	case	E_BLUE:
		return RGB(0,0,255);
	case	E_YELLOW:
		return RGB(255,255,0);
	case    E_PURPLE:
		return RGB(255,0,255);
	case	E_CYAN: 
		return RGB(0,255,255);
	case	E_BLACK:
		return RGB(0,0,0);
	case	E_ORANGE:
		return RGB(255,150,0);
	default:
		return CLEAR_COLOR;
	}
}

//void CItem::FillColor()
//{
//
//	POINT temppos;
//	temppos.x = m_nPos.x * GIRD_SIZE + 5;
//	temppos.y = m_nPos.y * GIRD_SIZE + 5;
//
//	//绘制
//	RECT  PicRect;
//	HBRUSH  SnakeBrush = CreateSolidBrush(m_dColor);
//	SetRect(&PicRect, temppos.x, temppos.y, temppos.x +PIC_SIZE, temppos.y +PIC_SIZE);
//
//}