#pragma once

#define PLACEHOLDER_TILE 0	// 0 is the tile ID of the default blue placeholder tile.

enum class TileDir
{
	North,
	East,
	South,
	West,
	Count
};

struct TileAndDir
{
	int t;
	TileDir d;
};
