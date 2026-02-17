#include "Vector2D.h"
#include "Persistence.h"
#include <fstream>

Persistence::Persistence()
{
}

bool Persistence::SaveFileExists()
{
    std::ifstream file(saveFileName);
    return file.good();
}

SaveData Persistence::Load()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(saveFileName);
    if (result.status != pugi::xml_parse_status::status_ok) return SaveData();
    pugi::xml_node root = doc.child("game");

    int currentScene = root.child("currentScene").attribute("value").as_int();
    int time = root.child("time").attribute("value").as_int();
    
    float xPlayerSpawn = root.child("player").attribute("xSpawn").as_float();
    float yPlayerSpawn = root.child("player").attribute("ySpawn").as_float();
    Vector2D playerPosSpawn(xPlayerSpawn, yPlayerSpawn);

    float xPlayer = root.child("player").attribute("x").as_float();
    float yPlayer = root.child("player").attribute("y").as_float();
    Vector2D playerPos(xPlayer, yPlayer);

    int lives = root.child("player").attribute("lives").as_int();
    int coins = root.child("player").attribute("coins").as_int();
    int hasItem1 = root.child("player").attribute("hasItem1").as_bool();
    int hasItem2 = root.child("player").attribute("hasItem2").as_bool();

    std::vector<EnemyData> enemies;
    for (pugi::xml_node enemyNode = root.child("enemies").child("enemy"); enemyNode != NULL; enemyNode = enemyNode.next_sibling("enemy"))
    {
        EnemyData enemy;
        float xEnemy = enemyNode.attribute("x").as_float();
        float yEnemy = enemyNode.attribute("y").as_float();
        Vector2D enemyPos(xEnemy, yEnemy);
        enemy.position = enemyPos;
        enemy.enType = enemyNode.attribute("enType").as_int();
        enemies.push_back(enemy);
    }

    std::vector<ItemData> items;
    for (pugi::xml_node itemNode = root.child("items").child("item"); itemNode != NULL; itemNode = itemNode.next_sibling("item"))
    {
        ItemData item;
        float xItem = itemNode.attribute("x").as_float();
        float yItem = itemNode.attribute("y").as_float();
        Vector2D itemPos(xItem, yItem);
        item.position = itemPos;
        item.type = itemNode.attribute("type").as_int();
        items.push_back(item);
    }

    return SaveData(currentScene, time, lives, coins, hasItem1, hasItem2, playerPos, playerPosSpawn, enemies, items);
}


void Persistence::Save(SaveData data)
{
    pugi::xml_document doc;

    pugi::xml_node root = doc.append_child("game");
    
    root.append_child("currentScene").append_attribute("value").set_value(data.currentScene);
    root.append_child("time").append_attribute("value").set_value(data.time);

    pugi::xml_node playerNode = root.append_child("player");
    playerNode.append_attribute("xSpawn").set_value(data.playerRespawnPoint.getX());
    playerNode.append_attribute("ySpawn").set_value(data.playerRespawnPoint.getY());
    playerNode.append_attribute("x").set_value(data.playerPosition.getX());
    playerNode.append_attribute("y").set_value(data.playerPosition.getY());
    playerNode.append_attribute("lives").set_value(data.lives);
    playerNode.append_attribute("coins").set_value(data.coins);
    playerNode.append_attribute("hasItem1").set_value(data.hasItem1);
    playerNode.append_attribute("hasItem2").set_value(data.hasItem2);

    pugi::xml_node enemyList = root.append_child("enemies");
    for (EnemyData e : data.enemies) {
        pugi::xml_node enemy = enemyList.append_child("enemy");
        enemy.append_attribute("x").set_value(e.position.getX());
        enemy.append_attribute("y").set_value(e.position.getY());
        enemy.append_attribute("enType").set_value(e.enType);
    }

    pugi::xml_node itemList = root.append_child("items");
    for (ItemData i : data.items) {
        pugi::xml_node item = itemList.append_child("item");
        item.append_attribute("x").set_value(i.position.getX());
        item.append_attribute("y").set_value(i.position.getY());
        item.append_attribute("type").set_value(i.type);
    }

    doc.save_file(saveFileName);
}
