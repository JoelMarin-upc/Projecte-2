#include "Gear.h"

Gear::Gear(std::string id, std::string name, std::string texturePath) : Equipable(id, name, texturePath, InteractionType::PICKUP)
{
}

void Gear::OnEquip()
{
}

void Gear::OnUnequip()
{
}
