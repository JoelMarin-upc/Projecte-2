#include "PressurePlate.h"
#include "EntityManager.h"
#include "DungeonGate.h"
#include "Log.h"

bool PressurePlate::Start()
{
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());
    icon = texture;

    AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, -20, -20, 1, 1);
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
    if (physB->etype != EntityType::PUSH_BOX) return;
    if (isPressed) return;

    isPressed = true;
    if (linkedGate) linkedGate->Unlock();
    
}

void PressurePlate::OnCollisionEnd(Collider* physA, Collider* physB)
{
    if (physB->etype != EntityType::PUSH_BOX) return;

    if (linkedGate && linkedGate->active) {
        isPressed = false;
    }
}
