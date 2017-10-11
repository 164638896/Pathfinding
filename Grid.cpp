#include "Grid.h"

 CGrid::CGrid(HWND hWnd, HDC dc):m_hWnd(hWnd),m_dc(dc)
{
	m_pGridMark = NULL;
}

 CGrid::~CGrid()
{
	SAFE_DELETE(m_pGridMark);
}

//-----------------------------------------------
//物品操作
void CGrid::AddItem(CItem* pItem/*, E_COLOR color*/)
{
	//if(!this->GetGridIsUsed(p))
	{
		//int nIndex = p.x + m_nCol * p.y;
		//CItem* pItem = new CItem(/*m_hWnd*/);
		//pItem->InitItem(p, color, nIndex);

		m_VecItem.push_back(pItem);

		//int nIndex = pItem->GetIndex();
		//m_pGridMark[nIndex] = pItem->GetIndex();	

		//DelFreeGrid(nIndex);
	}
}

void CGrid::DelItem(int nIndex)
{
	ITEM_VEC::iterator iter;
	for ( iter = m_VecItem.begin(); iter != m_VecItem.end(); ++iter)
	{
		CItem* pItem = *iter;
		if (pItem && pItem->GetIndex() == nIndex )
		{
			SAFE_DELETE(pItem);
			m_VecItem.erase(iter);
			break;
		}
	}
}

void CGrid::ClearAllItem()
{
	ITEM_VEC::iterator iter;
	for ( iter = m_VecItem.begin(); iter != m_VecItem.end(); ++iter)
	{
		CItem* pItem = *iter;
		if (pItem)
		{
			SAFE_DELETE(pItem);
		}
	}
	m_VecItem.clear();
}

CItem* CGrid::GetItem(int nIndex)
{
	ITEM_VEC::iterator iter;
	for ( iter = m_VecItem.begin(); iter != m_VecItem.end(); ++iter)
	{
		CItem* pItem = *iter;
		if (pItem && pItem->GetIndex() == nIndex)
		{
			return pItem;	
		}
	}

	return NULL;
}

CItem* CGrid::GetItem(const POINT& p)
{
	if (!this->IsValidPos(p))
	{
		return NULL;
	}
	int nIndex = p.x + m_nCol * p.y;
	return this->GetItem(nIndex);
}

void CGrid::SetItem(const POINT& p, CItem* pItem)
{
	if (!this->IsValidPos(p) || pItem == NULL)
	{
		return;
	}

	int nIndex = p.x + m_nCol * p.y;
	pItem->SetIndex(nIndex);
	pItem->SetPos(p);
	
	m_pGridMark[nIndex] = pItem->GetIndex();
	DelFreeGrid(nIndex);
	
}

// 
bool CGrid::MoveItem(const POINT& pFrom, const POINT& pTo)
{
	// 判断位置
	if (this->GetGridIsUsed(pTo))
	{
		return false;
	}

	int nIndex = pFrom.x + m_nCol * pFrom.y;

	CItem * pItem = GetItem(nIndex);
	if (pItem)
	{
		SetItem(pTo, pItem);
		m_pGridMark[nIndex] = E_UNUSED;
		AddFreeGrid(nIndex);
	}
	return false;
}

void CGrid::FillItemColor(POINT pos, DWORD dcolor)
{
	POINT temppos;
	temppos.x = pos.x * GIRD_SIZE + 5;
	temppos.y = pos.y * GIRD_SIZE + 5;
	//绘制
	RECT  PicRect;
	HBRUSH  BrushHandle = CreateSolidBrush(dcolor);
	SetRect(&PicRect, temppos.x, temppos.y, temppos.x +PIC_SIZE, temppos.y +PIC_SIZE);
	FillRect(m_dc, &PicRect, BrushHandle); 
	DeleteObject(BrushHandle);
}

void CGrid::FillAllItemColor()
{
	ITEM_VEC::iterator iter;
	for ( iter = m_VecItem.begin(); iter != m_VecItem.end(); ++iter)
	{
		CItem* pItem = *iter;
		if (pItem)
		{
			FillItemColor(pItem->GetPos(), pItem->GetColor());
			//POINT Pos = pItem->GetPos();
			//Pos.x = Pos.x * GIRD_SIZE + 5;
			//Pos.y = Pos.y * GIRD_SIZE + 5;

			////绘制
			//RECT  PicRect;
			//HBRUSH  SnakeBrush = CreateSolidBrush(pItem->GetColor());
			//SetRect(&PicRect, Pos.x, Pos.y, Pos.x +PIC_SIZE, Pos.y +PIC_SIZE);
			//FillRect(m_dc, &PicRect, SnakeBrush); 
		}
	}
}

