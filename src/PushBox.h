#pragma once
#include "InteractableItem.h"

class PushBox : public InteractableItem {
public:
    PushBox(std::string id, std::string name, std::string description, std::string texturePath)
        : InteractableItem(id, name, description, texturePath, ItemInteractionType::DEFAULT, false) {
    }

    bool Start() override;
    bool Update(float dt) override;
    void Draw(float dt) override;
    bool CleanUp() override;


    void ResetToSpawn();


    Vector2D spawnPosition;

private:

    void Interact() override {}
};