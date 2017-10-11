#include "GridMgr.h"
#include "data/MapData.h"
#include "ClientPathfindingMap.h"


CGridMgr::CGridMgr()
{
	m_pGrid   = NULL;
	m_ActItem = NULL;
	//m_pAstar  = NULL;
	m_pFile	  = NULL;
	m_nNum	  = 0;
	m_OldIndex =-1;
	m_dc = NULL;
}

CGridMgr::~CGridMgr()
{
	ReleaseDC(m_hWnd, m_dc);
	//SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pFile);
}


void CGridMgr::InitGame(HWND hWnd)
{
	srand((unsigned)time(NULL)); //随机打乱数据
	m_hWnd	=hWnd;
	m_dc = GetDC(m_hWnd);
	m_pGrid = new CGrid(hWnd, m_dc);
	m_pGrid->InitGrid(GRID_WIDTH*GRID_HEIGHT,GRID_WIDTH);
	MapData::getInstance().resizeMap(GRID_WIDTH, GRID_HEIGHT);

	ClientPathfindingMap* pMap = new ClientPathfindingMap();
	pMap->setMapData(&MapData::getInstance());
	Pathfinding::getInstance().setPathfindingMap(pMap);
	FlightPathfinding::getInstance().setPathfindingMap(pMap);

	m_pFile = new CFile();

	POINT p;
	p.x = 11; 
	p.y = 1;
	for (int i=0; i<RAND_GRID; ++i)
	{	
		p.y++;
		int nIndex = p.x + GRID_WIDTH * p.y;
		m_pItem[i] = new CItem();
		E_COLOR color = RandColor();
	
		m_pItem[i]->InitItem(p, color, nIndex);
		//m_pGrid->AddItem(p,color);
	}
	//m_pGrid->FillItemColor();
	//this->RandGrid();
	//m_pGrid->FillAllItemColor();


	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		E_COLOR color = RandColor();
		CItem* pItem = new CItem;

		POINT pos;
		pos.x = i;
		pos.y = 9;
		int nIndex = getIndex(pos.x, pos.y);
		pItem->InitItem(pos, color, nIndex);
		m_pGrid->AddItem(pItem);
		m_pGrid->SetItem(pos, pItem);
		MapData::getInstance().setMapTile(pos.x, pos.y, 1);
	}


	this->RepeatWindow();
}

void CGridMgr::RepeatWindow()
{
	if (m_pGrid)
	{	
		m_pGrid->OnDrawGrid();
		m_pGrid->FillAllItemColor();
		this->Count();
		
		for (int i=0; i<RAND_GRID; ++i)
		{	
			m_pGrid->FillItemColor(m_pItem[i]->GetPos(), m_pItem[i]->GetColor());
		}
	}

}

void CGridMgr::OnMouseMove()
{
	if (m_pGrid)
	{	
		POINT p = GetMouseGirdPos();
		int nIndex = m_pGrid->GetIndexByPos(p);
		if (m_OldIndex != nIndex)
		{	
			m_pGrid->onDrawRect(m_OldIndex,CLEAR_COLOR,46,46);
			m_pGrid->onDrawRect(nIndex,RGB(255,0,0),46,46);
			m_OldIndex = nIndex;
		}

	}
}

