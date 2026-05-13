#pragma once
#include "InteractableItem.h"

class DungeonGate;

class PressurePlate : public InteractableItem {
public:
    PressurePlate(std::string id, std::string name, std::string description,std::string texturePath)
        : InteractableItem(id, name, description, texturePath,ItemInteractionType::DEFAULT, false) {
    }

    bool Start() override;
    void Draw(float dt) override;

    void OnCollision(Collider* physA, Collider* physB) override;

    DungeonGate* linkedGate = nullptr;

private:
    bool isPressed = false;
};