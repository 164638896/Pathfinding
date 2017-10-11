#ifndef	 _GRID_H_
#define  _GRID_H_

#include "Item.h"
#include "head.h"

class CGrid
{
public:
	CGrid(HWND hWnd, HDC dc);
	~CGrid();

	enum EGRIDISVALID
	{
		E_UNUSED = -0x001, //未被使用
		E_ISUSED = 0xffff // 使用
	};
	//-----------------------------------------------
	//格子上的物品
private:
	HWND			m_hWnd;
	HDC				m_dc;
	typedef std::vector<CItem*>			ITEM_VEC;
	ITEM_VEC							m_VecItem;

public:
	void	AddItem(CItem* pItem/*, E_COLOR color*/);
	void	DelItem(int nIndex);
	void	ClearAllItem();

	CItem*	GetItem(int nIndex);
	CItem*	GetItem(const POINT& p);
	void	SetItem(const POINT& p, CItem* pItem);
	bool	MoveItem(const POINT& pFrom, const POINT& p); // 移动格子
	
	void	FillItemColor(POINT pos, DWORD dcolor);
	void	FillAllItemColor();
	void	updateAllItem();

	void clearAllColor();
	//-----------------------------------------------
	//格子
private:
	int		m_nIndex;
	int		m_nGridNum;
	int		m_nCol;
	int		m_nRow;
	int		m_nLastRow;
	int*	m_pGridMark;

public:
	void	InitGrid(int nGridNum, int nCol);
	void	SetAllGridUnUsed();
	void	SetGridIsUsed(int i, int itemId );
	void	SetGridIsUsed(const POINT& p, int itemId );
	bool	GetGridIsUsed(const POINT& p);
	bool	IsValidPos(const POINT& p);
	int		GetIndexByPos(const POINT& p);

private:
	typedef std::list<int>			FREEGRID_LIST;
	FREEGRID_LIST					m_ListFreeGrid;
public:
	void		AddFreeGrid(int n);
	void		DelFreeGrid(int n);
	int			GetFreeGridSize();
	POINT		GetFreeGirByIndex(int i);

	// 其他操作
	void		OnDrawGrid();
	void		onDrawRect(int nIndex, DWORD color,int w,int h);
	void		DrawTextNum(TCHAR* pch);
};

#endif