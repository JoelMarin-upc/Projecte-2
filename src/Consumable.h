#pragma once

#include "Equipable.h"

class Consumable : public Equipable {
public:
	Consumable() {}
	Consumable(std::string id, std::string name, std::string description, std::string texturePath, bool canStack);
	~Consumable() {}
	void OnConsume();
};