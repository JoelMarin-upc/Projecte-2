#include "Inventory.h"
#include "Log.h"

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

bool Inventory::RemoveItem(InteractableItem* item)
{
	if (item->canStack) {
		int existingStack = FindItem(item->id);
		if (existingStack != -1) {
			items[existingStack]->count -= item->count;
			if (item->count <= 0) {
				for (int i = 0; i < items.size(); ++i) {
					if (items[i]->name == item->name) {
						items[i]->Destroy();
						items.erase(items.begin() + i);
					}
				}
			}
		}
		else {
			LOG("This item isn't in the inventory");
		}
	}

	else {
		int itemInInventory = FindItem(item->name);
		for (int i = 0; i < items.size(); ++i) {
			if (items[i]->name == item->name) {
				items[i]->Destroy();
				items.erase(items.begin() + i);
			}
		}
	}

	return true;
}

int Inventory::FindItem(std::string itemName)
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
		if (items[i] == nullptr) {
			return i;
		}
	}
	return -1;
}
