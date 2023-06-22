#include <stdio.h>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "..\TileDatabase\include\TileDatabase.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int tsWidth = 1536;
int tsHeight = 1344;
const int MAX_TILES = (tsWidth / 32) * (tsHeight / 32);
// SDL textures can't be greater than 16384x16384
// vanilla OP2 has 2012 tiles
// each tile is 32x32 pixels
// a texture with dimensions 1536x1344 can store 2016 tiles, 48 tiles per row across 42 rows
int tileCount = 0;
std::string tilesetPath = "C:\\Sierra\\Outpost 2 Divided Destiny\\OPU\\base\\tilesets\\";
int cameraX = 0;
int cameraY = 0;
TileDatabase tileDb;
bool showTilesheet = false;
int curDisplayedTile = 0;

SDL_Window* w = NULL;
SDL_Renderer* r = NULL;
SDL_Texture* tilesets;
TTF_Font* f = NULL;

struct tOffset
{
	int x, y;
};

tOffset GetTileOffset(int _tId)
{
	return tOffset{ (_tId * 32) % tsWidth, ((_tId * 32) / tsWidth) * 32 };
}

void SetCurTile(int _amt)
{
	curDisplayedTile += _amt;
	if (curDisplayedTile < 0)
	{
		curDisplayedTile = 0;
	}
	else if (curDisplayedTile >= tileCount)
	{
		curDisplayedTile = tileCount - 1;
	}
}

