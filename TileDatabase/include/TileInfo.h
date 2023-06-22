#pragma once

/*
	This class serves as a container for each tile ID's adjacency lists in all available directions.
*/

#include "TileDb_Types.h"
#include "NeighborList.h"

class TileInfo
{
public:
	TileInfo() { tileType = 0; }
	~TileInfo() {};

	void RecordNeighbor(int _neighborTileId, TileDir _neighborDir);
	void SetNeighborCount(int _neighborTileId, int _neighborCount, TileDir _neighborDir);
	NeighborList* GetNeighborList(TileDir _neighborDir) { return &neighborCounts[(int)_neighborDir]; }
	int GetRandomNeighborTile(TileDir _neighborDir);

private:
	//void UpdateNeighborCount(std::map<int, int> *_toUpdate, int _tile);
	NeighborList neighborCounts[(int)TileDir::Count];
	int tileType;	// cell type - might need to keep a running tally of these too and go with the most commonly used cell type (make sure to exclude the bottom row of a map when tallying this)
};
