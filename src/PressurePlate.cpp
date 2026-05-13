#include "PressurePlate.h"
#include "EntityManager.h"
#include "DungeonGate.h"

bool PressurePlate::Start()
{
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());
    icon = texture;

    AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 0, 0, 1, 1);
    sensorCollider = colliders[0];
    sensorCollider->etype = EntityType::INTERACTABLE_ITEM;
    sensorCollider->listener = this;

    pbody = nullptr;
    texW = 30;
    texH = 30;

    return true;
}

void PressurePlate::Draw(float dt)
{
    if (!active) return;

    int x, y;
    sensorCollider->GetPosition(x, y);

    Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2);
}

void PressurePlate::OnCollision(Collider* physA, Collider* physB)
{
    if (isPressed) return;

    switch (physB->etype) {
    case EntityType::INTERACTABLE_ITEM:
        isPressed = true;
        if (linkedGate) linkedGate->Unlock();
        break;
    case EntityType::PLAYER:
        isPressed = true;
        if (linkedGate) linkedGate->Unlock();
        break;
    case EntityType::NPC:
        isPressed = true;
        if (linkedGate) linkedGate->Unlock();
        break;
    case EntityType::ENEMY:
        isPressed = true;
        if (linkedGate) linkedGate->Unlock();
        break;
    default:
        break;
    }

    
}
