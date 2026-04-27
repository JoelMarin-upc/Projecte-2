#pragma once

#include "Equipable.h"

enum class GearSlot
{
	HELMET,
	BODY,
	BOOTS
};

class Gear : public Equipable {
public:
	Gear() {}
	Gear(std::string id, std::string name, std::string description, std::string texturePath, GearSlot slot, bool canStack);
	~Gear() {}
	void OnEquip() override;
	void OnUnequip() override;

public:
	GearSlot gearSlot = GearSlot::BODY;
};