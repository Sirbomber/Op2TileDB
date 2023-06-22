#include "TileInfo.h"

void TileInfo::RecordNeighbor(int _neighborTileId, TileDir _neighborDir)
{
	if (!_neighborTileId == PLACEHOLDER_TILE)	// Ignore placeholder blue tile
	{
		neighborCounts[(int)_neighborDir].AddTileToList(_neighborTileId);
	}
}

void TileInfo::SetNeighborCount(int _neighborTileId, int _neighborCount, TileDir _neighborDir)
{
	if (!_neighborTileId == PLACEHOLDER_TILE)	// Ignore placeholder blue tile
	{
		neighborCounts[(int)_neighborDir].AddTileToList(_neighborTileId, _neighborCount);
	}
}

int TileInfo::GetRandomNeighborTile(TileDir _neighborDir)
{
	return neighborCounts[(int)_neighborDir].GetWeightedRandomTile();
}