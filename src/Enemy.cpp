#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Animation.h"

Enemy::~Enemy()
{
}

bool Enemy::Awake()
{
    return true;
}

bool Enemy::Start()
{
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	if (combatTexturePath != "") combatTexture = Engine::GetInstance().textures->Load(combatTexturePath.c_str());

   /* AddCollider(ColliderType::CIRCLE, texture, 0, 0, -10, 0, 1, 1);
	colliders[0]->etype = EntityType::ENEMY;
	colliders[0]->listener = this;
	enemyBody = colliders[0];
	enemyBody->listener = this;*/

    party = new EnemyParty(std::static_pointer_cast<Enemy>(shared_from_this()));

    texW = 64;
    texH = 64;

	map = Engine::GetInstance().sceneManager->GetCurrentScene()->GetMap();
	pathfinding = std::make_shared<Pathfinding>();

	std::string walkFxPath = Engine::GetInstance().audio->GetAudioPath("enemy", "walk");
	std::string attackFxPath = Engine::GetInstance().audio->GetAudioPath("enemy", "attack");
	std::string dieFxPath = Engine::GetInstance().audio->GetAudioPath("enemy", "die");

	walkFxId = Engine::GetInstance().audio->LoadFx(walkFxPath.c_str());
	attackFxId = Engine::GetInstance().audio->LoadFx(attackFxPath.c_str());
	dieFxId = Engine::GetInstance().audio->LoadFx(dieFxPath.c_str());

	return true;
}

bool Enemy::Update(float dt)
{
    if (!active) return true;
	if (active) {
		//To make sure that the Sensor follows the pbody
		int x, y;
		colliders[0]->GetPosition(x, y);
		b2Body_SetTransform(sensorCollider->body, { PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, b2Body_GetRotation(sensorCollider->body));

		UpdateState(dt);
		GetPhysicsValues();
		ApplyPhysics();
		Draw(dt);
	}
    return true;
}

void Enemy::Draw(float dt)
{
	if (pathfinding)
		pathfinding->DrawPath();

	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	if (!animationsPath.empty()) {
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();
		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, 1, &animFrame, isFacingRight);
	}
	else {
		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
	}
}

bool Enemy::CleanUp() {
    for (const auto& collider : colliders) Engine::GetInstance().physics->DestroyBody(collider);
    return true;
}

void Enemy::HandleAnimations(b2Vec2 velocity)
{
	if (animationsPath.empty()) return;

	const float MOVE_THRESHOLD = 0.1f;
	bool isMoving = (std::abs(velocity.x) > MOVE_THRESHOLD || std::abs(velocity.y) > MOVE_THRESHOLD);

	if (isMoving) {
		if (std::abs(velocity.x) > std::abs(velocity.y)) {
			facing = "right";
			isFacingRight = (velocity.x > 0);
		}
		else {
			facing = (velocity.y > 0) ? "down" : "up";
		}

		std::string animName = "move_" + facing;
		if (currentAnimation != animName) {
			anims.SetCurrent(animName);
			currentAnimation = animName;
		}
	}
	else {
		if (currentAnimation != "idle") {
			anims.SetCurrent("idle");
			currentAnimation = "idle";
		}
	}
}

void Enemy::LoadAnimations()
{
	if (animationsPath.empty()) return;
	std::unordered_map<int, std::string> aliases = {
		{0, "idle"}, {12, "move_down"}, {16, "move_up"}, {20, "move_right"}
	};

	anims.LoadFromTSX(animationsPath.c_str(), aliases);
	anims.SetCurrent("idle");
	currentAnimation = "idle";
}

void Enemy::CreateColliders()
{
	if (animationsPath.empty()) {
		AddCollider(ColliderType::CIRCLE, texture, 0, 0, -10, 0, 1, 1);
		enemyBody = colliders[0];
		enemyBody->listener = this;
		enemyBody->etype = EntityType::ENEMY;

		b2Body_SetFixedRotation(enemyBody->body, true);

		b2MassData massData;
		massData.mass = 1000.0f;
		massData.center = { 0.0f, 0.0f };
		massData.rotationalInertia = 0.0f;
		b2Body_SetMassData(enemyBody->body, massData);

		AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 20, 20, 1, 1);
		sensorCollider = colliders[1];
		sensorCollider->listener = this;
		sensorCollider->etype = EntityType::ENEMY;

		return;
	}

	int sheetW = texture->w;
	float wOffset = -sheetW + texW; 

	AddCollider(ColliderType::CIRCLE, texture, 0, 0, wOffset - 50, 0, 1, 1);
	enemyBody = colliders[0];
	enemyBody->listener = this;
	enemyBody->etype = EntityType::ENEMY;

	b2Body_SetFixedRotation(enemyBody->body, true);

	b2MassData massData;
	massData.mass = 1000.0f;
	massData.center = { 0.0f, 0.0f };
	massData.rotationalInertia = 0.0f;
	b2Body_SetMassData(enemyBody->body, massData);

	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, wOffset - 30, 20, 1, 1);
	sensorCollider = colliders[1];
	sensorCollider->listener = this;
	sensorCollider->etype = EntityType::ENEMY;
}

