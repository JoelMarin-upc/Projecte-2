#pragma once

#include "Item.h"
#include "Stats.h"

const float SELLING_PRICE_RATIO = 0.7f;

enum ItemInteractionType {
	DEFAULT,
	PICKUP,
	TOGGLE,
	DIALOGUE
};

struct ItemDef {
	std::string id = "";
	std::string name = "";
	std::string description = "";
	std::string texturePath = "";
	EntityType type = EntityType::UNKNOWN;
	ItemInteractionType interactionType = ItemInteractionType::DEFAULT;
	bool canStack = false;
	std::string toggledTexturePath = "";
	std::string itemClass = "";
	int slot = 0;
	int gold = 0;
	Stats* stats = nullptr;
};

class InteractableItem : public Item {
public:
	InteractableItem(){}
	InteractableItem(std::string id, std::string name, std::string description, std::string texturePath, ItemInteractionType type, bool canStack, std::string toggledTexturePath = "");

	virtual ~InteractableItem();

	virtual bool Awake() override;

	virtual bool Start();

	virtual bool Update(float dt) override;

	virtual void Draw(float dt) override;

	virtual bool CleanUp() override;

	virtual void OnCollision(Collider* physA, Collider* physB) override;

	virtual void OnCollisionEnd(Collider* physA, Collider* physB) override;

protected:
	virtual void Interact();

	virtual void Pickup();

	virtual void Toggle();

public:

	bool isPlayerInRange = false;
	bool isPicked = false;
	bool canStack = false;
	int count = 1; //Current number of this item in the inventory slot
	bool isToggled = false;
	std::string toggledTexturePath = "";
	SDL_Texture* toggledTexture = nullptr;
	ItemInteractionType itemInteractionType;
	int price = 0;
	Stats* stats = nullptr;

protected:
	//Dialogue type by default

	SDL_Texture* pickupIcon;
	const char* pickupIconPath;
	int pickupIconW = 16;
	int pickupIconH = 16;

	//L08 TODO 4: Add a physics to an item
	Collider* pbody;
	Collider* sensorCollider;
};