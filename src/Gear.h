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
	Gear(std::string id, std::string name, std::string texturePath, GearSlot slot) : Equipable(id, name, texturePath, ItemInteractionType::PICKUP), gearSlot(slot){}
	~Gear() {}
	void OnEquip() override;
	void OnUnequip() override;

public:
	GearSlot gearSlot = GearSlot::BODY;
};