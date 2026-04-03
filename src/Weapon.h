#pragma once

#include "Equipable.h"

class Weapon : public Equipable {
public:
	Weapon() {}
	Weapon(std::string id, std::string name, std::string texturePath);
	~Weapon() {}
	void OnEquip() override;
	void OnUnequip() override;
};