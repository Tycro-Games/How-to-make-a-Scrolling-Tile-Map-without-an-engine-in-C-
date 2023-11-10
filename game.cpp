// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include <iostream>
uint widthX, heightY;
uint* tileArray;
Surface* tilePalette = nullptr;
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	// anything that happens only once at application start goes here
	//load pallete
	tilePalette = new Surface("assets/BasicTilemap.png");
	//load the tileArray

	LoadCSVFile("assets/Tilemap.tmx");
	//for (int i = 0; i < heightY; i++)
	//{
	//	for (int j = 0; j < widthX; j++)
	//	{
	//		tileArray[j + i * widthX]; //traversing a 1D array as it was a 2D one
	//	}
	//}
}

void Game::ConvertCharToInt(const char* pch, uint& numberForm)
{
	uint moveToRight = 1;
	for (size_t i = 0; i < strlen(pch); i++)
	{
		numberForm = numberForm * moveToRight + pch[i] - '0'; //conversion from char to int
		moveToRight = 10;
	}
}

void Game::ExtractWidthHeight(const char* csvRaw, uint& w, uint& h)
{
	//widthX and heightY get
	char* tileRaw = new char[strlen(csvRaw) + 1];
	strcpy(tileRaw, csvRaw);

	char* getX = strstr(tileRaw, "width=") + strlen("width=");
	char* getY = strstr(tileRaw, "height=") + strlen("height=");
	const char* pX = strtok(getX, "\"");
	const char* pY = strtok(getY, "\"");

	ConvertCharToInt(pX, w);

	ConvertCharToInt(pY, h);
	delete[] tileRaw;
}

void Game::LoadCSVFile(const char* csvPath)
{
	//copy into a c style string
	char* tilemapRaw = new char[strlen(TextFileRead(csvPath).c_str()) + 1];
	strcpy(tilemapRaw, TextFileRead(csvPath).c_str());

	//print the whole file
	//cout << tilemapRaw << "\n";

	ExtractWidthHeight(tilemapRaw, widthX, heightY);
	//make a new array of unsigned intergers
	tileArray = new uint[widthX * heightY];

	//get to the start of the csv
	char* startOfCsv = strstr(tilemapRaw, "csv\">\n") + strlen("csv\">\n");
	//get to the first element of the csv
	const char* pch = strtok(startOfCsv, ",");

	int index = 0;
	while (*pch != '<') //stops when it reaches the end of the csv
	{
		uint numberForm = 0;
		ConvertCharToInt(pch, numberForm);

		tileArray[index++] = numberForm;
		pch = strtok(nullptr, ",\n");
	}

	delete[] tilemapRaw;
}

//{{
//	//the x and y for the left down corner of the tile
//	const int maxScreenX = screenX + TILE_SIZE;
//	const int maxScreenY = screenY + TILE_SIZE;
//	//surface bounds
//	const int minBoundaryX = 0;
//	const int minBoundaryY = 0;
//	const int maxBoundaryX = surface->width;
//	const int maxBoundaryY = surface->height;
//	//determine if tile is on surface
//	if (maxScreenX >= minBoundaryX && maxScreenY >= minBoundaryY
//		&& screenX <= maxBoundaryX && screenY <= maxBoundaryY)
//	{
//		//partial rendering of tiles
//		const int minOffsetX = max(minBoundaryX, screenX);
//		const int minOffsetY = max(minBoundaryY, screenY);
//
//		const int maxOffsetX = min(maxBoundaryX, maxScreenX);
//		const int maxOffsetY = min(maxBoundaryY, maxScreenY);
//
//		//offsets for surface and source
//
//		const uint2 offsetMin = {
//			abs((minOffsetX - (screenX))),
//			abs((minOffsetY - screenY))
//		};
//
//		const uint2 offsetMax = {
//			abs((maxOffsetX - maxScreenX)),
//			abs((maxOffsetY - maxScreenY))
//		};
//
//		screenX += offsetMin.x;
//		screenY += offsetMin.y;
//
//		sourceX += offsetMin.x;
//		sourceY += offsetMin.y;

const uint TILE_SIZE = 32;