int main(int argc, char* args[])
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else if (!tileDb.ImportDatabase("C:\\Sierra\\Outpost 2 Divided Destiny\\OPU\\maps\\Dev\\tileDB.txt"))
	{
		std::cout << "Could not import tile database; aborting." << std::endl;
	}
	else if (TTF_Init() == -1)
	{
		std::cout << "Could not initialize fonts.  Error: " << TTF_GetError() << std::endl;
	}
	else
	{
		// Create window
		w = SDL_CreateWindow("OP2 Tile Database Debugger/Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (w == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			// Get window surface and fill it black
			r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (r == NULL)
			{
				std::cout << "Couldn't create renderer (error: " << SDL_GetError() << ")" << std::endl;
				return -1;
			}
			tilesets = SDL_CreateTexture(r, SDL_GetWindowPixelFormat(w), SDL_TEXTUREACCESS_TARGET, tsWidth, tsHeight);
			if (tilesets == NULL)
			{
				std::cout << "Couldn't create tileset texture (error: " << SDL_GetError() << ")" << std::endl;
				return -1;
			}
			f = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 16);
			if (f == NULL)
			{
				std::cout << "Failed to load font.  Error: " << TTF_GetError() << std::endl;
				return -1;
			}

			SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
			SDL_RenderClear(r);

			// Load OP2 tilesets
			if (SDL_SetRenderTarget(r, tilesets) != 0)
			{
				std::cout << "Couldn't render to tileset texture (error: " << SDL_GetError() << ")" << std::endl;
				return -1;
			}
			SDL_Surface* well = NULL;
			SDL_Texture* optimized = NULL;
			for (int i = 0; i < 13; i++)
			{
				std::string wellStr = "well00" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i)) + ".bmp";
				well = SDL_LoadBMP((tilesetPath + wellStr).c_str());
				if (well == NULL)
				{
					std::cout << "Couldn't load tileset " << wellStr << std::endl;
					return -1;
				}
				optimized = SDL_CreateTextureFromSurface(r, well);
				if (optimized == NULL)
				{
					std::cout << "Couldn't optimize tileset " << wellStr << "(error: " << SDL_GetError() << ")" << std::endl;
					return -1;
				}
				int tilesheetX;
				int tilesheetY;
				for (int t = 0; t < well->h; t += 32)
				{
					// 1536x1344
					tilesheetX = (tileCount * 32) % tsWidth;
					tilesheetY = ((tileCount * 32) / tsWidth) * 32;
					SDL_Rect copyTo = { tilesheetX, tilesheetY, 32, 32 };
					SDL_Rect copyFrom = { 0, t, 32, 32 };
					SDL_RenderCopy(r, optimized, &copyFrom, &copyTo);
					tileCount++;
					if (tileCount == MAX_TILES)
					{
						std::cout << "Number of tiles (" << tileCount << " exceeded maximum possible (" << MAX_TILES << ")" << std::endl;
						return -1;
					}
				}
			}
			SDL_FreeSurface(well);
			SDL_DestroyTexture(optimized);
			if (SDL_SetRenderTarget(r, NULL) != 0)
			{
				std::cout << "Couldn't reset renderer (error: " << SDL_GetError() << ")" << std::endl;
				return -1;
			}
			

			// Test: draw tileset
			/*
			for (int t = (cameraY / 32) - 1; t < ((cameraY + SCREEN_HEIGHT) / 32) + 1; t++)
			{
				SDL_Rect drawFrom = { 0, t * 32, 32, 32 };
				SDL_Rect drawTo = { 0, cameraY, 32, 32 };
				SDL_RenderCopy(r, tilesets, &drawFrom, &drawTo);
			}
			*/


			// Main loop
			SDL_Event e;
			bool quit = false;
			while (!quit)
			{
				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						if (e.key.keysym.sym == SDLK_F5)
						{
							showTilesheet = !showTilesheet;
							cameraX = 0;
							cameraY = 0;
						}

						// These are intentionally not else-ifs
						if (e.key.keysym.sym == SDLK_UP)
						{
							if (showTilesheet)
							{
								cameraY -= 4;
								if (cameraY < 0)
								{
									cameraY = 0;
								}
							}
							else
							{
								SetCurTile(+10);
							}
						}
						if (e.key.keysym.sym == SDLK_DOWN)
						{
							if (showTilesheet)
							{
								cameraY += 4;
								if (cameraY > tsHeight - SCREEN_HEIGHT)
								{
									cameraY = tsHeight - SCREEN_HEIGHT;
								}
							}
							else
							{
								SetCurTile(-10);
							}
						}
						if (e.key.keysym.sym == SDLK_LEFT)
						{
							if (showTilesheet)
							{
								cameraX -= 4;
								if (cameraX < 0)
								{
									cameraX = 0;
								}
							}
							else
							{
								SetCurTile(-1);
							}
						}
						if (e.key.keysym.sym == SDLK_RIGHT)
						{
							if (showTilesheet)
							{
								cameraX += 4;
								if (cameraX > tsWidth - SCREEN_WIDTH)
								{
									cameraX = tsWidth - SCREEN_WIDTH;
								}
							}
							else
							{
								SetCurTile(+1);
							}
						}
					}
					else if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (showTilesheet)
						{
							x = (x + cameraX) / 32;
							y = ((y + cameraY) / 32) * (tsWidth / 32);
							if (x + y < tileCount)
							{
								std::cout << "Tile ID: " << x + y << std::endl;
							}
						}
						else
						{
							//
						}
					}
				}
				// Render
				SDL_RenderClear(r);
				if (showTilesheet)
				{
					SDL_Rect drawFrom = { 0, 0, tsWidth, tsHeight };
					SDL_Rect drawTo = { 0 - cameraX, 0 - cameraY, tsWidth, tsHeight };
					SDL_RenderCopy(r, tilesets, &drawFrom, &drawTo);
				}
				else
				{
					std::string tileIdString = "Tile ID: " + std::to_string(curDisplayedTile);
					SDL_Surface* textSurface = TTF_RenderText_Solid(f, tileIdString.c_str(), SDL_Color{ 255,255,255 });
					SDL_Texture* textTexture = SDL_CreateTextureFromSurface(r, textSurface);

					tOffset t = GetTileOffset(curDisplayedTile);
					SDL_Rect drawFrom = { t.x, t.y, 32, 32 };
					SDL_Rect drawTo = { 0, 0, 32, 32 };
					SDL_Rect drawTextTo = { 48,0,textSurface->w,textSurface->h };
					SDL_RenderCopy(r, tilesets, &drawFrom, &drawTo);
					SDL_RenderCopy(r, textTexture, NULL, &drawTextTo);

					SDL_FreeSurface(textSurface);
					SDL_DestroyTexture(textTexture);
				}
				SDL_RenderPresent(r);
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(w);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
