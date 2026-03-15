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

InteractableItem::InteractableItem(InteractionType type)
{
	name = "interactableItem";
}

InteractableItem::~InteractableItem() {}

bool InteractableItem::Awake() {
	return true;
}

bool InteractableItem::Start() {
	texturePath = "Assets/Textures/heart.png";
	pickupIconPath = "Assets/Textures/item.png";
	texture = Engine::GetInstance().textures->Load(texturePath);
	pickupIcon = Engine::GetInstance().textures->Load(pickupIconPath);
	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 0, 0, 0, 0);
	texW = 30;
	texH = 30;
	sensorCollider->listener = this;
	sensorCollider->etype = EntityType::ITEM;

	return true;
}

bool InteractableItem::Update(float dt) {
	if (!active) {
		return true;
	}
	if (isPlayerInRange) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			Interact();
		}
	}
	return true;
}

void InteractableItem::Draw(float dt) {
	if (!active) {
		return;
	}

	int x, y;
	sensorCollider->GetPosition(x, y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);

	//To draw pick up icon next to the item sprite, adjustable later
	if (isPlayerInRange) {
		Engine::GetInstance().render->DrawTexture(pickupIcon, x - pickupIconW, y - pickupIconH);
	}
}

bool InteractableItem::CleanUp() {
	LOG("Cleanup Interactable Item");
	if (sensorCollider != nullptr) {
		Engine::GetInstance().textures->UnLoad(texture);
		Engine::GetInstance().textures->UnLoad(pickupIcon);
		Engine::GetInstance().physics->DeletePhysBody(sensorCollider);
		sensorCollider = nullptr;
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
	switch (interactionType)
	{
	case InteractionType::PICKUP:   
		Pickup();  
		break;
	case InteractionType::TOGGLE:   
		Toggle();   
		break;
	case InteractionType::DIALOGUE: 
		Dialogue(); 
		break;
	default:
		break;
	}
}

void InteractableItem::Pickup()
{
	LOG("'%s' picked up", name.c_str());
}

void InteractableItem::Toggle()
{
	LOG("'%s' toggled", name.c_str());
}

void InteractableItem::Dialogue()
{
}
