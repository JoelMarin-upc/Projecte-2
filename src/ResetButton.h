#pragma once
#include "InteractableItem.h"

class PushBox;

class ResetButton : public InteractableItem {
public:
    ResetButton(std::string id, std::string name, std::string description, std::string texturePath, std::string toggledTexturePath = "")
        : InteractableItem(id, name, description, texturePath, ItemInteractionType::DEFAULT, false, toggledTexturePath) {
    }

    bool Start() override;
    bool Update(float dt) override;

    PushBox* linkedBox = nullptr;

protected:
    void Interact() override;
    float pressedTimer = 0.0f;
    const float pressDuration = 50.0f;
    int buttonFxId = -1;
};
