#include "GroundEnemy.h"
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

GroundEnemy::GroundEnemy() : Enemy(EntityType::ENEMY_GROUND)
{
	enType = 1;
}

GroundEnemy::~GroundEnemy() {

}

bool GroundEnemy::Awake()
{
	return true;
}

bool GroundEnemy::Start() {
	//// load
	texturePath = "Assets/Textures/Multi_Slime_Spritesheet.png";
	animationsPath = "Assets/Textures/Multi_Slime_Spritesheet.tsx";
	texture = Engine::GetInstance().textures->Load(texturePath);
	std::unordered_map<int, std::string> aliases = { {0,"idle"},{15,"death"},{30,"charge"},{33,"jump"} };
	anims.LoadFromTSX(animationsPath, aliases);
	anims.SetCurrent("idle");

	texW = 28;
	texH = 28;
	if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC, 0x0002, 0X003);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	//initialize audio effect
	jumpFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("slimeJump").as_string());
	walkFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("slimeWalk").as_string());
	deathFxId = Engine::GetInstance().audio->LoadFx(Engine::GetInstance().scene->configParameters.child("audios").attribute("death").as_string());

	pathfinding = std::make_shared<Pathfinding>();

	return true;
}

bool GroundEnemy::Update(float dt)
{
	ZoneScoped;
	CheckTimers();
	if (!active) return true;

	if (!Engine::GetInstance().render->IsOnScreenWorldRect(position.getX(), position.getY(), (float)texW, (float)texH, 0))
		return true;

	if (!isDead) {
		GetPhysicsValues();
		CheckGround();
		CheckPlayerNear();
		if (playerNear) {
			FindPath();
			GetTargetDirection();
			DashToPlayer();
			//CheckCanAttack();
			//Attack();
		}
		else if (playerOnScreen) Patroll();
		Move();
		ApplyPhysics();
	}

	return true;
}

void GroundEnemy::CheckTimers()
{
	if (isDead && deathTimer.ReadMSec() >= deathMS) {
		active = false;
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
		Disable();
	}
	if (isDead) return;
	if (isChargingDash && DashChargeTimer.ReadMSec() >= DashChargeMS) {
		isChargingDash = false;
		DashCDTimer.Start();
		shouldDash = true;
	}

	if (isChargingDash && (DashChargeTimer.ReadMSec() >= (DashChargeMS - 100)) && (DashChargeTimer.ReadMSec() <= (DashChargeMS - 50))) {
		posToDash = Engine::GetInstance().scene->GetPlayerPosition();
	}

	if (dashStart && dashStartTimer.ReadMSec() >= dashStartMS) {
		dashStart = false;
	}

	
}

void GroundEnemy::CheckGround()
{
	float verticalVel = std::abs(velocity.y);
	if (verticalVel < 0.3)
	{
		int x, y;
		pbody->GetPosition(x, y);
		b2Vec2 feetPos{ x, y + 20 + texH / 2 };
		float _;
		int dist = pbody->RayCast(x, y, feetPos.x, feetPos.y, _, _);
		int distLeft = pbody->RayCast(x, y, feetPos.x - texW / 2, feetPos.y, _, _);
		int distRight = pbody->RayCast(x, y, feetPos.x + texW / 2, feetPos.y, _, _);
		if (dist != -1 || distLeft != -1 || distRight != -1) {
			isJumping = false;
		}
		if (dashStart) return;
		distLeft = pbody->RayCast(x, y, feetPos.x - texW / 2 - 15, feetPos.y-10, _, _);
		distRight = pbody->RayCast(x, y, feetPos.x + texW / 2 + 15, feetPos.y-10, _, _);
		if (distLeft == -1) canLeft = false;
		else canLeft = true;
		if (distRight == -1) canRight = false;
		else canRight = true;
	}
	else {
		isJumping = true;
	}
}

void GroundEnemy::GetPhysicsValues() {
	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	velocity = { 0, velocity.y };
}

void GroundEnemy::CheckPlayerNear()
{
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());
	auto enemyTile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	if (playerTile.distanceSquared(enemyTile) < playerNearThreshold) {
		playerNear = true;
	}
	else {
		if (playerTile.distanceSquared(enemyTile) < playerOnScreenThreshold) {
			if (!playerOnScreenThreshold) patrollTimer.Start();
			playerOnScreen = true;
		}
		else playerOnScreen = false;
		playerNear = false;
	}
	if (playerTile.distanceSquared(enemyTile) < jumpToPlayerThreshhold && !isJumping && !isChargingDash && targetDirection.getY() == -1 && DashCDTimer.ReadMSec() > DashCD) {
		isChargingDash = true;
		anims.PlayOnce("charge");
		DashChargeTimer.Start();
	}
}

