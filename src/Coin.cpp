#include "Coin.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Coin::Coin() : Entity(EntityType::COIN)
{
	name = "coin";
}

Coin::~Coin() {}

bool Coin::Awake() {
	return true;
}

bool Coin::Start() {
	if (started) return true;
	started = true;

	texture = Engine::GetInstance().textures->Load("Assets/Textures/goldCoin.png");

	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);

	//if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);

	pbody->ctype = ColliderType::COIN;

	pbody->listener = this;

	return true;
}

bool Coin::Update(float dt)
{
	if (!active) return true;

	return true;
}

void Coin::Draw(float dt) {
	if (!active) return;

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

bool Coin::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	return true;
}

bool Coin::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}