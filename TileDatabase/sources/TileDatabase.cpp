#include "TileDatabase.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::ifstream input;
std::ofstream output;

void TileDatabase::UpdateTileInDatabase(int _srcTile, int _neighborTile, TileDir _dirOfNextTile)
{
	// Ignore blue placeholder tile
	if (_srcTile == PLACEHOLDER_TILE || _neighborTile == PLACEHOLDER_TILE)
	{
		return;
	}

	allTiles[_srcTile].RecordNeighbor(_neighborTile, _dirOfNextTile);
	return;
}

void TileDatabase::SetTileNeighborCount(int _srcTile, int _neighborTile, int _neighborCount, TileDir _dirOfNextTile)
{
	// Ignore blue placeholder tile
	if (_srcTile == PLACEHOLDER_TILE || _neighborTile == PLACEHOLDER_TILE)
	{
		return;
	}

	allTiles[_srcTile].SetNeighborCount(_neighborTile, _neighborCount, _dirOfNextTile);
}

int TileDatabase::PickNextTile(int _srcTile, TileDir _dirOfNextTile)
{
	// Make sure the source tile exists in the database
	if (!allTiles.count(_srcTile))
	{
		return 0;
	}
	else
	{
		return allTiles[_srcTile].GetRandomNeighborTile(_dirOfNextTile);
	}
}

int TileDatabase::PickTileBasedOnAllNeighbors(TileAndDir _tiles[], int _num)
{
	// Try to build a neighborCounts list using only tiles that appear on all 4 neighboring tiles' neighborCounts lists.
	// If there are no overlaps, just glue all 4 lists together and cross your fingers.
	NeighborList commonNeighbors;
	NeighborList* toPass[4];
	int nonEmptyLists = 0;
	for (int i = 0; i < _num; i++)
	{
		if (_tiles[i].t != 0)
		{
			toPass[nonEmptyLists] = allTiles[_tiles[i].t].GetNeighborList(_tiles[i].d);
			nonEmptyLists++;
		}
	}

	// Build the common neighbors list
	if (nonEmptyLists > 0)
	{
		commonNeighbors.GetCommonList(toPass, nonEmptyLists);
	}

	// If that fails, build a "non-strict" common neighbors list.
	if (commonNeighbors.Empty())
	{
		commonNeighbors.GetCommonList(toPass, nonEmptyLists, false);
	}

	// If THAT fails, just glue all the neighbor lists together.
	if (commonNeighbors.Empty())
	{
		commonNeighbors.GetMergedList(toPass, nonEmptyLists);
	}

	// If the common neighbors list isn't empty, pick a random tile from it
	if (!commonNeighbors.Empty())
	{
		return commonNeighbors.GetWeightedRandomTile();
	}

	// No matches, return the placeholder tile.
	return 0;
}

/*
	for (int i = 0; i < _num-1; i++)
	{
		if (_tiles[i].t == 0)
		{
			continue;
		}
		for (nTileList::iterator it1 = allTiles[_tiles[i].t].GetNeighborCountList(_tiles[i].d)->begin(); it1 != allTiles[_tiles[i].t].GetNeighborCountList(_tiles[i].d)->end(); it1++)
		{
			gluedList[it1->first] = it1->second;
			for (int j = i + 1; j < _num; j++)
			{
				if (_tiles[j].t == 0)
				{
					continue;
				}
				for (nTileList::iterator it2 = allTiles[_tiles[j].t].GetNeighborCountList(_tiles[j].d)->begin(); it2 != allTiles[_tiles[j].t].GetNeighborCountList(_tiles[j].d)->end(); it2++)
				{
					if (it1->first == it2->first)
					{
						commonNeighbors[it1->first] += (it1->second + it2->second);
						break;
					}
				}
			}
		}
	}

	// If the common neighbors list isn't empty, pick a tile at random from it.
	if (!commonNeighbors.empty())
	{
		//
	}

	// Else just pick any possible neighbor tile at random (assuming that list isn't also empty).
	else if (!gluedList.empty())
	{
		//
	}

	// No matches, return the placeholder tile.
	return 0;
}
*/

/*
void TileDatabase::ProcessTile(int _x, int _y, int _mapWidth, int _mapHeight, bool _isWorldMap)
{
	// Iterate through each tile on the map.
	for (int y = 0; y < _mapHeight; y++)
	{
		for (int x = 0; x < _mapWidth; x++)
		{
			// Get
			// Special case code for world maps
			if (_isWorldMap && (x == 0 || x == _mapWidth - 1))
			{
				if (x == 0)
				{
					// Check right edge of the world
				}
				else
				{
					// Check left edge of the world
				}
			}
		}
	}
}
*/

/* DB file format (planned)
*  <tile=(tileId)>
*   <dir=(north/east/south/west)>
*    <neighbor=(neighborTileId)>
*     count=(number of times this tile appeared as a neighbor)
*    </neighbor>
*   </dir>
*  </tile>
*/

