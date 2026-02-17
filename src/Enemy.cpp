#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"


Enemy::Enemy(EntityType type) : Entity(type)
{
	name = "Enemy";
	
}

Enemy::~Enemy() {
	
}

void Enemy::ToggleDebug() {
	if (pathfinding) pathfinding->debug = !pathfinding->debug;
}