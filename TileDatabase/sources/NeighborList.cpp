#include "NeighborList.h"

#ifdef _OP2_MAP_BUILD
#include <Tethys\Game\Random.h>
#include <Tethys\API\Game.h>
#else
#include <random>
#endif

int NeighborList::GetRandomTile()
{
	// If this neighbor list is empty, just return 0 (the tile ID of the default blue placeholder tile)
	if (nList.empty())
	{
		return PLACEHOLDER_TILE;
	}
	else
	{
		int randNum = GetRandom(nList.size());
		// This feels like a hack
		int i = 0;
		for (std::map<int, int>::iterator it = nList.begin(); it != nList.end(); it++)
		{
			if (i == randNum)
			{
				return it->first;
			}
		}
		return PLACEHOLDER_TILE;
	}
}

int NeighborList::GetWeightedRandomTile()
{
	// If this neighbor list is empty, just return 0 (the tile ID of the default blue placeholder tile)
	if (nList.empty())
	{
		return PLACEHOLDER_TILE;
	}
	else
	{
		// Random number prep
		int randMax = 0,
			randTally = 0,
			randNum = 0;

		// Random tile is weighted by how commonly a tile gets used as a neighbor.  First, add up the total number of times each tile was used as a neighbor.  We'll generate a random number 0 through that sum.
		for (std::map<int, int>::iterator it = nList.begin(); it != nList.end(); it++)
		{
			randMax += it->second;
		}
		randNum = GetRandom(randMax);

		// Now iterate over the neighbor tile list again.  If our random number is less than (times tile used as a neighbor + running tally) than use this tile.  Else keep going.
		for (std::map<int, int>::iterator it = nList.begin(); it != nList.end(); it++)
		{
			if (randNum < it->second + randTally)
			{
				return it->first;
			}
			else
			{
				randTally += it->second;
			}
		}
	}

	// We shouldn't reach here but if we do return the placeholder tile as a precaution.
	return PLACEHOLDER_TILE;
}

int NeighborList::GetMostCommonTile()
{
	// If this neighbor list is empty, just return 0 (the tile ID of the default blue placeholder tile)
	if (nList.empty())
	{
		return PLACEHOLDER_TILE;
	}
	else
	{
		std::map<int, int>::iterator it = nList.begin();
		int mostCommon = it->first;
		it++;
		for (it; it != nList.end(); it++)
		{
			if (it->second > nList[mostCommon])
			{
				mostCommon = it->second;
			}
		}
		return mostCommon;
	}
}

void NeighborList::GetCommonList(NeighborList* _listsToMerge[], int _numToMerge, bool _strict)
{
	// Make sure we're working with an empty list.
	nList.clear();

	if (_numToMerge == 1)
	{
		nList = _listsToMerge[0]->nList;
		return;
	}

	for (int i = 0; i < _numToMerge - 1; i++)
	{
		for (std::map<int, int>::iterator it1 = _listsToMerge[i]->nList.begin(); it1 != _listsToMerge[i]->nList.end(); it1++)
		{
			// Skip any common tiles we've already identified.
			if (nList.count(it1->first))
			{
				continue;
			}

			// If strict mode is set, we only want to record a tile as common if it exists in the list for ALL adjacent tiles.  This bool gets set if a tile isn't found in even one of the passed-in lists.
			bool isCommon = true;
			int commonCount = it1->second;
			for (int j = i + 1; j < _numToMerge; j++)
			{
				int tempCount = _listsToMerge[j]->nList.count(it1->first);
				if (tempCount == 0 && _strict)
				{
					isCommon = false;
					break;
				}
				else
				{
					commonCount += tempCount;
				}
			}
			if (isCommon)
			{
				nList[it1->first] = commonCount;
			}
		}
	}
}

void NeighborList::GetMergedList(NeighborList* _listsToMerge[], int _numToMerge)
{
	// Make sure we're working with an empty list.
	nList.clear();

	if (_numToMerge == 1)
	{
		nList = _listsToMerge[0]->nList;
		return;
	}

	for (int i = 0; i < _numToMerge; i++)
	{
		for (std::map<int, int>::iterator it1 = _listsToMerge[i]->nList.begin(); it1 != _listsToMerge[i]->nList.end(); it1++)
		{
			nList[it1->first] += it1->second;
		}
	}
}

int NeighborList::GetRandom(int _r)
{
#ifdef _OP2_MAP_BUILD
	return Tethys::TethysAPI::Game::GetRand(_r);
#else
	return rand() % _r;
#endif
}