#pragma once

#include "InteractableItem.h"
#include "Stats.h"

class Equipable : InteractableItem
{
public:
	Equipable() : InteractableItem() {}
	~Equipable() {}

	Stats stats;

private:

};