void Game::RenderTile(const Surface* surface,
                      int screenX, int screenY, uint sourceX, uint sourceY)
{
	//note positive y is downwards in this template

	//the x and y for the right down corner of the tile
	const int tileMaxX = screenX + TILE_SIZE;
	const int tileMaxY = screenY + TILE_SIZE;

	//screen resolutions
	const int maxBoundaryX = surface->width;
	const int maxBoundaryY = surface->height;

	//determine if the tile is partially on screen
	if (tileMaxX >= 0 && tileMaxY >= 0
		&& screenX <= maxBoundaryX && screenY <= maxBoundaryY)
	{
		//partial rendering of tiles
		//minimum amount that is on screen
		const int minOffsetX = max(0, screenX);
		const int minOffsetY = max(0, screenY);
		//maximum amount that is on screen
		const int maxOffsetX = min(maxBoundaryX, tileMaxX);
		const int maxOffsetY = min(maxBoundaryY, tileMaxY);

		//offsets for surface and source

		const uint2 offsetMin = {
			abs(minOffsetX - screenX),
			abs(minOffsetY - screenY)
		};

		const uint2 offsetMax = {
			abs(maxOffsetX - tileMaxX),
			abs(maxOffsetY - tileMaxY)
		};
		//offset the start of the tile
		screenX += offsetMin.x;
		screenY += offsetMin.y;

		sourceX += offsetMin.x;
		sourceY += offsetMin.y;

		//subtract the offset from the amount of pixels we have to draw

		const uint width = TILE_SIZE - offsetMin.x - offsetMax.x;
		const uint height = TILE_SIZE - offsetMin.y - offsetMax.y;
		//the destination and source pixel is computed using our traversal of 1D array to 2D array.
		uint* dst = surface->pixels
			+ screenX + screenY * surface->width;

		uint* src = tilePalette->pixels
			+ sourceX + sourceY * tilePalette->width;
		//copy pixels
		for (uint i = 0; i < height; i++)
		{
			for (uint j = 0; j < width; j++)
			{
				dst[j] = src[j];
			}
			//next row
			dst += surface->width;
			src += tilePalette->width;
		}
	}
}


// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
float2 position = 0;
float2 directionToMoveTo = 0;

void Game::Tick(float /* deltaTime */)
{
	//makes screen black
	screen->Clear(0);
	//move from the arrow keys (negative)
	position = position - directionToMoveTo;

	//render the tile map
	for (uint i = 0; i < heightY; i++)
	{
		for (uint j = 0; j < widthX; j++)
		{
			//getting the index of our array
			uint index = tileArray[j + i * widthX];
			//if the index is not 0, in other words, if there is a tile we should draw
			if (index > 0)
			{
				//so the index can start from 0
				index--;
				//we do some maths to adjust the source x and y
				//will get the y coordinate of the tile 
				const uint source_y = index / (tilePalette->width / TILE_SIZE);
				//will get the x coordinate of the tile 
				const uint source_x = index % (tilePalette->width / TILE_SIZE);
				//we draw our tile
				RenderTile(screen,
				           j * TILE_SIZE + position.x,
				           i * TILE_SIZE + position.y,
				           source_x * TILE_SIZE,
				           source_y * TILE_SIZE);
			}
		}
	}
}

void Game::KeyUp(int key)
{
	switch (key)
	{
	case GLFW_KEY_LEFT:
		directionToMoveTo.x -= -1;
		break;
	case GLFW_KEY_RIGHT:
		directionToMoveTo.x -= 1;
		break;
	case GLFW_KEY_DOWN:
		directionToMoveTo.y -= 1;
		break;
	case GLFW_KEY_UP:
		directionToMoveTo.y -= -1;
		break;
	}
}

void Game::KeyDown(int key)
{
	switch (key)
	{
	case GLFW_KEY_LEFT:
		directionToMoveTo.x += -1;
		break;
	case GLFW_KEY_RIGHT:
		directionToMoveTo.x += 1;
		break;
	case GLFW_KEY_DOWN:
		directionToMoveTo.y += 1;
		break;
	case GLFW_KEY_UP:
		directionToMoveTo.y += -1;
		break;
	}
}