void CGridMgr::OnButtonDown()
{
	glm::ivec2 roleSize{ 1, 2 };

	if (m_pGrid == NULL)
		return;

	POINT p = GetMouseGirdPos();
	int nIndex = m_pGrid->GetIndexByPos(p);
	if (nIndex < 0)
		return;

	updateUi();

	// 被使用了
	if(m_pGrid->GetGridIsUsed(p))
	{
		m_ActItem = m_pGrid->GetItem(nIndex);
		if (m_ActItem)
		{	
			refreshRole(m_ActItem->GetPos(), roleSize, m_ActItem->GetColor());
			m_pGrid->onDrawRect(m_ActItem->GetIndex(),RGB(0,255,255),44,44);
			m_pGrid->onDrawRect(m_ActItem->GetIndex(),RGB(255,0,255),43,43);
			m_pGrid->onDrawRect(m_ActItem->GetIndex(),RGB(255,255,0),42,42);
			//PlaySound("sound\\tada.wav", NULL, SND_FILENAME | SND_ASYNC);
		}

	}
	else // 未被使用
	{
		// 有item 在手
		if (m_ActItem)
		{
			//NODE* path;
			//NODE* pHead;
			//NODE* pTeam;
			//POINT temppos;
			//path = AStar::getInstance().findPath(m_ActItem->GetPos().x,m_ActItem->GetPos().y, p.x, p.y);
			//if (path)
			//{

			//	pTeam = path;
			//	while (pTeam->Parent != NULL)
			//	{	
			//		pHead = pTeam->Parent;
			//		pHead->NextNode = pTeam;
			//		pTeam = pTeam->Parent;
			//	}
			//	path->NextNode = NULL;

			//	m_pGrid->FillItemColor(m_ActItem->GetPos(),CLEAR_COLOR);	// 删除
			//	while(pHead != NULL)
			//	{

			//		temppos.x = pHead->x;
			//		temppos.y = pHead->y;

			//		if(m_pGrid->GetItem(temppos) == NULL)
			//		{
			//			m_pGrid->FillItemColor(temppos,m_ActItem->GetColor());	//走动
			//			Sleep(30);
			//			m_pGrid->FillItemColor(temppos,m_ActItem->GetColor()/2);
			//			Sleep(30);
			//			m_pGrid->FillItemColor(temppos,CLEAR_COLOR);	
			//		}
			//		else
			//		{
			//			Sleep(100);
			//		}

			//		pHead = pHead->NextNode;

			//	}

			MapData::getInstance().setMapTile(m_ActItem->GetPos().x, m_ActItem->GetPos().y, 0);
		
			int i = 0;
			std::vector<glm::ivec2> vPath;
			//bool bFind = Pathfinding::getInstance().getFindPath(vPath, glm::ivec2(m_ActItem->GetPos().x, m_ActItem->GetPos().y), glm::ivec2(p.x, p.y), roleSize, 3);
			bool bFind = FlightPathfinding::getInstance().getFindPath<glm::ivec2>(vPath, glm::ivec2(m_ActItem->GetPos().x, m_ActItem->GetPos().y), glm::ivec2(p.x, p.y), roleSize);
			if (bFind)
			{
				TCHAR str[64];
				//int nNum = Pathfinding::getInstance().getGenerateNumber();
				int nNum = FlightPathfinding::getInstance().getGenerateNumber();
				sprintf(str, "amout:%d", nNum);
				m_pGrid->DrawTextNum(str);

				refreshRole(m_ActItem->GetPos(), roleSize, CLEAR_COLOR);

				for (std::vector<glm::ivec2>::reverse_iterator itor = vPath.rbegin(); itor != vPath.rend(); ++itor)
				{
					const glm::ivec2& vPos = *itor;
					POINT temppos;
					temppos.x = vPos.x;
					temppos.y = vPos.y;

					if (m_pGrid->GetItem(temppos) == NULL)
					{
						refreshRole(temppos, roleSize, m_ActItem->GetColor());
						Sleep(100);
						refreshRole(temppos, roleSize, m_ActItem->GetColor() / 2);
						Sleep(100);
						refreshRole(temppos, roleSize, CLEAR_COLOR);
					}
					else
					{
						Sleep(100);
					}

					TCHAR str[64];
					sprintf(str, "%d", ++i);
					TextOut(m_dc, temppos.x * GIRD_SIZE + 20, temppos.y * GIRD_SIZE + 20, str, strlen(str));
				}

				//go
				std::vector<glm::ivec2>::iterator itor = vPath.begin();
				if (itor != vPath.end())
				{
					POINT currPos = m_ActItem->GetPos();
					glm::ivec2& vPos = *itor;

					POINT newPos;
					newPos.x = vPos.x;
					newPos.y = vPos.y;

					m_pGrid->MoveItem(currPos, newPos);
					refreshRole(newPos, roleSize, m_ActItem->GetColor());

					MapData::getInstance().setMapTile(currPos.x, currPos.y, 0);

					//CAStar::TS_NODE_INFO roadInfo;
					//roadInfo.nNodeCost = 1;
					//AStar::getInstance().addRoadNode(AStar::getInstance().getIndexByPos(currPos.x, currPos.y), roadInfo);

					if (this->updatePos(newPos, m_ActItem))
					{
						this->Count();
					}
					else
					{
						Sleep(200);
						//this->RandGrid();
					}

					m_pGrid->FillAllItemColor();

					m_ActItem = NULL;
				}
			}
			else
			{
				MapData::getInstance().setMapTile(m_ActItem->GetPos().x, m_ActItem->GetPos().y, 1);
			}
		}
		//m_pGrid->onDrawRect(m_ActItem->GetIndex(),CLEAR_COLOR,46,46);
	}
}

