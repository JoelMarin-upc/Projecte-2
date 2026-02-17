#pragma once

#include "Enemy.h"
#include "Animation.h"
#include "Timer.h"
#include "Spear.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

class AirEnemy : public Enemy
{
public:

	AirEnemy();

	~AirEnemy();

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
	void HandleAnimations();
	void CheckTimers();
	void GetPhysicsValues();
	void ApplyPhysics();
	void GetTargetDirection();

public:
	//Audio fx
	int flapFxId;
	int deathFxId;
	int attackFxId;
	float deathMS = 500.0f;

	int maxPathFindingSteps = 30;

	int x, y;

};