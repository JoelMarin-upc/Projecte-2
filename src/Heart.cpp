#include "Heart.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Heart::Heart() : Entity(EntityType::HEART)
{
	name = "heart";
}

Heart::~Heart() {}

bool Heart::Awake() {
	return true;
}

bool Heart::Start() {
	if (started) return true;
	started = true;

	texture = Engine::GetInstance().textures->Load("Assets/Textures/heartPick.png");
	
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);

	//if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);

	pbody->ctype = ColliderType::HEART;

	pbody->listener = this;

	return true;
}

bool Heart::Update(float dt)
{
	if (!active) return true;

	return true;
}

void Heart::Draw(float dt) {
	if (!active) return;

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

bool Heart::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	return true;
}

bool Heart::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}