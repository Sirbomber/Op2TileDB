#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Tethys\API\API.h>

#include "TileDatabase\include\TileDatabase.h"

#include <list>
#include "MapGenerator\include\MapFeatures.h"
#include <iostream>
//#include <fstream>
//#include <sstream>

using namespace Tethys;
using namespace Tethys::TethysAPI;

//std::ifstream input;
//std::ofstream output;

// temp
void ScanAllBaseMaps(std::string _pathToMapsFolder);
void AnalyzeMap();

/*
bool ReadTileDatabase(std::string _pathToDbFile);
bool WriteTileDatabase(std::string _pathToDbFile);
*/

EXPORT_OP2_MULTIPLAYER_SCRIPT("Random Map Generator - Proof of Concept", MissionType::Colony, 1, "tutorial.map", "multitek.txt");

TileDatabase tileDB;

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) 
	{
		DisableThreadLibraryCalls(hinstDLL);
	}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{
	}

	return TRUE;
}
std::list<Location> nextPlacements;
Trigger randomize;
bool mirrorX = false,
	 mirrorY = false;

void Randomize(bool _visualize)
{
	// Blank the map
	for (int y = 1; y <= GameMap::GetHeight(); y++)
	{
		for (int x = 1; x <= GameMap::GetWidth(); x++)
		{
			GameMap::SetTile(GameMap::At(x, y), 0);
		}
	}

	nextPlacements.clear();

	// Place seed tiles
	// gray cliff wall bottom left corner: {70 71 72} {73 74 75} {76 77 78} {79 80 81} {82 83 84} {85 86 87} (3x6)
	int rF = Game::GetRand(sizeof(allFeatures) / sizeof(Feature));
	int* seedTileDefs = allFeatures[rF].tiles;
	int seedWidth = allFeatures[rF].width;		// size of the terrain feature used for seeding
	int seedHeight = allFeatures[rF].height;
	int seedX = Game::GetRand(GameMap::GetWidth() - seedWidth);
	int seedY = Game::GetRand(GameMap::GetHeight() - seedHeight);
	Player[0].CenterViewOn(Location(seedX, seedY));
	std::cout << "Placed feature #" << rF << " at " << seedX << ", " << seedY << std::endl;

	for (int y = 0; y < seedHeight; y++)
	{
		for (int x = 0; x < seedWidth; x++)
		{
			// Set current seed tile
			GameMap::SetTile(GameMap::At(seedX + x, seedY + y), seedTileDefs[x + (y * seedWidth)]);

			// Add tiles along the north edge of the seed placement to the tiles to update list
			if (y == 0 && seedY - 1 > 0)
			{
				nextPlacements.push_back(Location(seedX + x, seedY + y - 1));
			}
			// Get the southern edge
			else if (y == seedHeight - 1 && y + 1 <= GameMap::GetHeight())
			{
				nextPlacements.push_back(Location(seedX + x, seedY + y + 1));
			}

			// Now do the western edge
			if (x == 0 && seedX - 1 > 0)
			{
				nextPlacements.push_back(Location(seedX + x - 1, seedY + y));
			}
			// And the eastern edge
			else if (x == seedWidth - 1 && x + 1 <= GameMap::GetWidth())
			{
				nextPlacements.push_back(Location(seedX + x + 1, seedY + y));
			}
		}
	}

	if (_visualize)
	{
		randomize = CreateTimeTrigger(12, "RandomizeInPhases", false, true);
	}
	else
	{
		// Place random tiles
		Location nextTile;
		while (!nextPlacements.empty())
		{
			nextTile = nextPlacements.front();
			nextPlacements.pop_front();

			// Skip already-processed tiles.
			if (GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y)) != 0)
			{
				continue;
			}

			TileAndDir neighbors[4];
			int numNeighbors = 0;

			int nextNeighbor;
			if (nextTile.y > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y - 1))) != 0)
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::South;
				numNeighbors++;
			}

			if (nextTile.x > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x - 1, nextTile.y))) != 0) // needs world map logic
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::East;
				numNeighbors++;
			}

			if (nextTile.y < GameMap::GetHeight() && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y + 1))) != 0)
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::North;
				numNeighbors++;
			}

			if (nextTile.x < GameMap::GetWidth() && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x + 1, nextTile.y))) != 0) // needs world map logic
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::West;
				numNeighbors++;
			}

			GameMap::SetTile(GameMap::At(nextTile.x, nextTile.y), tileDB.PickTileBasedOnAllNeighbors(neighbors, numNeighbors));

			// Update list of tiles to update
			if (nextTile.y > 1 && !GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y - 1)))
			{
				nextPlacements.push_back(Location(nextTile.x, nextTile.y - 1));
			}
			else if (nextTile.y < GameMap::GetHeight() && !GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y + 1)))
			{
				nextPlacements.push_back(Location(nextTile.x, nextTile.y + 1));
			}
			if (nextTile.x > 1 && !GameMap::GetTile(GameMap::At(nextTile.x - 1, nextTile.y)))
			{
				nextPlacements.push_back(Location(nextTile.x - 1, nextTile.y));
			}
			else if (nextTile.x < GameMap::GetWidth() && !GameMap::GetTile(GameMap::At(nextTile.x + 1, nextTile.y)))
			{
				nextPlacements.push_back(Location(nextTile.x + 1, nextTile.y));
			}
		}
	}
	
}

