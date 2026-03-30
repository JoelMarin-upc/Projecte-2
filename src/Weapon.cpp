#include "Weapon.h"

Weapon::Weapon(std::string id, std::string name, std::string texturePath) : Equipable(id, name, texturePath, ItemInteractionType::PICKUP)
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
