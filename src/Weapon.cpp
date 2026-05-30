#include "Weapon.h"

Weapon::Weapon(std::string id, std::string name, std::string description, std::string texturePath, bool canStack) : Equipable(id, name, description, texturePath, ItemInteractionType::PICKUP, canStack)
{
}

void Weapon::OnEquip()
{
	//What happens when equipping the weapon
}

void Weapon::OnUnequip()
{
	//What happens when unequipping the weapon
}
