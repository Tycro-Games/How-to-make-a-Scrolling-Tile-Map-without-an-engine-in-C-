// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
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

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(float /* deltaTime */)
{
	//makes screen black
	screen->Clear(0);
	//destination pixel
	uint* dst = screen->pixels;

	//source pixel
	const uint* src = tilePalette->pixels;

	const uint TILE_HEIGHT = 32;
	const uint TILE_WIDTH = 32;
	for (uint i = 0; i < TILE_HEIGHT; i++)
	{
		for (uint j = 0; j < TILE_WIDTH; j++)
		{
			dst[j] = src[j];
		}
		//these are also 1D arrays represented as 2D ones
		//go to the next row in the dst and src
		dst += screen->width;
		src += tilePalette->width;
	}
}
