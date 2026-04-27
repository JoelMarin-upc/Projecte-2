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
	if (IsFull()) {
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
		return true;
	}

	items.push_back(item);
	return true;
}

bool Inventory::RemoveItem(std::string& itemName)
{
	int i = FindItem(itemName);
	if (i == -1) {
		LOG("%s not found in inventory", itemName.c_str());
		return false;
	}

	InteractableItem* item = items[i];

	if (item->canStack && item->count > 1) {
		item->count--;
		LOG("Removed one %s from the stack, now %d", itemName.c_str(), item->count);
	}
	else {
		items[i]->Destroy();
		items.erase(items.begin() + i);
		LOG("Removed %s from inventory", itemName.c_str());
	}

	return true;
}

bool Inventory::HasItem(std::string& itemName)
{
	//Find the item in the inventory
	if (FindItem(itemName) != -1) {
		return true;
	}
	return false;
}

bool Inventory::EquipWeapon(std::string& itemName)
{
	int i = FindItem(itemName);
	if (i == -1) {
		LOG("%s not in inventory", itemName.c_str());
		return false;
	}

	//Make sure that the item is of type Weapon
	Weapon* w = dynamic_cast<Weapon*>(items[i]);
	if (!w)	{
		LOG("%s is not a Weapon", itemName.c_str());
		return false;
	}

	//If there's weapon equipped in that slot
	if (equippedWeapon)	{
		//Remove new weapon from inventory
		items.erase(items.begin() + i);

		//Check if inventory is full, fail to swap weapon if full
		if (IsFull())	{
			items.insert(items.begin() + i, w);
			LOG("No free slot for previously equipped weapon %s", equippedWeapon->name.c_str());
			return false;
		}
		//Unequip old weapon and put back in the inventory
		equippedWeapon->OnUnequip();
		items.push_back(equippedWeapon);
		LOG("Unequipped %s back to inventory", equippedWeapon->name.c_str());
	}
	//If the weapon slot was empty
	else {
		items.erase(items.begin() + i);
	}

	//Equip the new gear
	equippedWeapon = w;
	equippedWeapon->OnEquip();
	LOG("Equipped weapon: '%s'", w->name.c_str());
	return true;
}

bool Inventory::EquipGear(std::string& itemName)
{
	//Find the item in the inventory
	int i = FindItem(itemName);
	if (i == -1) {
		LOG("%s isnt' in inventory", itemName.c_str());
		return false;
	}

	//Make sure that the item is of type Gear
	Gear* g = dynamic_cast<Gear*>(items[i]);
	if (!g) {
		LOG("%s isn't a Gear item", itemName.c_str());
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
			LOG("no free slot available");
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

	if (!gear) {
		LOG("The slot is already empty");
		return true;
	}

	if (IsFull()) {
		LOG("Inventory full, cannot unequip %s", gear->name.c_str());
		return false;
	}

	//Remove gear effects and add back to inventory
	gear->OnUnequip();
	items.push_back(gear);
	LOG("Unequipped gear %s to inventory", gear->name.c_str());
	gear = nullptr;
	return true;
}

bool Inventory::UnequipWeapon()
{
	//Get weapon from slot
	if (!equippedWeapon) {
		LOG("No weapon equipped");
		return true;
	}

	if (IsFull()) {
		LOG("Inventory full, cannot unequip %s", equippedWeapon->name.c_str());
		return false;
	}

	//Remove weapon effects and add back to inventory
	equippedWeapon->OnUnequip();
	items.push_back(equippedWeapon);
	LOG("Unequipped weapon %s to inventory", equippedWeapon->id.c_str());
	equippedWeapon = nullptr;
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

bool Inventory::IsFull()
{
	if ((int)items.size() >= MAX_SLOTS) {
		return true;
	}
	return false;
}
void Inventory::PrintContents()
{
	LOG("=== INVENTORY (%d/%d) ===", items.size(), MAX_SLOTS);
	for (int i = 0; i < items.size(); ++i) {
		LOG("%d. %s (x%d)", i, items[i]->name.c_str(), items[i]->count);
	}	
	if (equippedWeapon) {
		LOG("Weapon: %s", equippedWeapon->name.c_str());
	}	
	if (equippedHelmet) {
		LOG("Helmet: %s", equippedHelmet->name.c_str());
	}	
	if (equippedBody) {
		LOG("Body: %s", equippedBody->name.c_str());
	}
	if (equippedBoots) {
		LOG("Boots: %s", equippedBoots->name.c_str());
	}
}

void Inventory::AddGold(int amount)
{
	gold += amount;
	if (gold < 0) gold = 0;
}

