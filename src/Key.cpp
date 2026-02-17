#include "Key.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Key::Key() : Entity(EntityType::KEY)
{
	name = "key";
}

Key::~Key() {}

bool Key::Awake() {
	return true;
}

bool Key::Start() {
	if (started) return true;
	started = true;

	texture = Engine::GetInstance().textures->Load("Assets/Textures/key.png");
	
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);

	//if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);

	pbody->ctype = ColliderType::KEY;

	pbody->listener = this;

	return true;
}

bool Key::Update(float dt)
{
	if (!active) return true;
	if (!pbody) CreateBody();

	return true;
}

void Key::Draw(float dt) {
	if (!active) return;

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);

	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

bool Key::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	return true;
}

bool Key::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}

void Key::Despawn() {
	active = false;
	if (pbody) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

void Key::CreateBody() {
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);
	pbody->ctype = ColliderType::KEY;
	pbody->listener = this;
}

void Key::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		Despawn();
		break;
	}
}