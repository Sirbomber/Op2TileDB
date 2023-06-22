#pragma once

#include <map>
#include <string>

#include "TileDb_Types.h"
#include "TileInfo.h"



class TileDatabase
{
public:
	TileDatabase() {};
	~TileDatabase() {};

	// Database file I/O
	bool ImportDatabase(std::string _iFilename);
	bool ExportDatabase(std::string _oFilename);

	// Methods used to seed database by examining existing maps
	void UpdateTileInDatabase(int _srcTile, int _neighborTile, TileDir _dirOfNextTile);
	void SetTileNeighborCount(int _srcTile, int _neighborTile, int _neighborCount, TileDir _dirOfNextTile);

	//void AnalyzeMap(std::string _mapName);-
	//void ProcessTile(int _x, int _y, int _mapWidth, int _mapHeight, bool _isWorldMap = false);

	// Methods used to generate new maps based on tile distribution info in database
	int PickNextTile(int _srcTile, TileDir _dirOfNextTile);		// Return a tile ID at random, weighted by how frequently other tile IDs appear next to this tile in the passed-in direction.  If no tiles defined, return 0 (blue placeholder tile).
	int PickTileBasedOnAllNeighbors(TileAndDir _tiles[], int _num);

private:
	std::map<int, TileInfo> allTiles;
	bool imported;
};