void Enemy::PerformPathfinding()
{
	Vector2D pos = GetPosition();
	Vector2D tilePos = map->WorldToMap((int)pos.getX(), (int)pos.getY() + 8);
	pathfinding->ResetPath(tilePos);

	int ctr = 0;

	while (pathfinding->pathTiles.empty() && ctr < 100) {
		ctr++;
		pathfinding->PropagateAStar(SQUARED);
	}

	if (!pathfinding->pathTiles.empty()) {
		pathfinding->pathTiles.reverse();
	}

	// Reset pathfinding with R key
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		//Get the position of the enemy
		Vector2D pos = GetPosition();
		//Convert to tile coordinates
		Vector2D tilePos = map->WorldToMap((int)pos.getX(), (int)pos.getY() + 1);
		//Reset pathfinding
		pathfinding->ResetPath(tilePos);
	}

	// L13: TODO 3:	Add the key inputs to propagate the A* algorithm with different heuristics (Manhattan, Euclidean, Squared)
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		pathfinding->PropagateAStar(MANHATTAN);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateAStar(MANHATTAN);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		pathfinding->PropagateAStar(EUCLIDEAN);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateAStar(EUCLIDEAN);
	}

}

void Enemy::UpdateState(float dt)
{
	Vector2D playerPos = Engine::GetInstance().sceneManager->currentScene->GetPlayerPosition();
	float dist = DistanceTo(playerPos);

	switch (state)
	{
	case EnemyState::IDLE:
		//LOG("IDLE");
		velocity = { 0, 0 };

		if (dist < chaseDistance)
			state = EnemyState::CHASING;
		break;

	case EnemyState::CHASING:
	{

		Vector2D playerTile = map->WorldToMap(playerPos.getX(), playerPos.getY());
		Vector2D enemyTile = map->WorldToMap(position.getX(), position.getY());
		float pathTimer = 0.0f;
		float pathInterval = 0.3f;

		if (playerTile != lastPlayerTile && (pathTimer += dt) >= pathInterval) {
			lastPlayerTile = playerTile;
			PerformPathfinding();
			pathTimer = 0.0f;

		}

		Move(currentTarget);

		if (DistanceTo(playerPos) > chaseDistance)
			state = EnemyState::IDLE;
		break;
	}
	}
}

void Enemy::GetPhysicsValues() {
	// Read current velocity
	if (state != EnemyState::CHASING)
	{
		velocity = Engine::GetInstance().physics->GetLinearVelocity(colliders[0]);
	}
}

float Enemy::DistanceTo(const Vector2D& v) const
{
	float dx = v.getX() - position.getX();
	float dy = v.getY() - position.getY();
	return sqrtf(dx * dx + dy * dy);
}

void Enemy::Move(const Vector2D& target) {
	//LOG("moving");
	Vector2D currentPos = GetPosition();
	auto& tiles = pathfinding->pathTiles;

	if (tiles.size() < 2) {
		velocity = { 0, 0 };
		return;
	}

	auto it = tiles.begin();
	std::advance(it, (tiles.size() > 2) ? 2 : 1);

	Vector2D nextTile = *it;

	Vector2D nextWorldPos = map->MapToWorld((int)nextTile.getX(), (int)nextTile.getY());
	int tileWidth = map->GetTileWidth();
	int tileHeight = map->GetTileHeight();

	nextWorldPos.setX(nextWorldPos.getX() + tileWidth / 2.0f);
	nextWorldPos.setY(nextWorldPos.getY() + tileHeight / 2.0f);

	float distanceX = nextWorldPos.getX() - currentPos.getX();
	float distanceY = nextWorldPos.getY() - currentPos.getY();

	float deadZone = 1.0f;

	Vector2D dir = nextWorldPos - currentPos;

	float length = sqrt(dir.getX() * dir.getX() + dir.getY() * dir.getY());

	if (length > deadZone) {
		dir.setX(dir.getX() / length);
		dir.setY(dir.getY() / length);

		velocity.x = dir.getX() * speed;
		velocity.y = dir.getY() * speed;
	}
	else {
		pathfinding->pathTiles.pop_front();
		velocity = { 0, 0 };
	}

	if ((velocity.x != 0 || velocity.y != 0) && walkTimer.ReadMSec() > walkMS) {
		Engine::GetInstance().audio->PlayFx(walkFxId);
		walkTimer = Timer();
	}
}

void Enemy::ApplyPhysics() {

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
	HandleAnimations(velocity);
}

Vector2D Enemy::GetPosition() {
	int x, y;
	enemyBody->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x - texW / 2, (float)y - texH / 2);
}



