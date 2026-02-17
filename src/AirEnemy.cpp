#include "AirEnemy.h"
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
#include "tracy/Tracy.hpp"

AirEnemy::AirEnemy() : Enemy(EntityType::ENEMY_AIR)
{
	enType = 0;
}

AirEnemy::~AirEnemy() {

}

bool AirEnemy::Awake()
{
	return true;
}

bool AirEnemy::Start() {
	//// load
	texturePath = "Assets/Textures/bat32.png";
	animationsPath = "Assets/Textures/bat32.tsx";
	texture = Engine::GetInstance().textures->Load(texturePath);
	std::unordered_map<int, std::string> aliases = { {0,"idle"},{4,"death"} ,{8,"attack"} };
	anims.LoadFromTSX(animationsPath, aliases);
	anims.SetCurrent("idle");
	

	texW = 32;
	texH = 32;
	if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC, 0x0002, 0X003);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	Engine::GetInstance().physics->SetGravityScale(pbody, 0);

	pathfinding = std::make_shared<Pathfinding>();

	//initialize audio effect
	flapFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("flap").as_string());
	deathFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("death").as_string());
	attackFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("batAttack").as_string());

	return true;
}

bool AirEnemy::Update(float dt)
{
	ZoneScoped;
	CheckTimers();
	if (!active) return true;

	if (!Engine::GetInstance().render->IsOnScreenWorldRect(position.getX(), position.getY(), (float)texW, (float)texH, 0))
		return true;

	GetPhysicsValues();
	CheckPlayerNear();
	if (playerNear && !isDead) {
		FindPath();
		GetTargetDirection();
		Move();
		CheckCanAttack();
		Attack();
	}
	ApplyPhysics();
	HandleAnimations();

	return true;
}

void AirEnemy::GetPhysicsValues() {
	// Read current velocity
	if (isDead) return;
	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	velocity = { 0, velocity.y };
}

void AirEnemy::CheckPlayerNear()
{
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());
	auto enemyTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	playerNear = playerTile.distanceSquared(enemyTile) < playerNearThreshold;
}

void AirEnemy::CheckCanAttack()
{
	if (attackTimer.ReadMSec() < attackCooldownMS)
	{
		canAttack = false;
		return;
	}
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());
	auto enemyTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	canAttack = playerTile.distanceSquared(enemyTile) < attackThreshold;
}

void AirEnemy::FindPath()
{
	int pathFindingSteps = maxPathFindingSteps;
	auto tile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());
	
	Vector2D* nt = nullptr;
	if (playerTile == tile) nt = new Vector2D(playerTile);
	pathfinding->ResetPath(tile);
	while (!nt && pathFindingSteps > 0)
	{
		nt = pathfinding->PropagateAStar(SQUARED);
		pathfinding->DrawPath();
		pathFindingSteps--;
	}
	nextTile = nt;
}

void AirEnemy::Move() {
	// Move left/right
	if (targetDirection.getX() < 0) {
		velocity.x = -speed;
		facingRight = false;
		if (walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(flapFxId);
			walkTimer = Timer();
		}
	}
	if (targetDirection.getX() > 0) {
		velocity.x = speed;
		facingRight = true;
		if (walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(flapFxId);
			walkTimer = Timer();
		}
	}
	// Move up/down
	if (targetDirection.getY() < 0) {
		velocity.y = -speed;
		facingRight = false;
		if (walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(flapFxId);
			walkTimer = Timer();
		}
	}
	if (targetDirection.getY() > 0) {
		velocity.y = speed;
		facingRight = true;
		if (walkTimer.ReadMSec() > walkMS) {
			Engine::GetInstance().audio->PlayFx(flapFxId);
			walkTimer = Timer();
		}
	}
}

void AirEnemy::Attack()
{
	if (!canAttack) return;
	attackSensor = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texW * attackRange, bodyType::STATIC, 0x0002, 0X003);
	attackSensor->ctype = ColliderType::ATTACK;
	attackTimer.Start();
	anims.PlayOnce("attack");
	Engine::GetInstance().audio->PlayFx(attackFxId);
}

void AirEnemy::ApplyPhysics() {
	if (isDead) return;
	if (!playerNear || isDead) velocity = { 0, 0 };
	/*velocity.y = Engine::GetInstance().physics->GetYVelocity(pbody);*/
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
}

void AirEnemy::GetTargetDirection()
{
	targetDirection = { 0, 0 };
	
	if (!nextTile) return;

	Vector2D mapPos = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());

	if (nextTile->getX() > mapPos.getX()) targetDirection.setX(1);
	if (nextTile->getX() < mapPos.getX()) targetDirection.setX(-1);
	if (nextTile->getY() > mapPos.getY()) targetDirection.setY(1);
	if (nextTile->getY() < mapPos.getY()) targetDirection.setY(-1);
	LOG("x: %f  y: %f", targetDirection.getX(), targetDirection.getY());
}

void AirEnemy::Die()
{
	isDead = true;
	anims.PlayOnce("death");
	Engine::GetInstance().audio->PlayFx(deathFxId);
	deathTimer = Timer();
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = nullptr;
	Engine::GetInstance().physics->DeletePhysBody(attackSensor);
	attackSensor = nullptr;
}

void AirEnemy::HandleAnimations()
{
	/*if (abs(velocity.x) > 0.2) {
		if (currentAnimation != "move") anims.SetCurrent("move");
		currentAnimation = "move";
	}
	else {
		if (currentAnimation != "idle") anims.SetCurrent("idle");
		currentAnimation = "idle";
	}*/
	
}

void AirEnemy::CheckTimers()
{
	if (attackSensor && attackTimer.ReadMSec() >= attackMS) {
		Engine::GetInstance().physics->DeletePhysBody(attackSensor);
		attackSensor = nullptr;
	}
	if (isDead && deathTimer.ReadMSec() >= deathMS) {
		active = false;
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
		Disable();
	}
}

void AirEnemy::Draw(float dt) {
	ZoneScoped;
	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	if (!isDead) {
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);
	}
	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame, facingRight);
}

bool AirEnemy::CleanUp()
{
	LOG("Cleanup Enemy");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void AirEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::SPEAR:
		Die();
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void AirEnemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("End Collision PLATFORM");
		break;
	case ColliderType::SPEAR:
		LOG("End Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}

