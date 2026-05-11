#pragma once
#include "InteractableItem.h"

class Scene;

class DungeonLever : public InteractableItem {
public:
    DungeonLever(std::string id, std::string name, std::string description, std::string texturePath, std::string toggledTexturePath = "")
        : InteractableItem(id, name, description, texturePath, ItemInteractionType::TOGGLE, false, toggledTexturePath) {
    }

protected:
    void Toggle() override;
};
