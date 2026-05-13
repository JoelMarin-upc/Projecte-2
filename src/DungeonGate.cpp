#include "DungeonGate.h"
#include "Engine.h"
#include "Physics.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Log.h"

bool DungeonGate::Start()
{
	pickupIconPath = "Assets/Textures/item.png";
	//if (toggledTexturePath != "") toggledTexture = Engine::GetInstance().textures->Load(toggledTexturePath.c_str());
	texture = Engine::GetInstance().textures->Load(texturePath.c_str());
	icon = texture;
	pickupIcon = Engine::GetInstance().textures->Load(pickupIconPath);
	AddCollider(ColliderType::SQUARE, texture, 0, 0, 0, 0, 1, 1);
	AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 50, 50, 1, 1);
	texW = 32;
	texH = 64;

	pbody = colliders[0];
	colliders[0]->etype = EntityType::INTERACTABLE_ITEM;
	pbody->listener = this;
	//pbody->listener->type = EntityType::INTERACTABLE_ITEM;

	sensorCollider = colliders[1];
	colliders[1]->etype = EntityType::INTERACTABLE_ITEM;
	sensorCollider->listener = this;
	//sensorCollider->listener->type = EntityType::INTERACTABLE_ITEM;

	b2Body_SetFixedRotation(pbody->body, true);

	b2MassData massData;
	massData.mass = 1000.0f;
	massData.center = { 0.0f, 0.0f };
	massData.rotationalInertia = 0.0f;
	b2Body_SetMassData(pbody->body, massData);

	return true;
}

void DungeonGate::Unlock()
{
	isToggled = true;
	active = false;

	if (pbody) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	if (sensorCollider) {
		Engine::GetInstance().physics->DeletePhysBody(sensorCollider);
		sensorCollider = nullptr;
	}
}

void DungeonGate::Interact()
{
	if (!isToggled) {
		Engine::GetInstance().sceneManager->currentScene->StartDialog("exit_locked");
	}
}