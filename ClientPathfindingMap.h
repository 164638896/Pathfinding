/*!
 * \file ClientPathfindingMap.h
 *
 * \author laizhisheng
 * \date 2017/08/22 11:09
 *
 */

#pragma once
#include "pathfinding/PathfindingMap.h"
class MapData;

class ClientPathfindingMap : public PathfindingMap
{
public:
	ClientPathfindingMap(){};
	virtual ~ClientPathfindingMap(){};

public:
	virtual int getMapHeight() override;
	virtual int getMapWidth() override;
	virtual bool isBlockingTile(int x, int y) override;

public:
	void setMapData(MapData* pMapData);

private:
	MapData* m_pMapData{};
};
