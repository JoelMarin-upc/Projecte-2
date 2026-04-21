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

Item::Item(std::string id, std::string name, std::string description, std::string texturePath, EntityType type) : DynamicEntity(id, name, texturePath, type)
{
	name = "item";
	this->description = description;
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {
	return true;
}

bool Item::Update(float dt)
{
	if (!active) return true;

	return true;
}

void Item::Draw(float dt) {
	if (!active) return;

	//int x, y;
	//pbody->GetPosition(x, y);
	//position.setX((float)x);
	//position.setY((float)y);

	//Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
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
	//Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}