// For visualization purposes
MISSION_API void RandomizeInPhases()
{
	// Place random tiles
	Location nextTile;
	std::list<Location> tempPlacements;

	while (!nextPlacements.empty())
	{
		nextTile = nextPlacements.front();
		nextPlacements.pop_front();

		// Skip already-processed tiles.
		if (GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y)) != 0)
		{
			continue;
		}

		TileAndDir neighbors[4];
		int numNeighbors = 0;

		int nextNeighbor;
		if (nextTile.y > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y - 1))) != 0)
		{
			neighbors[numNeighbors].t = nextNeighbor;
			neighbors[numNeighbors].d = TileDir::South;
			numNeighbors++;
		}

		if (nextTile.x > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x - 1, nextTile.y))) != 0) // needs world map logic
		{
			neighbors[numNeighbors].t = nextNeighbor;
			neighbors[numNeighbors].d = TileDir::East;
			numNeighbors++;
		}

		if (nextTile.y < GameMap::GetHeight() && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y + 1))) != 0)
		{
			neighbors[numNeighbors].t = nextNeighbor;
			neighbors[numNeighbors].d = TileDir::North;
			numNeighbors++;
		}

		if (nextTile.x < GameMap::GetWidth() && (nextNeighbor = GameMap::GetTile(GameMap::At(nextTile.x + 1, nextTile.y))) != 0) // needs world map logic
		{
			neighbors[numNeighbors].t = nextNeighbor;
			neighbors[numNeighbors].d = TileDir::West;
			numNeighbors++;
		}

		GameMap::SetTile(GameMap::At(nextTile.x, nextTile.y), tileDB.PickTileBasedOnAllNeighbors(neighbors, numNeighbors));

		// Update list of tiles to update
		if (nextTile.y > 1 && !GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y - 1)))
		{
			tempPlacements.push_back(Location(nextTile.x, nextTile.y - 1));
		}
		else if (nextTile.y < GameMap::GetHeight() && !GameMap::GetTile(GameMap::At(nextTile.x, nextTile.y + 1)))
		{
			tempPlacements.push_back(Location(nextTile.x, nextTile.y + 1));
		}
		if (nextTile.x > 1 && !GameMap::GetTile(GameMap::At(nextTile.x - 1, nextTile.y)))
		{
			tempPlacements.push_back(Location(nextTile.x - 1, nextTile.y));
		}
		else if (nextTile.x < GameMap::GetWidth() && !GameMap::GetTile(GameMap::At(nextTile.x + 1, nextTile.y)))
		{
			tempPlacements.push_back(Location(nextTile.x + 1, nextTile.y));
		}
	}

	nextPlacements = tempPlacements;
	if (nextPlacements.empty())
	{
		randomize.Destroy();
	}
}

