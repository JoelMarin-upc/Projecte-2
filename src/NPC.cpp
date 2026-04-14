#include "NPC.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Log.h"
#include "Party.h"
//#include <cmath>

NPC::~NPC() {}

bool NPC::Awake() {
	return true;
}

bool NPC::Start()
{
	//texturePath = "Assets/Textures/heart.png";
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());


	AddCollider(ColliderType::CIRCLE, texture, 0, 0, -10, 0, 1, 1);
	pbody = colliders[0];
	pbody->listener = this;
	pbody->etype = EntityType::NPC;

	b2Body_SetFixedRotation(pbody->body, true);

	b2MassData massData;
	massData.mass = 1000.0f;
	massData.center = { 0.0f, 0.0f };
	massData.rotationalInertia = 0.0f;
	b2Body_SetMassData(pbody->body, massData);

	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 20, 20, 1, 1);
	sensorCollider = colliders[1];
	sensorCollider->listener = this;
	sensorCollider->etype = EntityType::NPC;

	texW = 30;
	texH = 30;

	party = nullptr;

    return true;
}

bool NPC::Update(float dt)
{
	if (!active) {
		return true;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		if (party != nullptr) {
			active = false;
			isDead = true;
			Engine::GetInstance().sceneManager->deadNPCs.push_back(id);
			party->RemoveMember(id);
			Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
			return true;
		}
	}

	//To make sure that the Sensor follows the pbody
	int x, y;
	colliders[0]->GetPosition(x, y);
	b2Body_SetTransform(sensorCollider->body, { PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, b2Body_GetRotation(sensorCollider->body));

	if (isPlayerInRange) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			Interact();
		}
	}
	Move();
	Draw(dt);
    return true;
}

void NPC::Draw(float dt)
{
	if (!active) {
		return;
	}

	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2/*, &animFrame, facingRight*/);

	if (party) DrawHealthBar(texture);
}

bool NPC::CleanUp() {
	for (const auto& collider : colliders) Engine::GetInstance().physics->DestroyBody(collider);
	return true;
}

void NPC::Move()
{
	if (!party || !party->player) return;
	if (colliders.empty() || !colliders[0]) return;

	const float speed = 2.5f;
	const float stopDistance = 1.f;

	int x, y;
	colliders[0]->GetPosition(x, y);
	b2Vec2 npcPos = b2Vec2();
	npcPos.x = PIXEL_TO_METERS(x);
	npcPos.y = PIXEL_TO_METERS(y);

	float playerX = PIXEL_TO_METERS(party->player->position.getX());
	float playerY = PIXEL_TO_METERS(party->player->position.getY());
	b2Vec2 playerPos = b2Vec2();
	playerPos.x = playerX;
	playerPos.y = playerY;

	b2Vec2 dir = playerPos - npcPos;

	float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

	if (distance < stopDistance || distance == 0.0f) {
		Engine::GetInstance().physics->SetLinearVelocity(colliders[0], b2Vec2_zero);
		return;
	}

	dir.x /= distance;
	dir.y /= distance;

	b2Vec2 velocity = speed * dir;

	Engine::GetInstance().physics->SetLinearVelocity(colliders[0], velocity);
	int xFinal, yFinal;
	pbody->GetPosition(xFinal, yFinal);
	sensorCollider->SetPosition(xFinal, yFinal);
}

void NPC::Interact()
{
	Engine::GetInstance().sceneManager->currentScene->StartDialog(id);
}

void NPC::OnDialogEnd()
{
	switch (npcInteractionType)
	{
	case NPCInteractionType::RECRUIT:
		Recruit();
		break;
	case NPCInteractionType::SHOP:
		OpenShop();
		break;
	case NPCInteractionType::DIALOGUE:
		break;
	default:
		break;
	}
}

void NPC::Recruit()
{
	isRecruitConditionFulfilled = true; // for testing
	if (isRecruitConditionFulfilled && !party) {
		LOG("%s joined the party!", name.c_str());
		Engine::GetInstance().sceneManager->currentScene->player->AddPartyMember(std::dynamic_pointer_cast<NPC>(shared_from_this()), true);
	}
	else {

	}
}

void NPC::OpenShop()
{
	LOG("Opening shop for '%s'", name.c_str());
}

void NPC::OnCollision(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER:
		isPlayerInRange = true;
		break;
	}
}

void NPC::OnCollisionEnd(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER:
		isPlayerInRange = false;
		break;
	default:
		break;
	}
}