#include "Gear.h"

Gear::Gear(std::string id, std::string name, std::string description, std::string texturePath, GearSlot slot, bool canStack) : Equipable(id, name, description, texturePath, ItemInteractionType::PICKUP, canStack), gearSlot(slot)
{
}

void Gear::OnEquip()
{
}

void Gear::OnUnequip()
{
}
