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

	bool AddItem(std::shared_ptr<InteractableItem> item);
	bool RemoveItem(std::string& itemName);
	bool HasItem(std::string& itemName);

	bool EquipWeapon(std::string& itemName);
	bool EquipGear(std::string& itemName);
	bool UnequipGear(GearSlot slot);
	bool UnequipWeapon();

	std::shared_ptr<Gear> GetGearSlot(GearSlot slot);
	int FindItem(std::string& itemName);
	bool IsFull();
	void PrintContents();

	void AddGold(int amount);

public:
	std::vector<std::shared_ptr<InteractableItem>> items;
	std::shared_ptr<Gear> equippedHelmet = nullptr;
	std::shared_ptr<Gear> equippedBody = nullptr;
	std::shared_ptr<Gear> equippedBoots = nullptr;
	std::shared_ptr<Weapon> equippedWeapon = nullptr;

	int gold = 0;
};