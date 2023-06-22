#pragma once

/*
	This class is used to track which tiles are commonly adjacent to each other, and how frequently that happens.
	
*/
#include <map>
#include "TileDb_Types.h"

class NeighborList
{
public:
	NeighborList() {};
	~NeighborList() {};

	bool Empty() { return nList.empty(); }
	std::map<int, int>* GetNeighborList() { return &nList; }
	void AddTileToList(int _tileId) { if (_tileId != PLACEHOLDER_TILE) { nList[_tileId]++; } }
	void AddTileToList(int _tileId, int _count) { if (_tileId != PLACEHOLDER_TILE) { nList[_tileId] = _count; } }

	int GetRandomTile();						// Return a tile chosen at random from the list of all known neighbors
	int GetWeightedRandomTile();				// Return a tile based on how frequently that tile is used as a neighbor
	int GetMostCommonTile();					// Return the most frequently encountered neighbor tile

	void GetCommonList(NeighborList* _listsToMerge[], int _numToMerge, bool _strict = true);
	void GetMergedList(NeighborList* _listsToMerge[], int _numToMerge);

private:
	int GetRandom(int _r);

	std::map<int, int> nList;
};