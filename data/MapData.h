/*!
 * \file MapData.h
 * \date 4-14-2016 14:25:12
 * 
 * 
 * \author laizhisheng
 */

#pragma once

#include <vector>
#include "glm/glm.hpp"


class MapData
{
public:
	MapData();
	~MapData();

	enum CONST_DEFINE
	{
		TILE_INVALID = -1,
		TILE_EMPTY = 0,
		TILE_BLACK = 1,
		TILE_RED = 2,
		TILE_GREEN = 3,
		TILE_BLUE = 4,
		TILE_YELLOW = 5,
		TILE_PURPLE = 6,
		TILE_CYAN = 7,
		TILE_GRAY = 8,
		TILE_ORANGE = 9,
	};

public:
	typedef std::vector<glm::uint8> TV_UINT8;

public:
	static MapData& getInstance();

	bool resizeMap(int nWidth, int nHeight);
	void setMapTile(int x, int y, int nType);
	int getMapIndex(int x, int y);
	int getMapTile(int x, int y);
	bool isValidTile(int x, int y);
	//bool isBlockingTile(int x, int y);
	bool isBlockingTile(const glm::ivec2& coord);
	const glm::ivec2& getMapDim() const { return m_mapDim; }

private:
	TV_UINT8 m_vMapData;
	glm::ivec2 m_mapDim;
};