//-----------------------------------------------
// 格子操作
void CGrid::InitGrid(int nGridNum, int nCol)
{
	m_nGridNum = nGridNum;
	m_nCol = nCol;

	if (nCol > 0)
	{
		m_nLastRow = m_nGridNum % m_nCol;
		m_nRow = nGridNum / m_nCol;
	}

	if (m_nLastRow  > 0)
	{
		m_nRow +=1;
	}

	if (m_pGridMark)
	{
		SAFE_DELETE(m_pGridMark);
	}

	m_pGridMark = new int[m_nRow * m_nCol];
	//memset(m_pGridMark, E_UNUSED, sizeof(int)*(m_nRow * m_nCol));
	SetAllGridUnUsed();

	if (m_nLastRow)
	{
		int nTemp = m_nCol * m_nRow;
		for(int i = m_nGridNum; i < nTemp; ++i)
		{
			m_pGridMark[i] = E_ISUSED;
		}
	}

	int nTemp = m_nCol * m_nRow;
	for(int i = 0; i < nTemp; ++i)
	{
		AddFreeGrid(i);
	}

	// 清楚下item
	this->ClearAllItem();
}

void CGrid::SetAllGridUnUsed()//
{
	for(int i=0; i<m_nGridNum; ++i)
	{
		m_pGridMark[i] = E_UNUSED;
	}
}

void CGrid::SetGridIsUsed(int i, int itemId )//
{
	m_pGridMark[i] = itemId;
}

void CGrid::SetGridIsUsed(const POINT& p, int itemId )//
{
	if (!this->IsValidPos(p))
	{
		return;
	}
	SetGridIsUsed(p.x + m_nCol * p.y, itemId );
}

bool CGrid::GetGridIsUsed(const POINT& p)//
{
	if (!this->IsValidPos(p))
	{
		return true; // 超出的只能算已被使用了
	}

	if (m_pGridMark[p.x + m_nCol*p.y] != E_UNUSED)
	{ 
		return true;
	}

	// false 未被使用
	return false;
}

bool CGrid::IsValidPos(const POINT& p)//
{
	if (p.x < 0 || p.y < 0 || p.x >= m_nCol || p.y >= m_nRow)
	{
		return false;
	}
	return true;
}

int	CGrid::GetIndexByPos(const POINT& p)//
{
	if (!this->IsValidPos(p))
	{
		return -1;
	}

	return p.x + m_nCol*p.y;
}

void CGrid::AddFreeGrid(int n)
{
	//m_MapFreeGrid[n] = n;
	//m_MapFreeGrid.insert(FREEGRID_MAP::value_type(n,n));
	//m_VecFreeGrid.push_back(n);
	//m_SetFreeGrid.insert(n);
	//m_VecFreeGrid.insert( m_VecFreeGrid.begin()+n, n);
	m_ListFreeGrid.push_back(n);
}

void CGrid::DelFreeGrid(int n)
{
	//FREEGRID_MAP::iterator iter;
	//iter = m_MapFreeGrid.find(n);
	//if (iter != m_MapFreeGrid.end())
	//{
	//	m_MapFreeGrid.erase(iter);
	//}

	//FREEGRID_VEC::iterator iter;
	//if (n>=0 && n < (int)m_VecFreeGrid.size())
	//{
	//	iter = m_VecFreeGrid.begin()+n;

	//	if (iter != m_VecFreeGrid.end())
	//	{
	//		m_VecFreeGrid.erase(iter);
	//	}
	//	else
	//	{
	//		assert(0);
	//	}
	//}

	//FREEGRID_SET::iterator iter;
	//iter = m_SetFreeGrid.find(n);

	//if (iter != m_SetFreeGrid.end())
	//{
	//	m_SetFreeGrid.erase(iter);
	//}
	//else
	//{
	//	assert(0);
	//}

	m_ListFreeGrid.remove(n);
}	

int	CGrid::GetFreeGridSize()
{
	return  m_ListFreeGrid.size();
}

POINT CGrid::GetFreeGirByIndex(int i)
{
	POINT p; p.x =-1; p.y =-1;

	//FREEGRID_VEC::iterator iter;
	//if (i>=0 && i < (int)m_VecFreeGrid.size())
	//{
	//	iter = m_VecFreeGrid.begin()+i;

	//	if (iter != m_VecFreeGrid.end())
	//	{	p.x = *iter % m_nRow;
	//		p.y = *iter / m_nCol;
	//	}
	//	else
	//	{
	//		assert(0);
	//	}

	//}
	//return p;

	//FREEGRID_MAP::iterator iter;
	////iter = m_MapFreeGrid.find(i);
	// 
	//iter = m_MapFreeGrid.begin() + i;
	//if (iter != m_MapFreeGrid.end())
	//{
	//	p.x = iter->second % m_nRow;
	//	p.y = iter->second / m_nCol;

	//}
	//else
	//{
	//	assert(0);
	//}
	//return p;

	//FREEGRID_SET::iterator iter;
	//iter = m_SetFreeGrid.find(i);
	// 
	//if (iter != m_SetFreeGrid.end())
	//{
	//	p.x = *iter % m_nRow;
	//	p.y = *iter / m_nCol;

	//}
	//else
	//{
	//	assert(0);
	//}
	//return p;

	int n = 0;
	FREEGRID_LIST::iterator iter;
	for(iter = m_ListFreeGrid.begin(); iter != m_ListFreeGrid.end(); ++iter, ++n)
	{
		if (n == i)
		{
			if (iter != m_ListFreeGrid.end())
			{	p.x = *iter % m_nRow;
				p.y = *iter / m_nCol;
			}
			else
			{
				assert(0);
			}

			break;
		}
	}

	return p;
}


