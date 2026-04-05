#include "Character.h"

int Character::Attack()
{
    return 0;
}

Consumable* Character::UseConsumable(std::string type)
{
    return nullptr;
}

void Character::TakeConsumable(Consumable* consumable)
{
}

void Character::TakeStance(Stance stance)
{
}

// returns true if the character is dead
bool Character::TakeDamage(int damage)
{
    if (id == "player") return false;
    return true;
}
