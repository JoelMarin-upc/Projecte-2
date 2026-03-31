#include "Inventory.h"
#include "Log.h"
#include "Equipable.h"

Inventory::Inventory() {

}

Inventory::~Inventory()
{
}

bool Inventory::Awake() {
	return true;
}

bool Inventory::Start() {
	return true;
}

bool Inventory::Update() {
	return true;
}

bool Inventory::Cleanup()
{
	return true;
}

bool Inventory::AddItem(InteractableItem* item)
{
	if (items.size() == MAX_SLOTS) {
		LOG("The inventory is full, the item could not be added");
		return false;
	}

	if (item->canStack) {
		int existingStack = FindItem(item->name);
		if (existingStack != -1) {
			items[existingStack]->count += item->count;
		}
		else {
			items.push_back(item);
		}
	}

	else {
		items.push_back(item);

		/*int firstFreeSlot = FindFreeSlot();
		if (firstFreeSlot != -1) {
			items[firstFreeSlot] = item;
			LOG("Added %s to inventory space %d", item->name.c_str(), firstFreeSlot);
		}
		else {
			LOG("Inventory full");
			return false;
		}*/
	}
	return true;
}

bool Inventory::RemoveItem(std::string& itemName)
{
	int i = FindItem(itemName);
	if (i == -1)
	{
		LOG("RemoveItem: %s not found in inventory", itemName.c_str());
		return false;
	}

	InteractableItem* item = items[i];

	if (item->canStack && item->count > 1) {
		item->count--;
		LOG("RemoveItem: Removed one %s from the stack, now %d", itemName.c_str(), item->count);
	}
	else {
		items[i]->Destroy();
		items.erase(items.begin() + i);
		LOG("RemoveItem: Removed %s from inventory", itemName.c_str());
	}

	return true;
}

bool Inventory::HasItem(std::string& itemName)
{
	return false;
}

bool Inventory::EquipWeapon(std::string& itemName)
{
	return false;
}

bool Inventory::EquipGear(std::string& itemName)
{
	//Find the item in the inventory
	int i = FindItem(itemName);
	if (i == -1) {
		LOG("EquipGear: %s isnt' in inventory", itemName.c_str());
		return false;
	}

	//Make sure that the item is of type Gear
	Gear* g = dynamic_cast<Gear*>(items[i]);
	if (!g) {
		LOG("EquipGear: %s isn't a Gear item", itemName.c_str());
		return false;
	}

	//Get the currently equipped gear in that slot (nullptr if not equipped anything)
	Gear* slot = GetGearSlot(g->gearSlot);

	//If there's gear equipped in that slot
	if (slot) {
		//Remove new gear from inventory
		items.erase(items.begin() + i);

		//Check if inventory is full, fail to swap gear if full
		if (IsFull()) {
			items.insert(items.begin() + i, g);
			LOG("EquipGear: no free slot available");
			return false;
		}

		//Unequip old gear and put back in the inventory
		slot->OnUnequip();
		items.push_back(slot);
		LOG("Unequipped '%s' back to inventory", slot->id.c_str());
	}

	//If the gear slot was empty
	else {
		items.erase(items.begin() + i);
	}

	//Equip the new gear
	slot = g;
	slot->OnEquip();
	return true;
}

bool Inventory::UnequipGear(GearSlot slot)
{
	//Get gear from slot
	Gear* gear = GetGearSlot(slot);

	if (!gear)
	{
		LOG("UnequipGear: the slot is already empty");
		return true;
	}

	if (IsFull())
	{
		LOG("UnequipGear: inventory full, cannot unequip %s", gear->name.c_str());
		return false;
	}

	//Remove gear effects and add back to inventory
	gear->OnUnequip();
	items.push_back(gear);
	LOG("Unequipped gear %s to inventory", gear->name.c_str());
	gear = nullptr;
	return true;
}

Gear* Inventory::GetGearSlot(GearSlot slot)
{
	switch (slot) {
	case GearSlot::HELMET:
		return equippedHelmet;
		break;
	case GearSlot::BODY:
		return equippedBody;
		break;
	case GearSlot::BOOTS:
		return equippedBoots;
		break;
	default:
		break;
	}
}

int Inventory::FindItem(std::string& itemName)
{
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->name == itemName) {
			return i;
		}
	}
	return -1;
}

int Inventory::FindFreeSlot()
{
	for (int i = 0; i < items.size(); ++i) {
		if (!items[i]) {
			return i;
		}
	}
	return -1;
}

bool Inventory::IsFull()
{
	if (items.size() >= MAX_SLOTS) {
		return true;
	}
	return false;
}	
