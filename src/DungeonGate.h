#pragma once
#include "InteractableItem.h"

class DungeonGate : public InteractableItem {
public:
    DungeonGate(std::string id, std::string name, std::string description, std::string texturePath)
        : InteractableItem(id, name, description, texturePath, ItemInteractionType::DIALOGUE, false) {
        isLocked = true;
    }
    bool Start() override;
    void Unlock();

protected:
    void Interact() override;

public:
    bool isLocked = true;
};