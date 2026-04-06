#include "Lever.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Lever::Lever() : InteractableItem()
{
	name = "Lever";
}

Lever::~Lever() {}

bool Lever::Awake() {
	return true;
}

bool Lever::Start() {

	pickupIconPath = "Assets/Textures/icon.png";
	openLeverTex = Engine::GetInstance().textures->Load("Assets/Textures/dungeonLeverOpened.png");
	closeLeverTex = Engine::GetInstance().textures->Load("Assets/Textures/dungeonLeverClosed.png");
	texture = closeLeverTex;
	//texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	pickupIcon = Engine::GetInstance().textures->Load(pickupIconPath);
	AddCollider(ColliderType::CIRCLE, texture, 0, 0, -20, -20, 1, 1);
	texW = 64;
	texH = 32;

	pbody = colliders[0];
	colliders[0]->etype = EntityType::INTERACTABLE_ITEM;
	pbody->listener = this;
	//pbody->listener->type = EntityType::INTERACTABLE_ITEM;

	return true;
}

bool Lever::Update(float dt)
{
	if (!active) {
		return true;
	}
	if (isPlayerInRange && !isOpened) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			Interact();
		}
	}
	Draw(dt);
	return true;
}

void Lever::Draw(float dt) {
	if (!active) {
		return;
	}

	int x, y;
	pbody->GetPosition(x, y);
	Engine::GetInstance().render->DrawTexture(texture, x - texW, y - texH);

	//To draw pick up icon next to the item sprite, adjustable later
	if (isPlayerInRange) {
		Engine::GetInstance().render->DrawTexture(pickupIcon, x - pickupIconW, y - pickupIconH);
	}
}

bool Lever::CleanUp() {
	LOG("Cleanup Interactable Item");
	Engine::GetInstance().textures->UnLoad(openLeverTex);
	Engine::GetInstance().textures->UnLoad(closeLeverTex);
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

void Lever::OnCollision(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER:
		isPlayerInRange = true;
		break;
	}
}

void Lever::OnCollisionEnd(Collider* physA, Collider* physB)
{
	switch (physB->etype) {
	case EntityType::PLAYER:
		isPlayerInRange = false;
		break;
	default:
		break;
	}
}

void Lever::Interact()
{
	isOpened = !isOpened;

	if (isOpened) {
		texture = openLeverTex;
	}
	else {
		texture = closeLeverTex;
	}
}