//-----------------------------------------------------------
//其他
void CGrid::OnDrawGrid()
{
	for(int i=0; i<=GRID_WIDTH; ++i)
	{
		MoveToEx(m_dc,i*GIRD_SIZE,0,NULL);
		LineTo(m_dc,i*GIRD_SIZE,500);

		MoveToEx(m_dc,0,i*GIRD_SIZE,NULL);
		LineTo(m_dc,500,i*GIRD_SIZE);
	}
}

void CGrid::onDrawRect(int nIndex, DWORD color,int w,int h)
{
	POINT nPos;
	nPos.x = nIndex % m_nCol;
	nPos.y = nIndex / m_nCol;

	int nX,nY;
	//nX = (GIRD_SIZE - w) / 2;
	//nY = (GIRD_SIZE - h) / 2;
	nX = GIRD_SIZE - w;
	nY = GIRD_SIZE - h;


	HGDIOBJ  hOld = SelectObject(m_dc,GetStockObject(DC_PEN));
	SetDCPenColor(m_dc,color);

	MoveToEx(m_dc,nPos.x*GIRD_SIZE + nX, nPos.y*GIRD_SIZE + nY,NULL);
	LineTo(m_dc,nPos.x*GIRD_SIZE + w, nPos.y*GIRD_SIZE + nY);
	LineTo(m_dc,nPos.x*GIRD_SIZE + w, nPos.y*GIRD_SIZE + h);
	LineTo(m_dc,nPos.x*GIRD_SIZE + nX, nPos.y*GIRD_SIZE + h);
	LineTo(m_dc,nPos.x*GIRD_SIZE + nX, nPos.y*GIRD_SIZE + nY);

	SelectObject(m_dc,hOld);

	//if (m_OldIndex != nIndex)
	//{
	//	HGDIOBJ  hOld=SelectObject(m_dc,GetStockObject(DC_PEN));
	//	SetDCPenColor(m_dc,CLEAR_COLOR);

	//	POINT nPos;
	//	nPos.x = m_OldIndex % m_nCol;
	//	nPos.y = m_OldIndex / m_nCol;

	//	MoveToEx(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 3,NULL);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 47, nPos.y*GIRD_SIZE + 3);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 47, nPos.y*GIRD_SIZE + 47);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 47);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 3);

	//	m_OldIndex = nIndex;

	//	SetDCPenColor(m_dc,RGB(255,0,0));

	//	nPos.x = nIndex % m_nCol;
	//	nPos.y = nIndex / m_nCol;

	//	MoveToEx(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 3,NULL);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 47, nPos.y*GIRD_SIZE + 3);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 47, nPos.y*GIRD_SIZE + 47);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 47);
	//	LineTo(m_dc,nPos.x*GIRD_SIZE + 3, nPos.y*GIRD_SIZE + 3);

	//	SelectObject(m_dc,hOld);
	//}

}

void CGrid::DrawTextNum(TCHAR* pch)
{
	RECT rect;
	rect.left = 510;
	rect.top = 35;
	rect.right = 510 + 100;
	rect.bottom = 35 + 50;

	InvalidateRect(m_hWnd, &rect, true);
	UpdateWindow(m_hWnd);

	TextOut( m_dc, 510, 35, pch, strlen(pch) );
}

void CGrid::clearAllColor()
{
	for (int x = 0; x < m_nCol; ++x)
	{
		for (int y = 0; y < m_nRow; ++y)
		{
			POINT p;
			p.x = x;
			p.y = y;
			FillItemColor(p, CLEAR_COLOR);
		}
	}

	DrawTextNum("");
}

void CGrid::updateAllItem()
{
	for (int x = 0; x < m_nCol; ++x)
	{
		for (int y = 0; y < m_nRow; ++y)
		{
			POINT p;
			p.x = x;
			p.y = y;

			CItem* pItem = GetItem(p);
			if (pItem)
			{
				FillItemColor(pItem->GetPos(), pItem->GetColor());
			}
			else
			{
				FillItemColor(p, CLEAR_COLOR);
			}
		}
	}
}
