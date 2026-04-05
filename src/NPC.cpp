#include "NPC.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Log.h"

NPC::~NPC() {}

bool NPC::Awake() {
	return true;
}

bool NPC::Start()
{
	texturePath = "Assets/Textures/heart.png";
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());

	AddCollider(ColliderType::CIRCLE, texture, 0, 0, -10, 0, 1, 1);
	pbody = colliders[0];
	pbody->listener = this;
	pbody->etype = EntityType::NPC;

	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 20, 20, 1, 1);
	sensorCollider = colliders[1];
	sensorCollider->listener = this;
	sensorCollider->etype = EntityType::NPC;

	texW = 32;
	texH = 32;
    return true;
}

bool NPC::Update(float dt)
{
	if (!active) {
		return true;
	}
	if (isPlayerInRange) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			Interact();
		}
	}
	Draw(dt);
    return true;
}

void NPC::Draw(float dt)
{
	int x, y;
	colliders[0]->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - 6 - texH / 2/*, &animFrame, facingRight*/);
}

void NPC::Interact()
{
	Engine::GetInstance().sceneManager->currentScene->StartDialog(npcID);
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
	if (isRecruitConditionFulfilled) {
		LOG("%s joined the party!", name.c_str());
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