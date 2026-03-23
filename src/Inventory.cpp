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
	if (item->canStack) {
		int existingStack = FindItemStack(item->id);
		if (existingStack != -1) {
			items[existingStack]->count += item->count;
		}
	}

	else {
		int firstFreeSlot = FindFreeSlot();
		if (firstFreeSlot != -1) {
			items[firstFreeSlot] = item;
			LOG("Added %s to inventory space %d", item->name.c_str(), firstFreeSlot);
		}
		else {
			LOG("Inventory full");
			return false;
		}
	}
	return true;
}

int Inventory::FindItemStack(std::string id)
{
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->id == id) {
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
