#include "Boss.h"
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

Boss::Boss() : Enemy(EntityType::BOSS)
{
	enType = 2;
}

Boss::~Boss() {

}

bool Boss::Awake()
{
	return true;
}

bool Boss::Start() {
	//// load
	texturePath = "Assets/Textures/Boss.png";
	animationsPath = "Assets/Textures/Boss.tsx";
	texture = Engine::GetInstance().textures->Load(texturePath);

	std::unordered_map<int, std::string> aliases = { {0,"spawn"},{6,"idle"},{7,"despawn"},{8,"hidden"},{15,"hit"},{18,"death"}, };
	anims.LoadFromTSX(animationsPath, aliases);
	anims.SetCurrent("hidden");

	texW = 64;
	texH = 128;
	initialPosition = position;
	pivotPosition = { initialPosition.getX() + (texW / 2),initialPosition.getY() + texH };

	if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);

	//initialize audio effect
	deathFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("death").as_string());
	spawnFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("bossSpawn").as_string());
	despawnFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("bossSpawn").as_string());

	pathfinding = std::make_shared<Pathfinding>();

	return true;
}

bool Boss::Update(float dt)
{
	ZoneScoped;
	CheckTimers();
	if (!active) return true;

	if (!Engine::GetInstance().render->IsOnScreenWorldRect(position.getX(), position.getY(), (float)texW, (float)texH, 0))
		return true;

	if (!isDead) {
		CheckPlayerNear();
		if (playerNear) {
			FindPath();
			//CheckCanAttack();
			//Attack();
		}
		Move();
	}

	return true;
}

void Boss::CheckTimers()
{
	if (spawning && spawnTimer.ReadMSec() >= spawnMS) {
		spawning = false;
		anims.SetCurrent("idle");
		CreateBody();		
	}

	if (despawning && despawnTimer.ReadMSec() >= despawnMS) {
		despawning = false;
		anims.SetCurrent("hidden");
	}

	if (!canRespawn && respawnTimer.ReadMSec() >= respawnMS) {
		canRespawn = true;
	}

	if (isDead && deathTimer.ReadMSec() >= deathMS) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
		Disable();
		Engine::GetInstance().entityManager->aliveTentacles--;
		if (Engine::GetInstance().entityManager->aliveTentacles == 0) {
			Engine::GetInstance().scene->NextScene();
		}

	}
}


void Boss::CheckPlayerNear()
{
	if (!canRespawn) return;
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());
	auto enemyTile = Engine::GetInstance().map->WorldToMap(position.getX()+16, position.getY()+64);
	if (playerTile.distanceSquared(enemyTile) < playerNearThreshold && !playerNear && !despawning) {
		playerNear = true;
		Spawn();
	}
	if (playerNear && playerTile.distanceSquared(enemyTile) > playerOnScreenThreshold) {
		playerNear = false;
		anims.SetCurrent("despawn");
		Despawn();
	}

}

void Boss::AttackPlayer()
{

}


void Boss::CheckCanAttack()
{
	// check if next to player
	canAttack = false;
}

void Boss::FindPath()
{
	auto tile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());

	Vector2D* nt = nullptr;
	if (playerTile == tile) nt = new Vector2D(playerTile);
	pathfinding->ResetPath(tile);
	while (!nt) {
		nt = pathfinding->PropagateAStar(SQUARED);
		pathfinding->DrawPath();
	}
	nextTile = nt;
}

void Boss::Move() {
	
}

void Boss::Attack()
{
	if (!canAttack) return;
	if (playerNear) {

	}
}

void Boss::Die()
{
	isDead = true;
	anims.SetCurrent("death");
	Engine::GetInstance().audio->PlayFx(deathFxId);
	deathTimer = Timer();
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = nullptr;
	//pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texW / 2, bodyType::STATIC);
	//active = false;
}

void Boss::RecieveDamage()
{
	if (!canRespawn) return;
		--health;
		if (health == 0) Die();
		else {
			anims.SetCurrent("hit");
			Despawn();
		}	
}

void Boss::GetAngle()
{
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	//if (playerPos.getX() < (position.getX() + 32)) facingRight = false;
//	else facingRight = true;
	float x = playerPos.getX() + 16 - pivotPosition.getX();
	float y = - (playerPos.getY() + 16 - pivotPosition.getY());
	angle = std::atan2(x, y) * 180 / PI;
	LOG("%f", angle);
}

void Boss::CreateBody() {
	if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	float angleRad = angle * PI / 180;
	Vector2D offsetX = { ((texW / 2) * cos(angleRad)),((texW / 2) * sin(angleRad)) };
	Vector2D offsetY = { ((texH - 20) * sin(angleRad)),-((texH-20) * cos(angleRad)) };
	int points [8] = {
		-offsetX.getX(),-offsetX.getY(),
		- offsetX.getX() + offsetY.getX(),-offsetX.getY() + offsetY.getY(),
		offsetX.getX() + offsetY.getX(),offsetX.getY() + offsetY.getY(),
		offsetX.getX(),offsetX.getY()
	};

	int points2[8] = {
		-100,0,-100,-100,100,-100,100,0
	};

	pbody = Engine::GetInstance().physics->CreateChain(pivotPosition.getX()-32, pivotPosition.getY()-64, points, 8, bodyType::STATIC, 0x0002, 0X003);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
}

void Boss::Spawn() {
	Engine::GetInstance().audio->PlayFx(spawnFxId);
	spawnTimer.Start();
	spawning = true;
	GetAngle();
	anims.SetCurrent("spawn");
}

void Boss::Despawn() {
	Engine::GetInstance().audio->PlayFx(despawnFxId);
	despawnTimer.Start();
	despawning = true;
	playerNear = false;
	canRespawn = false;
	despawnTimer.Start();
	respawnTimer.Start();
	if (pbody) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

void Boss::Draw(float dt) {
	ZoneScoped;
	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	Engine::GetInstance().render->DrawTexture(texture, initialPosition.getX() - texW / 2, initialPosition.getY() - texH / 2, &animFrame, facingRight, angle, texW / 2, texH);
}

bool Boss::CleanUp()
{
	LOG("Cleanup Enemy");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::SPEAR:
		RecieveDamage();
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Boss::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
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