/*
Test Format:
tile###
north: tileID.count ...
east: tileID.count ...
south: tileID.count ...
west: tileID.count ...
end
...
*/
bool TileDatabase::ImportDatabase(std::string _iFilename)
{
	input.open(_iFilename);

	if (!input.is_open())
	{
		//TethysGame::AddMessage(-1, -1, "Failed to open database file", -1, sndDirt);
		return false;
	}

	else
	{
		imported = true;

		std::string nextLine;
		int curTile = -1,
			curNeighbor = -1,
			curCount = -1;
		TileDir curDir = TileDir::Count;

		while (std::getline(input, nextLine))
		{
			int t;
			std::stringstream tilesAndCounts;

			if ((t = nextLine.find("tile")) != std::string::npos)
			{
				curTile = std::stoi(nextLine.substr(t + 4, std::string::npos));
				curNeighbor = -1;
				curCount = -1;
				curDir = TileDir::Count;
			}
			else if ((t = nextLine.find("north")) != std::string::npos)
			{
				curDir = TileDir::North;
				curNeighbor = -1;
				curCount = -1;
				tilesAndCounts.str(nextLine.substr(t + 7, std::string::npos));
				std::string next;
				while (!tilesAndCounts.eof())
				{
					tilesAndCounts >> next;
					if ((t = next.find(".")) != std::string::npos && curTile != -1 && curDir != TileDir::Count)
					{
						curNeighbor = std::stoi(next.substr(0, t));
						curCount = std::stoi(next.substr(t + 1, std::string::npos));
						SetTileNeighborCount(curTile, curNeighbor, curCount, curDir);
					}
				}
				
			}
			else if ((t = nextLine.find("east")) != std::string::npos)
			{
				curDir = TileDir::East;
				curNeighbor = -1;
				curCount = -1;
				tilesAndCounts.str(nextLine.substr(t + 6, std::string::npos));
				std::string next;
				while (!tilesAndCounts.eof())
				{
					tilesAndCounts >> next;
					if ((t = next.find(".")) != std::string::npos && curTile != -1 && curDir != TileDir::Count)
					{
						curNeighbor = std::stoi(next.substr(0, t));
						curCount = std::stoi(next.substr(t + 1, std::string::npos));
						SetTileNeighborCount(curTile, curNeighbor, curCount, curDir);
					}
				}
			}
			else if ((t = nextLine.find("south")) != std::string::npos)
			{
				curDir = TileDir::South;
				curNeighbor = -1;
				curCount = -1;
				tilesAndCounts.str(nextLine.substr(t + 7, std::string::npos));
				std::string next;
				while (!tilesAndCounts.eof())
				{
					tilesAndCounts >> next;
					if ((t = next.find(".")) != std::string::npos && curTile != -1 && curDir != TileDir::Count)
					{
						curNeighbor = std::stoi(next.substr(0, t));
						curCount = std::stoi(next.substr(t + 1, std::string::npos));
						SetTileNeighborCount(curTile, curNeighbor, curCount, curDir);
					}
				}
			}
			else if ((t = nextLine.find("west")) != std::string::npos)
			{
				curDir = TileDir::West;
				curNeighbor = -1;
				curCount = -1;
				tilesAndCounts.str(nextLine.substr(t + 6, std::string::npos));
				std::string next;
				while (!tilesAndCounts.eof())
				{
					tilesAndCounts >> next;
					if ((t = next.find(".")) != std::string::npos && curTile != -1 && curDir != TileDir::Count)
					{
						curNeighbor = std::stoi(next.substr(0, t));
						curCount = std::stoi(next.substr(t + 1, std::string::npos));
						SetTileNeighborCount(curTile, curNeighbor, curCount, curDir);
					}
				}
			}
		}
		return true;
	}
}

bool TileDatabase::ExportDatabase(std::string _oFilename)
{
	if (imported)
	{
		return true;
	}

	output.open(_oFilename);

	if (!output.is_open())
	{
		//
		return false;
	}

	else
	{
		std::map<int, int> *toPrint;
		for (std::map<int, TileInfo>::iterator it = allTiles.begin(); it != allTiles.end(); it++)
		{
			output << "tile" << it->first << std::endl;
			output << "north: ";
			toPrint = it->second.GetNeighborList(TileDir::North)->GetNeighborList();
			for (std::map<int, int>::iterator it2 = toPrint->begin(); it2 != toPrint->end(); it2++)
			{
				output << it2->first << "." << it2->second << " ";
			}

			output << std::endl << "east: ";
			toPrint = it->second.GetNeighborList(TileDir::East)->GetNeighborList();
			for (std::map<int, int>::iterator it2 = toPrint->begin(); it2 != toPrint->end(); it2++)
			{
				output << it2->first << "." << it2->second << " ";
			}

			output << std::endl << "south: ";
			toPrint = it->second.GetNeighborList(TileDir::South)->GetNeighborList();
			for (std::map<int, int>::iterator it2 = toPrint->begin(); it2 != toPrint->end(); it2++)
			{
				output << it2->first << "." << it2->second << " ";
			}

			output << std::endl << "west: ";
			toPrint = it->second.GetNeighborList(TileDir::West)->GetNeighborList();
			for (std::map<int, int>::iterator it2 = toPrint->begin(); it2 != toPrint->end(); it2++)
			{
				output << it2->first << "." << it2->second << " ";
			}

			output << std::endl << "end" << std::endl << std::endl;
		}
		output.close();
	}
}
