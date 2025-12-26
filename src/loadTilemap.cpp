#include "loadTilemap.hpp"
#include "raylib.h"
#include <string>
#include <algorithm> // for std::remove_if

// Helper function to extract an integer value from JSON data
int ExtractIntValue(const std::string &jsonString, const std::string &key)
{
    // Search for the key (e.g., "width")
    std::string searchKey = "\"" + key + "\":";
    size_t pos = jsonString.find(searchKey);

    if (pos == std::string::npos)
        return 0;

    // Find the start of the value after the key
    size_t valueStart = pos + searchKey.length();

    // Find the end of the value (usually a comma or bracket)
    size_t valueEnd = jsonString.find_first_of(",}]", valueStart);

    if (valueEnd == std::string::npos)
        return 0;

    // Extract and convert the substring to an integer
    std::string valueStr = jsonString.substr(valueStart, valueEnd - valueStart);
    return std::stoi(valueStr);
}

Tilemap LoadTilemap(const char *mapFilePath, const char *tilesetImagePath)
{
    Tilemap map = {};

    // 1. Load the JSON file into a string
    int dataSize = 0;
    unsigned char *fileData = LoadFileData(mapFilePath, &dataSize);
    if (!fileData)
        return map; // Check for failure

    std::string jsonString((char *)fileData, dataSize);
    UnloadFileData(fileData);

    // 2. Extract map properties (no calculations needed!)
    map.width = ExtractIntValue(jsonString, "width");
    map.height = ExtractIntValue(jsonString, "height");
    map.tileWidth = ExtractIntValue(jsonString, "tilewidth");
    map.tileHeight = ExtractIntValue(jsonString, "tileheight");

    // 3. Load the tileset image
    map.tileset = LoadTexture(tilesetImagePath);

    // 4. Extract Layer Data (assuming one tile layer)
    size_t dataArrayStart = jsonString.find("\"data\":[");
    size_t dataArrayEnd = jsonString.find("]", dataArrayStart);

    if (dataArrayStart != std::string::npos && dataArrayEnd != std::string::npos)
    {
        std::string dataStr = jsonString.substr(dataArrayStart + 8, dataArrayEnd - dataArrayStart - 8);

        // Remove spaces and newlines
        dataStr.erase(std::remove_if(dataStr.begin(), dataStr.end(), ::isspace), dataStr.end());

        // Split by comma and convert to integers
        size_t start = 0;
        size_t end = dataStr.find(',');
        while (end != std::string::npos)
        {
            map.layerData.push_back(std::stoi(dataStr.substr(start, end - start)));
            start = end + 1;
            end = dataStr.find(',', start);
        }
        // Push the last tile ID
        map.layerData.push_back(std::stoi(dataStr.substr(start)));
    }

    // You now have the tile data array and map dimensions!
    return map;
}

// Function to draw the tilemap (where the simple math happens)
void DrawTilemap(const Tilemap &map, const Vector2 &offset)
{
    if (map.layerData.empty())
        return;

    int tilesetCols = map.tileset.width / map.tileWidth;

    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            int tileIndex = y * map.width + x;
            int gid = map.layerData[tileIndex];

            // GID 0 means no tile
            if (gid == 0)
                continue;

            // Simple drawing math (no complex calculation needed)
            int tileGid = gid - 1; // Tiled GIDs are 1-based, array is 0-based

            // Source rectangle on the tileset image
            Rectangle sourceRec = {
                (float)(tileGid % tilesetCols) * map.tileWidth,
                (float)(tileGid / tilesetCols) * map.tileHeight,
                (float)map.tileWidth,
                (float)map.tileHeight};

            // Destination rectangle on the screen
            Rectangle destRec = {
                offset.x + (float)x * map.tileWidth,
                offset.y + (float)y * map.tileHeight,
                (float)map.tileWidth,
                (float)map.tileHeight};

            // Draw the single tile
            DrawTexturePro(map.tileset, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
        }
    }
}