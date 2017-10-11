/*!
 * \file Pathfinding.cpp
 *
 * \author laizhisheng
 * \date 2017/08/23 14:03
 *
 */

#include "Pathfinding.h"
#include "PathfindingMap.h"
#include <algorithm>

#define SAFE_DELETE(x) if (x) {delete x; x = nullptr;}

IAStar::IAStar()
{

}

IAStar::~IAStar()
{
	freeNodelist();

	for (auto itor = m_freeNodes.begin(); itor != m_freeNodes.end(); ++itor)
	{
		Node* pNode = *itor;
		SAFE_DELETE(pNode);
	}

	m_freeNodes.clear();
}

void IAStar::setPathfindingMap(PathfindingMap* pMap, int maxNodeCache /*= 1024*/)
{
	m_pPathfindingMap = pMap;
	m_maxNodeCache = maxNodeCache;
	m_mapWidth = m_pPathfindingMap->getMapWidth();
	m_mapHeight = m_pPathfindingMap->getMapHeight();
}

int IAStar::getGenerateNumber()
{
	return m_generateNumber;
}

IAStar::Node* IAStar::findPath(int sx, int sy, int dx, int dy, bool bNear)
{
	m_generateNumber = 0;
	freeNodelist();

	Node* pRootNode = creatNode();
	pRootNode->m_startToCurrentCost = 0;
	pRootNode->m_currentToEndCost = getCurrentToEndCost(sx, sy, dx, dy);;
	pRootNode->m_totalCost = pRootNode->m_startToCurrentCost + pRootNode->m_currentToEndCost;
	pRootNode->m_index = getTileIndex(sx, sy);
	pRootNode->m_x = sx;
	pRootNode->m_y = sy;

	insertNode(pRootNode);

	m_pNearNode = pRootNode;

	Node* pBestNode = nullptr;
	while (true)
	{
		pBestNode = getBestNode();
		if (pBestNode)
		{
			int nIdexDest = getTileIndex(dx, dy);

			if (pBestNode->m_index == nIdexDest)
			{
				break;
			}

			m_generateNumber += generateChildNodes(pBestNode, dx, dy);
		}
		else
		{
			if (bNear)
			{
				pBestNode = m_pNearNode;
			}
			break;
		}
	}

	return pBestNode;
}

IAStar::Node* IAStar::getBestNode()
{
	while (!m_queueOpenNodes.empty())
	{
		Node* pBestNode = m_queueOpenNodes.top();
		m_queueOpenNodes.pop();
		if (pBestNode->m_bOpen)
		{
			pBestNode->m_bOpen = false;
			return pBestNode;
		}
		else
		{
			m_freeNodes.emplace_back(pBestNode);
		}
	}

	return nullptr;
}

void IAStar::generateTreeNode(Node* pBestNode, int x, int y, float startToCurrentCost, float currentToEndCost)
{
	Node* pChildNode = nullptr;

	int childIndex = getTileIndex(x, y);

	if ((pChildNode = findExistNode(childIndex)) != nullptr)
	{
		if (startToCurrentCost < pChildNode->m_startToCurrentCost)
		{
			if (pChildNode->m_bOpen)
			{
				pChildNode->m_bOpen = false;

				pChildNode = creatNode();
				pChildNode->m_pParent = pBestNode;
				pChildNode->m_startToCurrentCost = startToCurrentCost;
				pChildNode->m_currentToEndCost = currentToEndCost;
				pChildNode->m_totalCost = startToCurrentCost + pChildNode->m_currentToEndCost;
				pChildNode->m_x = x;
				pChildNode->m_y = y;
				pChildNode->m_index = childIndex;
				insertNode(pChildNode);
			}
			else
			{
				pChildNode->m_pParent = pBestNode;
				pChildNode->m_startToCurrentCost = startToCurrentCost;
				pChildNode->m_currentToEndCost = currentToEndCost;
				pChildNode->m_totalCost = startToCurrentCost + pChildNode->m_currentToEndCost;
			}
		}
	}
	else
	{
		pChildNode = creatNode();

		pChildNode->m_pParent = pBestNode;
		pChildNode->m_startToCurrentCost = startToCurrentCost;
		pChildNode->m_currentToEndCost = currentToEndCost;
		pChildNode->m_totalCost = startToCurrentCost + pChildNode->m_currentToEndCost;
		pChildNode->m_x = x;
		pChildNode->m_y = y;
		pChildNode->m_index = childIndex;
		insertNode(pChildNode);
	}

	if (pChildNode->m_currentToEndCost <= m_pNearNode->m_currentToEndCost)
	{
		m_pNearNode = pChildNode;
	}
}

