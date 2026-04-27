#pragma once

#include <iostream>
#include "Item.h"
#include "InteractableItem.h"
#include "Gear.h"
#include "Weapon.h"

const int MAX_SLOTS = 12;

class Inventory {
public:
	Inventory();
	~Inventory(); 

	bool Awake();
	bool Start();
	bool Update();
	bool Cleanup();

	bool AddItem(InteractableItem* item);
	bool RemoveItem(std::string& itemName);
	bool HasItem(std::string& itemName);

	bool EquipWeapon(std::string& itemName);
	bool EquipGear(std::string& itemName);
	bool UnequipGear(GearSlot slot);
	bool UnequipWeapon();

	Gear* GetGearSlot(GearSlot slot);
	int FindItem(std::string& itemName);
	bool IsFull();
	void PrintContents();

	void AddGold(int amount);

public:
	std::vector<InteractableItem*> items;
	Gear* equippedHelmet = nullptr;
	Gear* equippedBody = nullptr;
	Gear* equippedBoots = nullptr;
	Weapon* equippedWeapon = nullptr;

	int gold = 0;
};