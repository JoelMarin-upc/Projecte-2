#pragma once
#include "InteractableItem.h"
#include <functional>

class SequenceButton : public InteractableItem {
public:
    SequenceButton(std::string id, std::string name, std::string description, std::string texturePath, std::string toggledTexturePath = "", int index = 0)
        : InteractableItem(id, name, description, texturePath, ItemInteractionType::DEFAULT, false, toggledTexturePath),
        buttonIndex(index) {
    }

    bool Start() override;
    bool Update(float dt) override;

    std::function<void(int)> onPressed;

    const int buttonIndex;

private:
    void Interact() override;

    float pressedTimer = 0.0f;
    const float pressDuration = 50.0f;
};
