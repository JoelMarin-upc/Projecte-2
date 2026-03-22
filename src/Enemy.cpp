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

Enemy::Enemy(std::string id, std::string name, std::string texturePath) : AICharacter(id, name, texturePath, EntityType::ENEMY)
{
	name = "Enemy";
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	return true;
}

bool Enemy::Start() {

	LOG("ENEMY IS BEING CREATED");
	//ONCE WE CREATE DIFFERENT TYPES THIS PART WILL GO IN THE SEPERATE CLASSES
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);

	colliders[0]->etype = EntityType::ENEMY;
	pbody = colliders[0];
	pbody->listener = this;

	texW = 32;
	texH = 32;

	pathfinding = std::make_shared<Pathfinding>();
	auto scene = Engine::GetInstance().sceneManager->GetCurrentScene();
	if (map == nullptr) {
		auto scene = Engine::GetInstance().sceneManager->GetCurrentScene();
		if (scene != nullptr) {
			map = scene->GetMap();
			if (map != nullptr) {
				pathfinding->SetMap(map);
			}
		}
	}

	return true;
}

bool Enemy::Update(float dt)
{
	if (isActive) {
		UpdateState(dt);
		GetPhysicsValues();
		ApplyPhysics();
		Draw(dt);
	}

	return true;
}

void Enemy::UpdateState(float dt)
{
	Vector2D playerPos = Engine::GetInstance().sceneManager->currentScene->GetPlayerPosition();
	float dist = DistanceTo(playerPos);
	LOG("Distance: %f | ChaseDist: %f", dist, chaseDistance);

	switch (state)
	{
	case EnemyState::IDLE:
		LOG("IDLE");
		velocity = { 0, 0 };

		if (dist < chaseDistance)
			state = EnemyState::CHASING;
		break;

	case EnemyState::CHASING:
	{
		if (map == nullptr) return;
		LOG("chasing");

		Vector2D playerTile = map->WorldToMap(playerPos.getX(), playerPos.getY());
		Vector2D enemyTile = map->WorldToMap(position.getX(), position.getY());

		if (playerTile != lastPlayerTile) {
			lastPlayerTile = playerTile;

			pathfinding->ComputePath(enemyTile, playerTile);

			if (pathfinding->HasPath())
				currentTarget = pathfinding->GetNextWorld(map);
		}

		Move(currentTarget);

		float dx = currentTarget.getX() - position.getX();
		float dy = currentTarget.getY() - position.getY();

		if (sqrtf(dx * dx + dy * dy) < 8.0f)
		{
			if (pathfinding->HasPath())
				currentTarget = pathfinding->GetNextWorld(map);
		}

		LOG("HasPath: %d", pathfinding->HasPath());

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
	//velocity = { 0, velocity.y };
}

float Enemy::DistanceTo(const Vector2D& v) const
{
	float dx = v.getX() - position.getX();
	float dy = v.getY() - position.getY();
	return sqrtf(dx * dx + dy * dy);
}

void Enemy::Move(const Vector2D& target) {
	LOG("moving");
	float dx = target.getX() - position.getX();
	float dy = target.getY() - position.getY();

	float length = sqrtf(dx * dx + dy * dy);

	if (length > 0.0f)
	{
		dx /= length;
		dy /= length;
	}

	velocity.x = dx * speed;
	velocity.y = dy * speed;
}

void Enemy::ApplyPhysics() {

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
}

void Enemy::Draw(float dt) {
	if (!isActive) {
		return;
	}

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW, y - texH);

}

void Enemy::SetMap(Map* m)
{
	map = m;
	if (pathfinding) {
		pathfinding->SetMap(m);
	}
}


bool Enemy::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

