#include "DungeonLever.h"
#include "Engine.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Audio.h"

bool DungeonLever::Start()
{
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

	std::string leverFxPath = Engine::GetInstance().audio->GetAudioPath("puzzle", "lever");
	leverFxId = Engine::GetInstance().audio->LoadFx(leverFxPath.c_str());

	return true;
}

void DungeonLever::Toggle() {
    if (isToggled) return;

    isToggled = true;
	if (leverFxId != -1) Engine::GetInstance().audio->PlayFx(leverFxId);
    Engine::GetInstance().sceneManager->currentScene->OnLeverToggled();
}