#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <vector>
#include "Vector2D.h"

struct NPCData {
public:
    Vector2D position;
    std::string id;
};

struct ItemData {
public:
    Vector2D position;
    std::string id;
};

struct EnemyData {
public:
    Vector2D position;
    std::string id;
};

class SaveData {
public:
    SaveData(int _currentScene, int _time, int _lives, int _coins, bool _hasItem1, bool _hasItem2, Vector2D _playerPosition, Vector2D _playerRespawnPoint, std::vector<NPCData> _npcs, std::vector<ItemData> _items)
        : currentScene(_currentScene), time(_time), lives(_lives), coins(_coins), hasItem1(_hasItem1), hasItem2(_hasItem2), playerPosition(_playerPosition), playerRespawnPoint(_playerRespawnPoint), npcs(_npcs), items(_items), exists(true) { };

    SaveData()
        : currentScene(0), time(0), lives(0), coins(0), hasItem1(0), hasItem2(0), playerPosition({0, 0}), playerRespawnPoint({0, 0}), npcs({}), items({}), exists(false) { };

    bool exists;
    int currentScene;
    int time;
    int lives;
    int coins;
    bool hasItem1;
    bool hasItem2;
    Vector2D playerPosition; 
    Vector2D playerRespawnPoint;
    std::vector<NPCData> npcs;
    std::vector<ItemData> items;
};

static class XMLHandler {
public:

    static bool SaveFileExists();
    static SaveData Load();
    static void Save(SaveData data);

    static pugi::xml_document LoadFile(const char* filePath);

    static const char* saveFileName;
};
