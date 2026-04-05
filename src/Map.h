#pragma once

#include "Module.h"
#include "XMLHandler.h"
#include "Player.h"
#include <list>
#include <vector>

// L09: TODO 5: Add attributes to the property structure
struct Properties
{
    struct Property
    {
        std::string name;
        std::string type;
        bool value_b;
        std::string value_s;
        int value_i;
        float value_f;
    };

    std::list<Property*> propertyList;

    ~Properties()
    {
        for (const auto& property : propertyList)
        {
            delete property;
        }

        propertyList.clear();
    }

    // L09: DONE 7: Method to ask for the value of a custom property
    Property* GetProperty(const char* name)
    {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                return property;
            }
        }

        return nullptr;
    }

    void ChangePorperty(const char* name, bool value) {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                property->value_b = value;
            }
        }

        return;
    }

    void ChangePorperty(const char* name, std::string value) {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                property->value_s = value;
            }
        }

        return;
    }

    void ChangePorperty(const char* name, int value) {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                property->value_i = value;
            }
        }

        return;
    }

    void ChangePorperty(const char* name, float value) {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                property->value_f = value;
            }
        }

        return;
    }

};

struct MapLayer
{
    // L07: TODO 1: Add the info to the MapLayer Struct
    int id;
    std::string name;
    int width;
    int height;
    std::vector<int> tiles;
    Properties properties;

    // L07: TODO 6: Short function to get the gid value of i,j
    unsigned int Get(int i, int j) const
    {
        return tiles[(j * width) + i];
    }
};

// L06: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct TileSet
{
    int firstGid;
    std::string name;
    int tileWidth;
    int tileHeight;
    int renderWidth;
    int renderHeight;
    int spacing;
    int margin;
    int tileCount;
    int columns;
    SDL_Texture* texture;

    // L07: TODO 7: Implement the method that receives the gid and returns a Rect
    SDL_Rect GetRect(unsigned int gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstGid;
        rect.w = tileWidth;
        rect.h = tileHeight;
        rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);

        return rect;
    }

};

struct Object
{
    int id;
    float x, y, width, height;
    Properties properties;
};

struct ObjectGroup
{
    int id;
    std::string name;
    std::list<Object*> objects;
};

// L06: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
    std::list<TileSet*> tilesets;

    // L07: TODO 2: Add the info to the MapLayer Struct
    std::list<MapLayer*> layers;
    std::list<ObjectGroup*> objectlayers;
};

struct AccessData {
    Vector2D position;
    float width, height;
    std::string targetSceneId;
    std::string targetSpawnId;
};

struct SpawnPoint {
    std::string spawnId;
    Vector2D position;
};

struct GameData {
    std::vector<SpawnPoint> spawnPoints;
    std::vector<NPCData> npcs;
    std::vector<ItemData> items;
    std::vector<AccessData> accesses;
};

class Map
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(std::string path, std::string mapFileName);

    Vector2D MapToWorld(int x, int y) const;
    Vector2D WorldToMap(int x, int y);

    // L09: TODO 2: Implement function to the Tileset based on a tile id
    TileSet* GetTilesetFromTileId(int gid) const;

    // L09: TODO 6: Load a group of properties 
    bool LoadProperties(pugi::xml_node& node, Properties& properties);

    // L10: TODO 7: Create a method to get the map size in pixels
    Vector2D GetMapSizeInPixels();
    Vector2D GetMapSizeInTiles();

    MapLayer* GetNavigationLayer();

    Vector2D GetCameraPositionInTiles();

    Vector2D GetCameraLimitsInTiles(Vector2D camPosTile);

    int GetTileWidth() {
        return mapData.tileWidth;
    }

    int GetTileHeight() {
        return mapData.tileHeight;
    }

public: 
    SDL_Texture* bgtexture;
    std::string mapFileName;
    std::string mapPath;
    Vector2D* playerStartPos = nullptr;
    MapData mapData;
    GameData gameData;
    bool mapLoaded;

private:
    float scale = 2.0f;
    // L06: DONE 1: Declare a variable data of the struct MapData
};