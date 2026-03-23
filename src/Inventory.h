#pragma once

#include <iostream>
#include "Item.h"
#include "InteractableItem.h"
#include "Gear.h"
#include "Weapon.h"

const int MAX_SLOTS = 12;

class Inventory {
	Inventory();
	~Inventory(); 

	bool Awake();
	bool Start();
	bool Update();
	bool Cleanup();

	bool AddItem(InteractableItem* item);
	int FindItemStack(std::string id);
	int FindFreeSlot();

public:
	std::vector<InteractableItem*> items;
	Gear armor;
	Weapon weapon;
};