#pragma once
#include "raylib.h"
#include <vector>

struct Tilemap
{
    int width;      // Map width in tiles
    int height;     // Map height in tiles
    int tileWidth;  // Tile width in pixels
    int tileHeight; // Tile height in pixels

    Texture2D tileset; // The entire tileset image

    std::vector<int> layerData; // The array of tile GIDs (Global IDs)
};

// Function to load the map data (implemented in .cpp)
Tilemap LoadTilemap(const char *mapFilePath, const char *tilesetImagePath);

// Function to draw the map
void DrawTilemap(const Tilemap &map, const Vector2 &offset);