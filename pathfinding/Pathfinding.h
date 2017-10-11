/*!
 * \file Pathfinding.h
 *
 * \author laizhisheng
 * \date 2017/08/23 14:02
 *
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <map>

class PathfindingMap;

class IAStar
{
public:
	IAStar();
	virtual ~IAStar();

public:
	class Node
	{
	public:
		void reset()
		{
			m_x = 0;
			m_y = 0;
			m_index = -1;

			m_totalCost = 0.f;
			m_startToCurrentCost = 0.f;
			m_currentToEndCost = 0.f;

			m_pParent = nullptr;
			m_bOpen = true;
		}

	public:
		int m_x{};
		int m_y{};
		int m_index{};

		float m_totalCost{};			// total cost (m_startToCurrentCost + m_currentToEndCost)
		float m_startToCurrentCost{};	// current point to start point cost
		float m_currentToEndCost{};		// current point to destiation point cost

		Node* m_pParent{}; 
		bool m_bOpen{ true };
	};

	class ivec2
	{
	public:
		ivec2(){};
		ivec2(int _x, int _y) : x(_x), y(_y){};

		int x{};
		int y{};
	};

	typedef std::vector<ivec2> PathPosList;
	typedef std::vector<Node*> NodeList;
	typedef std::unordered_map<int/*m_index*/, Node*> NodeUnorderedmap;

	struct greaterNode
	{	// functor for operator>
		bool operator()(const Node* _Left, const Node* _Right) const
		{	// apply operator> to operands
			return (_Left->m_totalCost > _Right->m_totalCost);
		}
	};
	typedef std::priority_queue<Node*, std::vector<Node*>, greaterNode> NodePrioityQueue;

public:
	void setPathfindingMap(PathfindingMap* pMap, int maxNodeCache = 1024);
	int getGenerateNumber();

protected:
	int getTileIndex(int x, int y);
	bool isValidTile(int x, int y);
	bool isIntersected(int x, int y);

protected:
	Node* findPath(int sx, int sy, int dx, int dy, bool bNear);
	Node* getBestNode();

	virtual int generateChildNodes(Node* pBestNode, int dx, int dy) = 0;
	void generateTreeNode(Node* pBestNode, int x, int y, float startToCurrentCost, float currentToEndCost);

	void freeNodelist();
	Node* creatNode();

	Node* findExistNode(int nIndex);
	void insertNode(Node* pChild);

	float getCurrentToEndCost(int x, int y, int dx, int dy);

protected:
	NodeUnorderedmap m_unorderMapAllNodes;
	NodePrioityQueue m_queueOpenNodes;
	NodeList m_freeNodes;
	Node* m_pNearNode{};
	PathfindingMap* m_pPathfindingMap{};

	int m_mapHeight{};
	int m_mapWidth{};

	int m_roleWidth{1};
	int m_roleHeight{1};

	float m_maxEndDistCost{};
	int m_generateNumber{};
	int m_maxNodeCache{ 1024 };
};

class Pathfinding : public IAStar
{
private:
	Pathfinding(){};
	virtual ~Pathfinding(){};

	typedef std::map<int /*y*/, int /*x*/> YXList;

public:
	static Pathfinding& getInstance();

	template<typename V> bool getFindPath(std::vector<V>& pathList,
		const V& from,
		const V& to,
		const V& roleSize,
		int advance = 2,
		float endCostMax = FLT_MAX,
		int drop = INT_MAX);

protected:
	virtual int generateChildNodes(Node* pBestNode, int dx, int dy) override;

private:
	int calculateCost(Node* pBestNode, int x, int y, int dx, int dy);
	void initAdvanceHighestPos(int advance);
	void getStandPosList(PathPosList& outPath, int x, int minY, int maxY, int currY = 0);
	void filterStandPosList(YXList& outPath, const PathPosList& inPath, int sx, int sy);
	bool isStandPos(int x, int y);
	bool isPassable(int sx, int sy, int dx, int dy);

private:
	PathPosList m_highestPos;
	int m_maxDrop{};
};

template<typename V>
bool Pathfinding::getFindPath(std::vector<V>& pathList, const V& from, const V& to, const V& roleSize, int advance /*= 2*/, float endCostMax /*= FLT_MAX*/, int drop /*= INT_MAX*/)
{
	m_roleWidth = roleSize.x;
	m_roleHeight = roleSize.y;

	initAdvanceHighestPos(advance);

	m_maxEndDistCost = endCostMax;

	m_maxDrop = drop;
	if (m_maxDrop > m_mapHeight)
	{
		m_maxDrop = m_mapHeight;
	}

	bool bFindPath = false;

	Node* pPathNode = findPath(from.x, from.y, to.x, to.y, true);
	if (pPathNode && pPathNode->m_pParent != nullptr)
	{
		while (pPathNode->m_pParent != nullptr)
		{
			pathList.emplace_back(V{ pPathNode->m_x, pPathNode->m_y });
			pPathNode = pPathNode->m_pParent;
		}

		bFindPath = true;
	}
	else
	{
		bFindPath = false;
	}

	freeNodelist();

	return bFindPath;
}

class FlightPathfinding : public IAStar
{
private:
	FlightPathfinding(){};
	virtual ~FlightPathfinding(){};

public:
	static FlightPathfinding& getInstance();

	template<typename V> bool getFindPath(std::vector<V>& pathList,
		const V& from,
		const V& to,
		const V& roleSize,
		float endCostMax = FLT_MAX);

protected:
	virtual int generateChildNodes(Node* pBestNode, int dx, int dy) override;

private:
	int calculateCost(Node* pBestNode, int x, int y, int dx, int dy);
	float getNodeCost(int x, int y);
	bool isPassable(int sx, int sy, int dx, int dy);
};

template<typename V>
bool FlightPathfinding::getFindPath(std::vector<V>& pathList, const V& from, const V& to, const V& roleSize, float endCostMax /*= FLT_MAX*/)
{
	m_roleWidth = roleSize.x;
	m_roleHeight = roleSize.y;

	m_maxEndDistCost = endCostMax;

	bool bFindPath = false;

	Node* pPathNode = findPath(from.x, from.y, to.x, to.y, true);
	if (pPathNode && pPathNode->m_pParent != nullptr)
	{
		while (pPathNode->m_pParent != nullptr)
		{
			pathList.emplace_back(V{ pPathNode->m_x, pPathNode->m_y });
			pPathNode = pPathNode->m_pParent;
		}

		bFindPath = true;
	}
	else
	{
		bFindPath = false;
	}

	freeNodelist();

	return bFindPath;
}
