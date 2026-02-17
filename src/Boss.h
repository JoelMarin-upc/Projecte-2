#pragma once

#include "Enemy.h"
#include "Animation.h"
#include "Timer.h"
#include "Spear.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"

class Boss : public Enemy
{
public:

	Boss();

	~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void Draw(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

private:
	void CheckPlayerNear();
	void CheckCanAttack();
	void FindPath();
	void Move();
	void Attack();
	void Die();
	void CheckTimers();
	void GetAngle();
	void CreateBody();
	void Spawn();
	void Despawn();

	void AttackPlayer();
	void RecieveDamage();

public:

	//Audio fx
	int deathFxId;
	int spawnFxId;
	int despawnFxId;

	Vector2D initialPosition;
	Vector2D pivotPosition;
	int health = 2;
	bool spawning = false;
	Timer spawnTimer;
	float spawnMS = 300.0f;

	bool despawning = false;
	Timer despawnTimer;
	float despawnMS = 300.0f;

	bool canRespawn = true;
	Timer respawnTimer;
	float respawnMS = 1500.0f;

	bool playerNear = false;
	float angle = 0;

	float playerNearThreshold = 50;
	float playerOnScreenThreshold = 200;
};