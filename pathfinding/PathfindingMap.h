/*!
 * \file PathfindingMap.h
 *
 * \author laizhisheng
 * \date 2017/08/22 11:01
 *
 */

#pragma once

class PathfindingMap
{
public:
	PathfindingMap(){};
	virtual ~PathfindingMap(){};

public:
	virtual int getMapHeight() = 0;
	virtual int getMapWidth() = 0;
	virtual bool isBlockingTile(int x, int y) = 0;
};