void GroundEnemy::DashToPlayer()
{
	if (!shouldDash) return;
	shouldDash = false;
	dashStart = true;
	dashStartTimer.Start();
	canRight = true;
	canLeft = true;
	float x = posToDash.getX() - position.getX();
	float y = posToDash.getY() - position.getY();
	float aux = sqrt(pow(x, 2) + pow(y, 2));
	x = x / aux;
	y = y / aux;
	y = std::abs(std::sin(PI / 9));
	x = std::abs(std::cos(PI / 9));
	Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, x * dashForce, y * dashForce, true);
	anims.PlayOnce("jump");
	Engine::GetInstance().audio->PlayFx(jumpFxId);
}


void GroundEnemy::CheckCanAttack()
{
	// check if next to player
	canAttack = false;
}

void GroundEnemy::FindPath()
{
	int pathFindingSteps = maxPathFindingSteps;
	auto tile = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());
	auto playerPos = Engine::GetInstance().scene->GetPlayerPositionCenter();
	auto playerTile = Engine::GetInstance().map->WorldToMap(playerPos.getX(), playerPos.getY());

	Vector2D* nt = nullptr;
	if (playerTile == tile) nt = new Vector2D(playerTile);
	pathfinding->ResetPath(tile);
	while (!nt && pathFindingSteps > 0) {
		nt = pathfinding->PropagateAStar(SQUARED);
		pathfinding->DrawPath();
		pathFindingSteps--;
	}
	nextTile = nt;
}

void GroundEnemy::Move() {
	// Move left/right
	if (!isChargingDash) {
		if (canLeft && targetDirection.getX() < 0) {
			velocity.x = -speed;
			facingRight = false;
			if (walkTimer.ReadMSec() > walkMS) {
				Engine::GetInstance().audio->PlayFx(walkFxId);
				walkTimer = Timer();
			}
		}
		if (canRight && targetDirection.getX() > 0) {
			velocity.x = speed;
			facingRight = true;
			if (walkTimer.ReadMSec() > walkMS) {
				Engine::GetInstance().audio->PlayFx(walkFxId);
				walkTimer = Timer();
			}
		}
	}
}

void GroundEnemy::Attack()
{
	if (!canAttack) return;
	if (playerNear) {

	}
}

void GroundEnemy::Jump() {
	if (shouldJump && !isJumping) {
		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -dashForce, true);
		anims.PlayOnce("jump");
		isJumping = true;
		Engine::GetInstance().audio->PlayFx(jumpFxId);
	}
}

void GroundEnemy::ApplyPhysics() {
	if (isJumping == true) {
		velocity.y = Engine::GetInstance().physics->GetYVelocity(pbody);
		velocity.x = Engine::GetInstance().physics->GetXVelocity(pbody);
	}
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
}
	
	

void GroundEnemy::GetTargetDirection()
{
	targetDirection = { 0, 0 };
	
	if (!nextTile) return;
		
	Vector2D mapPos = Engine::GetInstance().map->WorldToMap(position.getX(), position.getY());

	if (nextTile->getX() > mapPos.getX()) targetDirection.setX(1);
	if (nextTile->getX() < mapPos.getX()) targetDirection.setX(-1);
	if (nextTile->getY() > mapPos.getY()) targetDirection.setY(1);
	if (nextTile->getY() < mapPos.getY()) targetDirection.setY(-1);
}

void GroundEnemy::Patroll() {
	if (patrollTimer.ReadMSec() < patrollMS) {
		targetDirection.setX(0);
	}
	else {
		if (targetDirection.getX() == 0) {
			if (rand()%2) targetDirection.setX(-1);
			else targetDirection.setX(1);
		}
		if (Engine::GetInstance().physics->GetXVelocity(pbody) == 0) {
			if (targetDirection.getX() == 1) targetDirection.setX(-1);
			else targetDirection.setX(1);
		}
	}
}

void GroundEnemy::Die()
{
	isDead = true;
	anims.PlayOnce("death");
	Engine::GetInstance().audio->PlayFx(deathFxId);
	deathTimer = Timer();
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = nullptr;
	//pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texW / 2, bodyType::STATIC);
	//active = false;
}

void GroundEnemy::Draw(float dt) {
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

bool GroundEnemy::CleanUp()
{
	LOG("Cleanup Enemy");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void GroundEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::SPEAR:
		Die();
		break;
	case ColliderType::DEATHZONE:
		Die();
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void GroundEnemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
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

