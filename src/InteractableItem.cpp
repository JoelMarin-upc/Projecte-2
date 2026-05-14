#include "InteractableItem.h"
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

InteractableItem::InteractableItem(std::string id, std::string name, std::string description, std::string texturePath, ItemInteractionType type, bool canStack, std::string _toggledTexturePath) : Item(id, name, description, texturePath, EntityType::INTERACTABLE_ITEM)
{
	toggledTexturePath = _toggledTexturePath;
	itemInteractionType = type;
	this->canStack = canStack;
}

InteractableItem::~InteractableItem() {}

bool InteractableItem::Awake() {
	return true;
}

bool InteractableItem::Start() {
	//texturePath = "Assets/Textures/door_open.png";
	pickupIconPath = "Assets/Textures/item.png";
	if (toggledTexturePath != "") toggledTexture = Engine::GetInstance().textures->Load(toggledTexturePath.c_str());
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	icon = texture;
	pickupIcon = Engine::GetInstance().textures->Load(pickupIconPath);
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, -20, -20, 1, 1);
	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 50, 50, 1, 1);
	texW = 30;
	texH = 30;

	pbody = colliders[0];
	colliders[0]->etype = EntityType::INTERACTABLE_ITEM;
	pbody->listener = this;
	//pbody->listener->type = EntityType::INTERACTABLE_ITEM;

	sensorCollider = colliders[1];
	colliders[1]->etype = EntityType::INTERACTABLE_ITEM;
	sensorCollider->listener = this;
	//sensorCollider->listener->type = EntityType::INTERACTABLE_ITEM;

	return true;
}

bool InteractableItem::Update(float dt) {
	if (!active) {
		return true;
	}
	if (isPlayerInRange && !isToggled) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			Interact();
		}
	}
	//Draw(dt);
	return true;
}

void InteractableItem::Draw(float dt) {
	if (!active) {
		return;
	}

	int x, y;
	pbody->GetPosition(x, y);

	SDL_Texture* tex = texture;
	if (isToggled) tex = toggledTexture;
	Engine::GetInstance().render->DrawTexture(tex, x - texW, y - texH);

	//To draw pick up icon next to the item sprite, adjustable later
	if (isPlayerInRange) {
		Engine::GetInstance().render->DrawTexture(pickupIcon, x - pickupIconW, y - pickupIconH);
	}
}

bool InteractableItem::CleanUp() {
	LOG("Cleanup Interactable Item");
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().textures->UnLoad(pickupIcon);
	if (sensorCollider != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(sensorCollider);
		sensorCollider = nullptr;
	}
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}

void InteractableItem::OnCollision(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER: 
		isPlayerInRange = true;
		break;
	}
}

void InteractableItem::OnCollisionEnd(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER:
		isPlayerInRange = false;
		break;
	default:
		break;
	}
}

void InteractableItem::Interact()
{
	switch (itemInteractionType)
	{
	case ItemInteractionType::PICKUP:   
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		Pickup();  
		break;
	case ItemInteractionType::TOGGLE:   
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		Toggle();   
		break;
	case ItemInteractionType::DIALOGUE:
		Engine::GetInstance().sceneManager->currentScene->StartDialog(name);
		break;
	default:
		break;
	}
}

void InteractableItem::Pickup()
{
	for (const auto& e : Engine::GetInstance().sceneManager->currentScene->entityManager->entities) {
		Player* player = dynamic_cast<Player*>(e.get());
		if (player) {
			if (player->inventory->AddItem(std::make_shared<InteractableItem>(*this))) {
				isPicked = true;
				isPlayerInRange = false;
				active = false;

				if (pbody) {
					Engine::GetInstance().physics->DeletePhysBody(pbody);
					pbody = nullptr;
				}
				if (sensorCollider) {
					Engine::GetInstance().physics->DeletePhysBody(sensorCollider);
					sensorCollider = nullptr;
				}
				player->inventory->PrintContents();
				LOG("'%s' picked up", name.c_str());
				return;
			}
				
			else {
				LOG("Failed to pick up item");
			}
			return;
		}
	}	
}

void InteractableItem::Toggle()
{
	isToggled = !isToggled;
}