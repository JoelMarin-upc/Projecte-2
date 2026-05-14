#include "ResetButton.h"
#include "PushBox.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Log.h"

bool ResetButton::Start()
{
    pickupIconPath = "Assets/Textures/item.png";
    texture = Engine::GetInstance().textures->Load(texturePath.c_str());
    if (toggledTexturePath != "") toggledTexture = Engine::GetInstance().textures->Load(toggledTexturePath.c_str());
    icon = texture;
    pickupIcon = Engine::GetInstance().textures->Load(pickupIconPath);

    AddCollider(ColliderType::CIRCLE_SENSOR, texture, 0, 0, 50, 50, 1, 1);
    texW = 16;
    texH = 16;

    pbody = colliders[0];
    pbody->etype = EntityType::INTERACTABLE_ITEM;
    pbody->listener = this;

    sensorCollider = colliders[0];

    return true;
}

bool ResetButton::Update(float dt)
{
    if (!active) return true;

    if (isToggled) {
        pressedTimer += dt;
        if (pressedTimer >= pressDuration) {
            isToggled = false;
            pressedTimer = 0.0f;
        }
    }

    if (isPlayerInRange) {
        if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            Interact();
        }
    }

    return true;
}

void ResetButton::Interact()
{
    isToggled = !isToggled;
    if (!linkedBox) {
        LOG("ResetButton '%s': no linked PushBox!", id.c_str());
        return;
    }

    linkedBox->ResetToSpawn();
    LOG("ResetButton '%s': box reset triggered.", id.c_str());
}