IAStar::Node* IAStar::creatNode()
{
	Node* pNode = NULL;

	if (!m_freeNodes.empty())
	{
		pNode = m_freeNodes.back();
		m_freeNodes.pop_back();
		pNode->reset();
	}
	else
	{
		pNode = new Node();
	}

	return pNode;
}

void IAStar::freeNodelist()
{
	while (!m_queueOpenNodes.empty())
	{
		Node* pBestNode = m_queueOpenNodes.top();
		m_queueOpenNodes.pop();
		if (!pBestNode->m_bOpen)
		{
			m_freeNodes.emplace_back(pBestNode);
		}
	}

	int number = 0;
	for (auto itor = m_unorderMapAllNodes.begin(); itor != m_unorderMapAllNodes.end(); ++itor)
	{
		Node* pNode = itor->second;
		if (number < m_maxNodeCache)
		{
			m_freeNodes.emplace_back(pNode);
		}
		else
		{
			SAFE_DELETE(pNode);
		}
		++number;
	}
	m_unorderMapAllNodes.clear();

	m_pNearNode = nullptr;
}

IAStar::Node* IAStar::findExistNode(int nIndex)
{
	const auto& pair = m_unorderMapAllNodes.find(nIndex);
	if (pair != m_unorderMapAllNodes.end())
	{
		return pair->second;
	}
	return nullptr;
}

void IAStar::insertNode(Node* pChild)
{
	m_queueOpenNodes.emplace(pChild);
	m_unorderMapAllNodes[pChild->m_index] = pChild;
}

float IAStar::getCurrentToEndCost(int x, int y, int dx, int dy)
{
	float offset = (float)(m_roleWidth - 1) / 2.0f;
	float xDis = x + offset - dx;
	int yDis = y - dy;
	return (float)std::sqrt(xDis*xDis + yDis*yDis);
}

int IAStar::getTileIndex(int x, int y)
{
	if (!isValidTile(x, y)) return -1;

	int nIndex = y * m_mapWidth + x;
	return nIndex;
}

bool IAStar::isValidTile(int x, int y)
{
	if (x < 0 || x >= m_mapWidth) return false;
	if (y < 0 || y >= m_mapHeight) return false;

	return true;
}

