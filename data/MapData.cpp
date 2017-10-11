/*!
 * \file MapData.cpp
 * \date 4-14-2016 14:25:18
 * 
 * 
 * \author laizhisheng
 */

#include "MapData.h"



MapData::MapData()
{

}

MapData::~MapData()
{

}

MapData& MapData::getInstance()
{
	static MapData s_Instance;
	return s_Instance;
}

bool MapData::resizeMap(int nWidth, int nHeight)
{
	m_mapDim = glm::ivec2(nWidth, nHeight);

	m_vMapData.clear();
	m_vMapData.resize(nWidth*nHeight);

	return true;
}

void MapData::setMapTile(int x, int y, int nType)
{
	if (x < 0 || x >= m_mapDim.x) return;
	if (y < 0 || y >= m_mapDim.y) return;

	int tileIndex = y*m_mapDim.x + x;
	m_vMapData[tileIndex] = nType;
}

int MapData::getMapTile(int x, int y)
{
	if (x < 0 || x >= m_mapDim.x) return TILE_INVALID;
	if (y < 0 || y >= m_mapDim.y) return TILE_INVALID;

	int tileIndex = y * m_mapDim.x + x;
	return m_vMapData[tileIndex];
}

int MapData::getMapIndex(int x, int y)
{
	if (!isValidTile(x, y)) return -1;

	int nIndex = y * m_mapDim.x + x;
	return nIndex;
}

bool MapData::isValidTile(int x, int y)
{
	if (x < 0 || x >= m_mapDim.x) return false;
	if (y < 0 || y >= m_mapDim.y) return false;

	return true;
}

bool MapData::isBlockingTile(const glm::ivec2& coord)
{
	int nType = getMapTile(coord.x, coord.y);
	if (nType == MapData::TILE_EMPTY)
	{
		return false;
	}
	else if (nType == MapData::TILE_INVALID)
	{
		return true; // 超出也属于不能走
	}

	return true;
}
