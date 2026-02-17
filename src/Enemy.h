#pragma once

#include "Pathfinding.h"
#include "Entity.h"
#include "Animation.h"
#include "Timer.h"
#include "Vector2D.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include <list>

class Enemy : public Entity
{
public:
	Enemy(EntityType type);
	virtual ~Enemy();
	void ToggleDebug();

private:
	virtual void CheckPlayerNear() {}
	virtual void CheckCanAttack() {}
	virtual void FindPath() {}
	virtual void GetTargetDirection() {}
	virtual void Move() {}
	virtual void Attack() {}
	virtual void Die() {}
	virtual void CheckTimers() {}
	virtual void HandleAnimations() {}
	virtual void Draw(float dt) {}
	virtual void GetPhysicsValues() {}
	virtual void ApplyPhysics() {}

public:

	const char* texturePath;
	const char* animationsPath;

	float speed = 1.0f;
	SDL_Texture* texture = NULL;

	bool facingRight = true;
	bool canAttack = false;
	bool playerNear = false;
	bool playerOnScreen = false;
	bool jumpToPlayer = false;
	float playerNearThreshold = 60;
	float playerOnScreenThreshold = 2750;
	float jumpToPlayerThreshhold = 15;
	float attackThreshold = 7;
	float attackCooldownMS = 2000;
	float attackMS = 500;
	float attackRange = 1.3f;
	PhysBody* attackSensor = nullptr;
	Timer attackTimer;
	PhysBody* pbody = nullptr;
	bool isAttacking = false;
	b2Vec2 playerPos;
	Vector2D* nextTile;
	Vector2D targetDirection;
	bool isDead = false;
	Timer deathTimer;
	float deathMS = 600.0f;
	Timer walkTimer;
	float walkMS = 400.0f;
	int texW, texH;
	std::shared_ptr<Pathfinding> pathfinding;
	int enType;
	float posSensibility = 10.0f;

protected:
	b2Vec2 velocity;
	AnimationSet anims;
	std::string currentAnimation = "";

};