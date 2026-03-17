#include "NPC.h"
#include "Engine.h"
#include "SceneManager.h"

NPC::NPC(std::string id, NPCInteractionType type) : AICharacter(id, EntityType::NPC)
{
	npcID = id;
	name = "interactableNPC";
	npcInteractionType = type;
}

NPC::~NPC() {}

bool NPC::Awake() {
	return true;
}

bool NPC::Start()
{
	texturePath = "Assets/Textures/heart.png";
	texture = Engine::GetInstance().textures->Load(texturePath);
	position.setX(200);
	position.setY(200);
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, 0, 0, 1, 1);
	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 10, 10, 1, 1);

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
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			
		}
	}
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
	switch (npcInteractionType)
	{
	case NPCInteractionType::RECRUIT:
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		break;
	case NPCInteractionType::SHOP:
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		break;
	case NPCInteractionType::DIALOGUE:
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		break;
	default:
		break;
	}
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