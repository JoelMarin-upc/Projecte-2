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

Enemy::Enemy(std::string id) : AICharacter(id, EntityType::ENEMY_GROUND)
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
	texture = Engine::GetInstance().textures->Load("Assets/Textures/goldCoin.png");
	texturePath = "Assets/Textures/goldCoin.png";
	texture = Engine::GetInstance().textures->Load(texturePath);
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);

	colliders[0]->etype = EntityType::ENEMY_GROUND;
	pbody = colliders[0];
	pbody->listener = this;

	texW = 32;
	texH = 32;

	//pathfinding = std::make_shared<Pathfinding>();
	return true;
}

bool Enemy::Update(float dt)
{
	if (isActive) {
		GetPhysicsValues();
		Move();
		ApplyPhysics();
		//HandleAnimations();
		Draw(dt);
	}

	return true;
}

void Enemy::GetPhysicsValues() {
	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(colliders[0]);
	velocity = { 0, velocity.y };
}

void Enemy::Move() {
	velocity.y = speed;
}

void Enemy::ApplyPhysics() {

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
}

void Enemy::Draw(float dt) {
	if (!active) {
		return;
	}

	int x, y;
	pbody->GetPosition(x, y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW, y - texH);

}


bool Enemy::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

