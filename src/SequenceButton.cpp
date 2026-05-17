#include "SequenceButton.h"
#include "Engine.h"
#include "Textures.h"
#include "Input.h"
#include "Log.h"
#include "Audio.h"

bool SequenceButton::Start()
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

    std::string buttonFxPath = Engine::GetInstance().audio->GetAudioPath("puzzle", "button");
    buttonFxId = Engine::GetInstance().audio->LoadFx(buttonFxPath.c_str());

    return true;
}

bool SequenceButton::Update(float dt)
{
    if (!active) return true;

    if (isToggled) {
        pressedTimer += dt;
        if (pressedTimer >= pressDuration) {
            isToggled = false;
            pressedTimer = 0.0f;
        }
    }

    if (isPlayerInRange && !isToggled) {
        if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            Interact();
        }
    }

    return true;
}

void SequenceButton::Interact()
{
    isToggled = true;
    pressedTimer = 0.0f;

    if (buttonFxId != -1) Engine::GetInstance().audio->PlayFx(buttonFxId);
    if (onPressed) onPressed(buttonIndex);
}