bool IAStar::isIntersected(int x, int y)
{
	for (int w = 0; w < m_roleWidth; ++w)
	{
		for (int h = 0; h < m_roleHeight; ++h)
		{
			if (m_pPathfindingMap->isBlockingTile(x + w, y - h))
			{
				return true;
			}
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////

Pathfinding& Pathfinding::getInstance()
{
	static Pathfinding s_instance;
	return s_instance;
}

int Pathfinding::generateChildNodes(Node* pBestNode, int dx, int dy)
{
	int number = 0;
	int x, y;

	static YXList childNodeListR;
	static YXList childNodeListL;
	static PathPosList groundPosList;
	childNodeListR.clear();
	childNodeListL.clear();

	for (auto itor = m_highestPos.begin(); itor != m_highestPos.end(); ++itor)
	{
		y = pBestNode->m_y - itor->y;
		if (y < 0) y = 0;

		int minY = y - (m_roleHeight - 1);
		int maxY = pBestNode->m_y + m_maxDrop;
	
		// right
		{
			x = pBestNode->m_x + itor->x;
			if (x >= m_mapWidth) x = m_mapWidth - 1;

			groundPosList.clear();
			getStandPosList(groundPosList, x, minY, maxY, pBestNode->m_y - (m_roleHeight - 1));
			filterStandPosList(childNodeListR, groundPosList, pBestNode->m_x, pBestNode->m_y);
		}

		// left
		{
			x = pBestNode->m_x - itor->x;
			if (x < 0) x = 0;

			groundPosList.clear();
			getStandPosList(groundPosList, x, minY, maxY, pBestNode->m_y - (m_roleHeight - 1));
			filterStandPosList(childNodeListL, groundPosList, pBestNode->m_x, pBestNode->m_y);
		}
	}

	// right
	for (auto itor = childNodeListR.begin(); itor != childNodeListR.end(); ++itor)
	{
		number += calculateCost(pBestNode, itor->second, itor->first, dx, dy);
	}
	
	// left
	for (auto itor = childNodeListL.begin(); itor != childNodeListL.end(); ++itor)
	{
		number += calculateCost(pBestNode, itor->second, itor->first, dx, dy);
	}

	return number;
}

int Pathfinding::calculateCost(Node* pBestNode, int x, int y, int dx, int dy)
{
	float endDistCost = getCurrentToEndCost(x, y, dx, dy);
	if (endDistCost < m_maxEndDistCost)
	{
		int xCost = (pBestNode->m_x - x);
		int yCost = (pBestNode->m_y - y);
		float nodeCost = (xCost*xCost + yCost*yCost)*10.0f;
		float startDistCost = pBestNode->m_startToCurrentCost + nodeCost;
		generateTreeNode(pBestNode, x, y, startDistCost, endDistCost);
		return 1;
	}

	return 0;
}

void Pathfinding::initAdvanceHighestPos(int advance)
{
	m_highestPos.clear();

	ivec2 pos;

	for (int x = 1; x <= advance; ++x)
	{
		pos.x = x;
		pos.y = advance - x;
		m_highestPos.emplace_back(pos);
	}
}

void Pathfinding::getStandPosList(PathPosList& outPath, int x, int minY, int maxY, int currY /*= 0*/)
{
	if (x < 0) x = 0;
	if (x >= m_mapWidth) x = m_mapWidth - 1;
	if (minY < 0) minY = 0;
	if (maxY >= m_mapHeight) maxY = m_mapHeight - 1;

	ivec2 pos;
	int h = 0;
	for (int y = minY; y <= maxY; ++y)
	{
		if (!isStandPos(x, y))
		{
			++h;
		}
		else
		{
			if (h >= m_roleHeight)
			{
				pos.x = x;
				pos.y = y - 1;
				outPath.emplace_back(pos);

				if (y > currY)
				{
					break;
				}
			}

			h = 0;
		}
	}
}

void Pathfinding::filterStandPosList(YXList& outPath, const PathPosList& inPath, int sx, int sy)
{
	for (auto itor = inPath.begin(); itor != inPath.end(); ++itor)
	{
		if (outPath.find(itor->y) == outPath.end())
		{
			if (isPassable(sx, sy, itor->x, itor->y))
			{
				outPath.emplace(itor->y, itor->x);
			}
		}
	}
}

bool Pathfinding::isStandPos(int x, int y)
{
	for (int w = 0; w < m_roleWidth; ++w)
	{
		if (m_pPathfindingMap->isBlockingTile(x + w, y))
		{
			return true;
		}
	}

	return false;
}

bool Pathfinding::isPassable(int sx, int sy, int dx, int dy)
{
	// right down
	if (sx < dx && sy < dy)
	{
		for (int x = sx + 1; x <= dx; ++x)
		{
			if (isIntersected(x, sy))
			{
				return false;
			}
		}

		for (int y = sy + 1; y <= dy; ++y)
		{
			if (isIntersected(dx, y))
			{
				return false;
			}
		}
	}
	// right up
	else if (sx < dx && sy > dy)
	{
		for (int x = sx + 1; x <= dx; ++x)
		{
			if (isIntersected(x, dy))
			{
				return false;
			}
		}

		for (int y = dy; y < sy; ++y)
		{
			if (isIntersected(sx, y))
			{
				return false;
			}
		}
	}
	// left down
	else if (sx > dx && sy < dy)
	{
		for (int x = dx; x < sx; ++x)
		{
			if (isIntersected(x, sy))
			{
				return false;
			}
		}

		for (int y = sy + 1; y <= dy; ++y)
		{
			if (isIntersected(dx, y))
			{
				return false;
			}
		}
	}
	// left up
	else if (sx > dx && sy > dy)
	{
		for (int x = dx; x < sx; ++x)
		{
			if (isIntersected(x, dy))
			{
				return false;
			}
		}

		for (int y = dy; y < sy; ++y)
		{
			if (isIntersected(sx, y))
			{
				return false;
			}
		}
	}
	// right
	else if (sx < dx && sy == dy)
	{
		for (int x = sx + 1; x <= dx; ++x)
		{
			if (isIntersected(x, dy))
			{
				return false;
			}

			int offset = 0;
			if (dx - sx >= 2) offset = 1;
			if (offset > 0)
			{
				// top
				if (isIntersected(x, dy - offset))
				{
					return false;
				}
			}
		}
	}
	// left
	else if (sx > dx && sy == dy)
	{
		for (int x = dx; x < sx; ++x)
		{
			if (isIntersected(x, dy))
			{
				return false;
			}

			int offset = 0;
			if (sx - dx >= 2) offset = 1;
			if (offset > 0)
			{
				// top
				if (isIntersected(x, dy - offset))
				{
					return false;
				}
			}
		}
	}
	else
	{
		//sx = dx;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////

FlightPathfinding& FlightPathfinding::getInstance()
{
	static FlightPathfinding s_instance;
	return s_instance;
}

int FlightPathfinding::generateChildNodes(Node* pBestNode, int dx, int dy)
{
	static PathPosList offsetList{ { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }};
	static PathPosList childNodeList;

	childNodeList.clear();
	for (const auto& offset : offsetList)
	{
		ivec2 coord{ pBestNode->m_x + offset.x, pBestNode->m_y + offset.y };
		if (isPassable(pBestNode->m_x, pBestNode->m_y, coord.x, coord.y))
		{
			childNodeList.emplace_back(coord);
		}
	}

	int number{};
	for (auto itor = childNodeList.begin(); itor != childNodeList.end(); ++itor)
	{
		number += calculateCost(pBestNode, itor->x, itor->y, dx, dy);
	}
	return number;
}

int FlightPathfinding::calculateCost(Node* pBestNode, int x, int y, int dx, int dy)
{
	float currentToEndCost = getCurrentToEndCost(x, y, dx, dy);
	if (currentToEndCost < m_maxEndDistCost)
	{
		int xCost = (pBestNode->m_x - x);
		int yCost = (pBestNode->m_y - y);
		float bestToCurrCost = (float)std::sqrt(xCost*xCost + yCost*yCost);

		float currNodeCost = getNodeCost(x, y);
		float startToCurrentCost = pBestNode->m_startToCurrentCost + bestToCurrCost + currNodeCost;
		generateTreeNode(pBestNode, x, y, startToCurrentCost, currentToEndCost);
		return 1;
	}

	return 0;
}

float FlightPathfinding::getNodeCost(int x, int y)
{
	int bottom = y + 1;
	int top = y - 1;
	for (int i = 0; i < m_roleWidth; ++i)
	{
		if (m_pPathfindingMap->isBlockingTile(x + i, bottom))
		{
			return 1.f;
		}
		if (m_pPathfindingMap->isBlockingTile(x + i, bottom + 1))
		{
			return 1.5f;
		}
		if (m_pPathfindingMap->isBlockingTile(x + i, top))
		{
			return 1.f;
		}
	}

	int right = x + 1;
	int left = x - 1;
	for (int i = 0; i < m_roleHeight; ++i)
	{
		if (m_pPathfindingMap->isBlockingTile(right, y - i))
		{
			return 1.f;
		}
		if (m_pPathfindingMap->isBlockingTile(left, y - i))
		{
			return 1.f;
		}
	}

	return 0.f;
}

bool FlightPathfinding::isPassable(int sx, int sy, int dx, int dy)
{
	if (isIntersected(dx, dy)) return false;

	if ((dx != sx) && (sy != dy))
	{
		if (isIntersected(sx, dy)) return false;
		if (isIntersected(dx, sy)) return false;
	}

	return true;
}
