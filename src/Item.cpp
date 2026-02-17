#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {
	if (started) return true;
	started = true;

	texture = Engine::GetInstance().textures->Load("Assets/Textures/item.png");
	
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);

	//if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);

	pbody->ctype = ColliderType::ITEM;

	pbody->listener = this;

	return true;
}

bool Item::Update(float dt)
{
	if (!active) return true;

	return true;
}

void Item::Draw(float dt) {
	if (!active) return;

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

bool Item::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	return true;
}

bool Item::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}