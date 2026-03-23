#pragma once

#include <iostream>
#include "Item.h"
#include "Gear.h"
#include "Weapon.h"

const int MAX_SLOTS = 12;

class Inventory {
	Inventory();
	~Inventory(); 
public:
	std::vector<Item*> items;
	Gear armor;
	Weapon weapon;
};