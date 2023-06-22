#pragma once

#define PLACEHOLDER_TILE 0	// 0 is the tile ID of the default blue placeholder tile.

//#define _OP2_MAP_BUILD
// Provide an interface into the random map generation logic as well as the various parameters that control tile selection.
// The map generator doesn't store map info and is agnostic as to the calling application (could be a mapper/external editor or an OP2 mission script, for example).

class MapGen
{
public:
	MapGen() {};
	~MapGen() {};

	// Map initialization
	void SetMapDimensions(int _width, int _height) {
		mapWidth = _width;
		mapHeight = _height;
		if (mapWidth >= 512) { worldMap = true; }
	}
	void BlankMap();	// Replaces the entire map with the placeholder tile.
	

	// File stuff
	void LoadMap();
	void SaveMap();

private:
	// Map generation
	void UpdateTile(int _x, int _y, int _tileId);	// Write the passed-in tile to the map at the designated coordinates.

	// Map data
	
	// Generator parameters
	int mapWidth = 128;				// Size of map to generate.
	int mapHeight = 128;			// Size of map to generate.
	bool worldMap = false;			// If "world map" flag set (future OPU patch feature) or map width >= 512, we need to do special checks along the left/right edge.
	bool mirrorX = false;			// If true, generator will try to make the map horizontally symmetrical.  Can be paired with the mirrorY parameter for full rotational symmetry.
	bool mirrorY = false;			// If true, generator will try to make the map vertically symmetrical.  Can be paired with the mirrorX parameter for full rotational symmetry.
	int maxOverwriteDepth = 0;		// Values >0 allow the generator to overwrite existing tiles up to the specified depth.
};
