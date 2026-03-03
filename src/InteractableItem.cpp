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

InteractableItem::InteractableItem() : Entity(EntityType::ITEM)
{
	name = "interactableItem";
}

InteractableItem::~InteractableItem() {}

bool InteractableItem::Awake() {
	return true;
}

bool InteractableItem::Start() {
	pickupTexture = Engine::GetInstance().textures->Load("Assets/Textures/item.png");
	pickupTexW = 16;
	pickupTexH = 16;
	Engine::GetInstance().textures.get()->GetSize(pickupTexture, texW, texH);
}

bool InteractableItem::Update(float dt) {

}

void InteractableItem::Draw(float dt) {

}

bool InteractableItem::CleanUp() {

}

bool InteractableItem::Destroy() {
	LOG("Destroying interactable");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}

void InteractableItem::Interact()
{

}