// Old test - randomize by placing a single random seed tile at 1,1 and going row by row, column by column
MISSION_API void Randomize_Linear()
{
	// Blank the map
	for (int y = 1; y <= GameMap::GetHeight(); y++)
	{
		for (int x = 1; x <= GameMap::GetWidth(); x++)
		{
			GameMap::SetTile(GameMap::At(x, y), 0);
		}
	}

	GameMap::SetTile(GameMap::At(1, 1), Game::GetRand(12) + 1);
	for (int y = 1; y <= GameMap::GetHeight(); y++)
	{
		for (int x = 1; x <= GameMap::GetWidth(); x++)
		{
			if (x == 1 && y == 1)
			{
				continue;
			}
			TileAndDir neighbors[4];
			int numNeighbors = 0;

			int nextNeighbor;
			if (y > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(x, y - 1))) != 0)
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::South;
				numNeighbors++;
			}

			if (x > 1 && (nextNeighbor = GameMap::GetTile(GameMap::At(x - 1, y))) != 0) // needs world map logic
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::East;
				numNeighbors++;
			}

			if (y < GameMap::GetHeight() && (nextNeighbor = GameMap::GetTile(GameMap::At(x, y + 1))) != 0)
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::North;
				numNeighbors++;
			}

			if (x < GameMap::GetWidth() && (nextNeighbor = GameMap::GetTile(GameMap::At(x + 1, y))) != 0) // needs world map logic
			{
				neighbors[numNeighbors].t = nextNeighbor;
				neighbors[numNeighbors].d = TileDir::West;
				numNeighbors++;
			}

			GameMap::SetTile(GameMap::At(x, y), tileDB.PickTileBasedOnAllNeighbors(neighbors, numNeighbors));
		}
	}
}

MISSION_API void OnChat(OnChatArgs* pArgs)
{
	if (strcmp(pArgs->pText, "/randomize") == 0)
	{
		if (randomize.IsValid())
		{
			Game::AddMessage("Map generator already running!", SoundID::Dirt);
		}
		else
		{
			Randomize(false);
		}
	}
	else if (strcmp(pArgs->pText, "/visualize") == 0)
	{
		if (randomize.IsValid())
		{
			Game::AddMessage("Map generator already running!", SoundID::Dirt);
		}
		else
		{
			Randomize(true);
		}
	}
	else if (strcmp(pArgs->pText, "/linear") == 0)
	{
		if (randomize.IsValid())
		{
			Game::AddMessage("Map generator already running!", SoundID::Dirt);
		}
		else
		{
			Randomize_Linear();
		}
	}
	else if (strcmp(pArgs->pText, "/stop") == 0)
	{
		if (randomize.IsValid())
		{
			randomize.Destroy();
			nextPlacements.clear();
		}
	}
	else if (strcmp(pArgs->pText, "/mirrorx") == 0)
	{
		if (randomize.IsValid())
		{
			Game::AddMessage("Cannot change generator settings while running!", SoundID::Dirt);
		}
		else
		{
			mirrorX = !mirrorX;
			Game::AddMessage("Symmetrical generation not yet implemented", SoundID::Dirt);
		}
	}
	else if (strcmp(pArgs->pText, "/mirrory") == 0)
	{
		if (randomize.IsValid())
		{
			Game::AddMessage("Cannot change generator settings while running!", SoundID::Dirt);
		}
		else
		{
			mirrorY = !mirrorY;
			Game::AddMessage("Symmetrical generation not yet implemented", SoundID::Dirt);
		}
	}
	/*
	/// Info passed to OnChat() user callback.
struct OnChatArgs {
  size_t structSize;  ///< Size of this structures.
  char*  pText;       ///< Chat message (writable).
  size_t bufferSize;  ///< Size of chat message buffer.
  int    playerNum;   ///< Source player number.
};
*/
//
}


