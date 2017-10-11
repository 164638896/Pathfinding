#ifndef  GRID_MGR_H
#define  GRID_MGR_H

#include "Grid.h"
#include "Item.h"
#include "File.h"
#include "head.h"
#include "pathfinding/Pathfinding.h"
#include "glm/glm.hpp"

class CGridMgr
{
public:
	CGridMgr();
	~CGridMgr();

private:
	HWND		m_hWnd;
	HDC			m_dc;
	CGrid*		m_pGrid;
	CItem*		m_ActItem;
	CItem*		m_pItem[RAND_GRID];
	IAStar*		m_pAstar;
	CFile*		m_pFile;
	int			m_nWinNum;
	int			m_nWinGridIndex[GRID_HEIGHT];

	int			m_nNum;
	int			m_OldIndex;
public:
	void		InitGame(HWND hWnd); // 初始化格子
	void		RepeatWindow();
	void		RandGrid(); // 随即格子
	E_COLOR		RandColor();
	POINT		RandPos();
	void		OnMouseMove();
	void		OnButtonDown();
	POINT		GetMouseGirdPos();
	void		RendGrid();
	//void		RendActItem();
	bool		updatePos(POINT& nPos, CItem* pItem );
	void		Count();
	bool		Go(POINT& nPos, CItem* pItem );
	bool		Way(POINT& pos, int l, int r);
	void		ClearByIndex(int nIndex);
	void		ClearAll();
	void		ColorTransform(POINT p);
	void		ClearGridIndex();

	void		OnDrawGrid();
	void		DrawTextNum(TCHAR* pch);

	void OnRButtonDown();
	void addGrid(int x, int y);

	void updateUi();

	int getIndex(int x, int y);
	void refreshRole(POINT& rolePos, glm::ivec2& size, DWORD dcolor);
};
#endif