// 得出鼠标在第几个位置
POINT CGridMgr::GetMouseGirdPos()
{
	POINT pos;
	GetCursorPos( &pos );
	ScreenToClient(m_hWnd, &pos);
	pos.x = pos.x/GIRD_SIZE;
	pos.y = pos.y/GIRD_SIZE;
	return pos;
}

void CGridMgr::RandGrid() // 随即格子
{
	// 增加三个点
	POINT p, p1;

	p.x = 11; 
	p.y = 1;
	for (int i=0; i<RAND_GRID; ++i)
	{	
		p1 = RandPos();
		m_pGrid->AddItem(m_pItem[i]);

		if(this->updatePos(p1, m_pItem[i]))
		{
			this->Count();
		}

		p.y++;
		int nIndex = p.x + GRID_WIDTH * p.y;
		m_pItem[i] = new CItem();
		E_COLOR color = RandColor();

		m_pItem[i]->InitItem(p, color, nIndex);
		m_pGrid->FillItemColor(m_pItem[i]->GetPos(), m_pItem[i]->GetColor());
	}


}
E_COLOR CGridMgr::RandColor()
{
	int ndata = rand() % E_COlOR_MAX;//(int)(E_COlOR_MAX*rand()/(RAND_MAX+1.0));
	E_COLOR	c =(E_COLOR) ndata;
	if (c == E_BLACK)
	{
		int ndata = rand() % E_COlOR_MAX;//(int)(E_COlOR_MAX*rand()/(RAND_MAX+1.0));
		c =(E_COLOR) ndata;
	}
	return c;
}

POINT CGridMgr::RandPos()
{
	POINT p; p.x = -1; p.y = -1;
	//srand((unsigned)time(NULL)); //随机打乱数据
	int nSize = m_pGrid->GetFreeGridSize();

	if (nSize>0)
	{	
		int nIndex = rand() % nSize;//(int)(nSize*rand()/(RAND_MAX+1.0));
		p = m_pGrid->GetFreeGirByIndex(nIndex);
	}
	else
	{	
		::MessageBox(m_hWnd,"game over!", "搞鬼排排球-Lai", MB_OK );	
		this->ClearAll();
	}

	return p;
}

void CGridMgr::RendGrid()
{
	m_pGrid->FillAllItemColor();
}

bool CGridMgr::updatePos(POINT& nPos, CItem* pItem )
{
	//m_grid[x][y] = nColor;
	//this->SetColorByPos(nPos, nColor); // 设置格子
	m_pGrid->SetItem(nPos, pItem);

	//AStar::getInstance().addBlockTile(getIndex(nPos.x, nPos.y));
	MapData::getInstance().setMapTile(nPos.x, nPos.y, 1);

	return this->Go(nPos, pItem);
}

