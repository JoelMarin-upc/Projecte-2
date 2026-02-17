#pragma once
#include <iostream>
#include <pugixml.hpp>
#include <vector>
#include "Vector2D.h"

struct EnemyData {
public:
    Vector2D position;
    int enType;
};

struct ItemData {
public:
    Vector2D position;
    char type;
};

class SaveData {
public:
    SaveData(int _currentScene, int _time, int _lives, int _coins, bool _hasItem1, bool _hasItem2, Vector2D _playerPosition, Vector2D _playerRespawnPoint, std::vector<EnemyData> _enemies, std::vector<ItemData> _items)
        : currentScene(_currentScene), time(_time), lives(_lives), coins(_coins), hasItem1(_hasItem1), hasItem2(_hasItem2), playerPosition(_playerPosition), playerRespawnPoint(_playerRespawnPoint), enemies(_enemies), items(_items), exists(true) { };

    SaveData()
        : currentScene(0), time(0), lives(0), coins(0), hasItem1(0), hasItem2(0), playerPosition({0, 0}), playerRespawnPoint({0, 0}), enemies({}), items({}), exists(false) { };

    bool exists;
    int currentScene;
    int time;
    int lives;
    int coins;
    bool hasItem1;
    bool hasItem2;
    Vector2D playerPosition; 
    Vector2D playerRespawnPoint;
    std::vector<EnemyData> enemies;
    std::vector<ItemData> items;
};

class Persistence {
public:
    Persistence();

    bool SaveFileExists();
    SaveData Load();
    void Save(SaveData data);
    
    const char* saveFileName = "gamesave.xml";
};
