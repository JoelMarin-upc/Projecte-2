#pragma once

#include "InteractableItem.h"
#include "Stats.h"

class Equipable : InteractableItem
{
public:
	Equipable() : InteractableItem() {}
	Equipable(std::string id, std::string name, std::string texturePath, InteractionType type = InteractionType::PICKUP) : InteractableItem(id, name, texturePath, type){}
	~Equipable() {}

	virtual void OnEquip() {
		isEquipped = true;
	}

	virtual void OnUnequip() {
		isEquipped = false;
	}

public:
	Stats stats;
	bool isEquipped = false;
};

