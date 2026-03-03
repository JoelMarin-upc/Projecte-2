#pragma once

#include <iostream>
#include "Item.h"
#include "Gear.h"
#include "Weapon.h"

class Inventory {
public:
	std::list<Item*> items;
	Gear armor;
	Weapon weapon;
};