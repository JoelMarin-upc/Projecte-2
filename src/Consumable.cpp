#include "Consumable.h"

Consumable::Consumable(std::string id, std::string name, std::string description, std::string texturePath, bool canStack) : Equipable(id, name, description, texturePath, ItemInteractionType::PICKUP, canStack) 
{
}

void Consumable::OnConsume()
{
}