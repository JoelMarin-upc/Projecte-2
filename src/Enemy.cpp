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

    AddCollider(ColliderType::CIRCLE, texture, 0, 0, -10, 0, 1, 1);
	colliders[0]->etype = EntityType::ENEMY;
	colliders[0]->listener = this;
	enemyBody = colliders[0];
	enemyBody->listener = this;

    party = new EnemyParty(std::static_pointer_cast<Enemy>(shared_from_this()));

    texW = 30;
    texH = 30;

	map = Engine::GetInstance().sceneManager->GetCurrentScene()->GetMap();
	pathfinding = std::make_shared<Pathfinding>();

	return true;
}

bool Enemy::Update(float dt)
{
    if (!active) return true;
	if (active) {
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
	{
		pathfinding->DrawPath();
	}
    int x, y;
    colliders[0]->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);
    Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2/*, &animFrame, facingRight*/);

    //DrawHealthBar(texture);
}

bool Enemy::CleanUp() {
    for (const auto& collider : colliders) Engine::GetInstance().physics->DestroyBody(collider);
    return true;
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

		if (playerTile != lastPlayerTile) {
			lastPlayerTile = playerTile;
			PerformPathfinding();

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
	std::vector<Vector2D> tiles(pathfinding->pathTiles.begin(), pathfinding->pathTiles.end());
	int targetIndex = 1;

	//Checks a bit ahead in the tiles vector
	if (tiles.size() - 1 > 2) {
		targetIndex = 2;
	}

	Vector2D nextTile = tiles[targetIndex];

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
}

void Enemy::ApplyPhysics() {

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
}

Vector2D Enemy::GetPosition() {
	int x, y;
	enemyBody->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x - texW / 2, (float)y - texH / 2);
}



