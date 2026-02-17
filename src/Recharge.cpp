#include "Recharge.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Recharge::Recharge() : Entity(EntityType::RECHARGE)
{
	name = "Recharge";
}

Recharge::~Recharge () {}

bool Recharge::Awake() {
	return true;
}

bool Recharge::Start() {
	if (started) return true;
	started = true;

	texture = Engine::GetInstance().textures->Load("Assets/Textures/recharge.png");
	std::unordered_map<int, std::string> aliases = { {0,"idle"}};
	anims.LoadFromTSX("Assets/Textures/recharge.tsx", aliases);
	anims.SetCurrent("idle");
	
	
	Engine::GetInstance().textures.get()->GetSize(texture, texW, texH);

	//if (pbody) Engine::GetInstance().physics->DeletePhysBody(pbody);

	CreateBody();

	return true;
}

bool Recharge::Update(float dt)
{
	if (!visible && respawnTimer.ReadMSec() > respawnMS) {
		visible = true;
		CreateBody();
	}
	//if (!active) return true;


	return true;
}

void Recharge::Draw(float dt) {
	if (!active || !visible) return;

	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	Engine::GetInstance().render->DrawTexture(texture, x - 8 , y - texH / 2, &animFrame);
}

bool Recharge::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().physics->DeletePhysBody(pbody);
	return true;
}

bool Recharge::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}

void Recharge::Respawn() {
	visible = false;
	respawnTimer.Start();
	if (pbody) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

void Recharge::CreateBody() {
	pbody = Engine::GetInstance().physics->CreateCircleSensor((int)position.getX(), (int)position.getY(), texH / 2, bodyType::STATIC, 0x0002, 0X003);
	pbody->ctype = ColliderType::RECHARGE;
	pbody->listener = this;
}

void Recharge::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		Respawn();
		break;
	}
}