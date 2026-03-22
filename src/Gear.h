#pragma once

#include "Equipable.h"

class Gear : Equipable {
	Gear() {}
	Gear(std::string id, std::string name, std::string texturePath);
	~Gear() {}
	void OnEquip() override;
	void OnUnequip() override;
};