MISSION_API ibool InitProc()
{
	if (tileDB.ImportDatabase("C:\\Sierra\\Outpost 2 Divided Destiny\\OPU\\maps\\Dev\\tileDB.txt"))
	{
		//
	}
	else
	{
		ScanAllBaseMaps("C:\\Sierra\\Outpost 2 Divided Destiny\\OPU\\base\\maps");
		tileDB.ExportDatabase("C:\\Sierra\\Outpost 2 Divided Destiny\\OPU\\maps\\Dev\\tileDB.txt");
	}
	GameMap::Load("on4_01.map");
	Player[0].CenterViewOn(GameMap::At(2, 2));

	Trigger t = CreateTimeTrigger(100, "None", true, false);
	CreateVictoryCondition(t, "/linear - Run an outdated generator", true, false);
	CreateVictoryCondition(t, "/mirrory - Toggle vertical symmetry (not implemented)", true, false);
	CreateVictoryCondition(t, "/mirrorx - Toggle horizontal symmetry (not implemented)", true, false);
	CreateVictoryCondition(t, "/stop - Stop generation after using /visualize", true, false);
	CreateVictoryCondition(t, "/visualize - Same as /generate but pause briefly after each step, to watch it happen", true, false);
	CreateVictoryCondition(t, "/generate - Generate a map.  A random doodad is placed at a random location, and then tiles are placed radially outwards", true, false);
	CreateVictoryCondition(t, "Command list:", true, false);
	return 1;
}

// ------------------------------------------------------------------
// The usual OP2 stuff
MISSION_API void  AIProc()
{
	//
}

MISSION_API void  GetSaveRegions(SaveRegion* pSave)
{
	pSave->pData = nullptr;
	pSave->size = 0;
}

// I use "None" instead of "NoResponseToTrigger" because it's shorter
MISSION_API void None()
{
	//
}

// ------------------------------------------------------------------

#include <filesystem>

void ScanAllBaseMaps(std::string _pathToMapsFolder)
{
	for (auto& p : std::filesystem::recursive_directory_iterator(_pathToMapsFolder))
	{
		if (p.path().extension() == ".map")
		{
			GameMap::Load(p.path().filename().string());
			AnalyzeMap();
		}
	}
}

void AnalyzeMap()
{
	// If "world map" flag set (future OPU patch feature) or map width >= 512, we need to do special checks along the left/right edge
	bool worldMap = (GameMap::GetWidth() >= 512);

	for (int y = 1; y <= GameMap::GetHeight(); y++)
	{
		for (int x = 1; x <= GameMap::GetWidth(); x++)
		{
			// get current tile
			// get all 4 neighbors (exclude top edge at y = 0, bottom at y = map height, left edge at x = 0 unless world map, right edge at x = map width unless world map)
			// update neighbor lists for each direction

			// Check north
			if (y != 1)
			{
				tileDB.UpdateTileInDatabase(GameMap::GetTile(GameMap::At(x, y)), GameMap::GetTile(GameMap::At(x, y - 1)), TileDir::North);
			}

			// Check south
			if (y != GameMap::GetHeight())
			{
				tileDB.UpdateTileInDatabase(GameMap::GetTile(GameMap::At(x, y)), GameMap::GetTile(GameMap::At(x, y + 1)), TileDir::South);
			}

			// Check east
			if (x != GameMap::GetWidth() || worldMap)
			{
				tileDB.UpdateTileInDatabase(GameMap::GetTile(GameMap::At(x, y)), GameMap::GetTile(GameMap::At((x + 1) % GameMap::GetWidth(), y)), TileDir::East);
			}

			// Check west
			if (x != 1 || worldMap)
			{
				tileDB.UpdateTileInDatabase(GameMap::GetTile(GameMap::At(x, y)), GameMap::GetTile(GameMap::At((x - 1) % GameMap::GetWidth(), y)), TileDir::West);
			}
		}
	}

	return;
}

/*
bool ReadTileDatabase(std::string _pathToDbFile)
{
	input.open(_pathToDbFile);

	if (!input.is_open())
	{
		return false;
	}

	std::string nextLine;
	while (std::getline(input, nextLine))
	{
		//
	}
}

bool WriteTileDatabase(std::string _pathToDbFile)
{
	output.open(_pathToDbFile);

	if (!output.is_open())
	{
		Game::AddMessage("Failed to write tile database!", SoundID::Dirt);
		return false;
	}

	return false;
}
*/