bool CGridMgr::Go(POINT& nPos, CItem* pItem )
{
	if (Way(nPos,1,0) == true)
	{
		return true;
	}

	if (Way(nPos,0,1) == true)
	{
		return true;
	}

	if (Way(nPos,1,1) == true)
	{
		return true;
	}

	if (Way(nPos,1,-1) == true)
	{
		return true;
	}

	return false;
}
bool CGridMgr::Way(/*int x, int y*/POINT& pos, int l, int r)
{
	this->ClearGridIndex();
	
	POINT p;
	CItem* pitem1 = NULL;
	CItem* pitem2 = NULL;
	m_nWinNum  = 1;

	// 正方向	
	pitem2 = m_pGrid->GetItem(pos);
	if (pitem2 == NULL)
		return false;

	m_nWinGridIndex[0] = pitem2->GetIndex();
	for(int i=0; i<GRID_HEIGHT; ++i)
	{
		p.x = pos.x + l * (i+1);
		p.y = pos.y + r * (i+1);
	
		if (p.x >= 0 && p.y >= 0 && p.x < GRID_WIDTH && p.y < GRID_HEIGHT)
		{
			pitem1 = m_pGrid->GetItem(p);
			
			if(pitem1 && pitem2 && (pitem1->GetColor() == pitem2->GetColor() || pitem1->GetEColor()==E_BLACK|| pitem2->GetEColor()==E_BLACK))
			{
				m_nWinGridIndex[m_nWinNum++] = pitem1->GetIndex();
				
				//if (pitem2->GetEColor() == E_BLACK)
				//{
				//	pitem2 = pitem1;
				//}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	//-------------------------------------------
	//负方向
	pitem2 = m_pGrid->GetItem(pos);
	for(int i=0; i<GRID_WIDTH; ++i)
	{
		p.x = pos.x - l * (i+1);
		p.y = pos.y - r * (i+1);
		
		if (p.x >= 0 && p.y >= 0 && p.x < GRID_WIDTH && p.y < GRID_HEIGHT)
		{
			CItem* pitem1 = m_pGrid->GetItem(p);
		
			if(pitem1 && pitem2 && (pitem1->GetColor() == pitem2->GetColor()|| pitem1->GetEColor()==E_BLACK|| pitem2->GetEColor()==E_BLACK))
			{
				m_nWinGridIndex[m_nWinNum++] = pitem1->GetIndex();
				
				//if (pitem2->GetEColor() == E_BLACK)
				//{
				//	pitem2 = pitem1;
				//}

			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}


	if (m_nWinNum >= GRID_WIN)
	{
		return true;
	}
	return false;
}

void CGridMgr::ClearByIndex(int nIndex)
{
	POINT p;
	p.x = nIndex % GRID_WIDTH;
	p.y = nIndex / GRID_WIDTH;

	//AStar::getInstance().delBlockNode(p);

	//AStar::getInstance().delBlockTile(getIndex(p.x, p.y));
	MapData::getInstance().setMapTile(p.x, p.y, 0);
	m_pGrid->DelItem(nIndex);
	m_pGrid->SetGridIsUsed(nIndex,-1);
	m_pGrid->AddFreeGrid(nIndex);

	this->ColorTransform(p);
}

void CGridMgr::ClearAll()
{
	for (int i=0; i<GRID_HEIGHT*GRID_WIDTH; ++i)
	{
		this->ClearByIndex(i);
	}
	m_nNum = 0;
}

void CGridMgr::ColorTransform(POINT p)
{
	//m_pGrid->FillItemColor(p,RGB(150,150,150));	
	//m_pGrid->FillItemColor(p,RGB(200,200,200));	
	//m_pGrid->FillItemColor(p,RGB(230,230,230));	
	m_pGrid->FillItemColor(p,CLEAR_COLOR);	
}
void CGridMgr::ClearGridIndex()
{
	for (int i=0; i<GRID_WIN; ++i)
	{
		m_nWinGridIndex[i] = -1;
	}
}

void CGridMgr::Count()
{
	//if (m_nWinNum >= GRID_WIN)
	//{
	//	m_nNum += (m_nWinNum - 4)*10;

	//	for (int i=0; i<m_nWinNum; ++i)
	//	{
	//		this->ClearByIndex(m_nWinGridIndex[i]);
	//	}
	//}

	//TCHAR str[ 64 ];
	//sprintf(str,"分数：%d", m_nNum);
	//m_pGrid->DrawTextNum(str);
	//m_pFile->Write(m_nNum);
	//m_nWinNum = 0;
	////::MessageBox(m_hWnd,"you are win!", "搞鬼排排球", MB_OK );
}

void CGridMgr::addGrid(int x, int y)
{
	if (m_pGrid->GetItem(getIndex(x, y)) == nullptr)
	{
		E_COLOR color = RandColor();
		CItem* pItem = new CItem;

		POINT pos;
		pos.x = x;
		pos.y = y;
		int nIndex = getIndex(pos.x, pos.y);
		pItem->InitItem(pos, color, nIndex);
		m_pGrid->AddItem(pItem);
		m_pGrid->SetItem(pos, pItem);
		//AStar::getInstance().addBlockTile(nIndex);
		MapData::getInstance().setMapTile(pos.x, pos.y, 1);
	}
	else
	{
		int nIndex = getIndex(x, y);

		m_pGrid->DelItem(nIndex);
		m_pGrid->SetGridIsUsed(nIndex, -1);
		m_pGrid->AddFreeGrid(nIndex);

		//AStar::getInstance().delBlockTile(nIndex);
		MapData::getInstance().setMapTile(x, y, 0);
	}
}

void CGridMgr::OnRButtonDown()
{
	POINT p = GetMouseGirdPos();
	int nIndex = m_pGrid->GetIndexByPos(p);
	if (nIndex < 0)
		return;

	addGrid(p.x, p.y);

	updateUi();
}

void CGridMgr::updateUi()
{
	m_pGrid->OnDrawGrid();
	//m_pGrid->clearAllColor();
	m_pGrid->updateAllItem();
}

int CGridMgr::getIndex(int x, int y)
{
	int i = y * GRID_WIDTH + x;
	return i;
}

void CGridMgr::refreshRole(POINT& rolePos, glm::ivec2& size, DWORD dcolor)
{
	POINT tempPos = rolePos;
	for (int i = 0; i < size.x; ++i)
	{
		for (int y = 0; y < size.y; ++y)
		{
			tempPos.x = rolePos.x + i;
			tempPos.y = rolePos.y - y;
			m_pGrid->FillItemColor(tempPos, dcolor);	//走动
		}
	}
}

//void CGridMgr::RendActItem()
//{
//	if (m_ActItem)
//	{	
//		m_pGrid->FillItemColor(m_ActItem->GetPos(),m_ActItem->GetColor()/2);
//		Sleep(10);
//		m_pGrid->FillItemColor(m_ActItem->GetPos(),m_ActItem->GetColor());	
//		Sleep(10);
//	}
//
//}
//---------------------------------------------------
//void CGridMgr::OnDrawGrid()
//{
//	for(int i=0; i<=GRID_COL; ++i)
//	{
//		MoveToEx(m_dc,i*GIRD_SIZE,0,NULL);
//		LineTo(m_dc,i*GIRD_SIZE,550);
//
//		MoveToEx(m_dc,0,i*GIRD_SIZE,NULL);
//		LineTo(m_dc,500,i*GIRD_SIZE);
//	}
//}
//
//void CGridMgr::DrawTextNum(TCHAR* pch)
//{
//	TextOut( m_dc, 0, 35, pch, strlen(pch) );
//}
