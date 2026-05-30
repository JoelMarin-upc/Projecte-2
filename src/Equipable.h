#pragma once

#include "InteractableItem.h"

class Equipable : public InteractableItem
{
public:
	Equipable() : InteractableItem() {}
	Equipable(std::string id, std::string name, std::string description, std::string texturePath, ItemInteractionType type = ItemInteractionType::PICKUP, bool canStack = false) : InteractableItem(id, name, description, texturePath, type, canStack){}
	~Equipable() {}

	virtual void OnEquip() {
		isEquipped = true;
	}

	virtual void OnUnequip() {
		isEquipped = false;
	}

public:
	bool isEquipped = false;
};

