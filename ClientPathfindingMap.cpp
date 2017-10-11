/*!
 * \file ClientPathfindingMap.cpp
 *
 * \author laizhisheng
 * \date 2017/08/22 11:10
 *
 */

#include "ClientPathfindingMap.h"
#include "data/MapData.h"

int ClientPathfindingMap::getMapHeight()
{
	return m_pMapData->getMapDim().x;
}

int ClientPathfindingMap::getMapWidth()
{
	return m_pMapData->getMapDim().y;
}

bool ClientPathfindingMap::isBlockingTile(int x, int y)
{
	return m_pMapData->isBlockingTile(glm::ivec2(x, y));
}

void ClientPathfindingMap::setMapData(MapData* pMapData)
{
	m_pMapData = pMapData;
}
