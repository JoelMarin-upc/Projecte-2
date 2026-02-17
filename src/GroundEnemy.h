#pragma once

#include "Enemy.h"
#include "Animation.h"
#include "Timer.h"
#include "Spear.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"

class GroundEnemy : public Enemy
{
public:

	GroundEnemy();

	~GroundEnemy();

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
	void GetPhysicsValues();
	void ApplyPhysics();
	void GetTargetDirection();
	void Patroll();

	void CheckGround();
	void DashToPlayer();
	void Jump();

public:

	float jumpForce = 0.6f;
	float speed = 1.8f;
	bool isJumping = false;
	bool isChargingDash = false;
	bool shouldJump = false;
	bool shouldDash = false;
	bool dashStart = false;

	bool canRight = true;
	bool canLeft = true;

	//Audio fx
	int jumpFxId;
	int walkFxId;
	int deathFxId;

	Timer DashChargeTimer;
	float DashChargeMS = 500.0f;
	Timer DashCDTimer;
	float DashCD = 1000.f;
	Vector2D posToDash;
	float dashForce = 1.5f;
	Timer walkTimer;
	float walkMS = 400.0f;
	Timer patrollTimer;
	float patrollMS = 1500.0f;
	Timer dashStartTimer;
	float dashStartMS = 300.0f;

	int maxPathFindingSteps = 30;

	int x, y;
};