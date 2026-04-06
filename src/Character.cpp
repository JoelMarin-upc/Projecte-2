#include "Character.h"

float Character::Attack()
{
    Stat* attack = stats->GetStat("attack");
    return attack->getValue();
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
    switch (stance)
    {
    case REST:
        // +1 action next turn
        break;
    case DEFEND:
        stats->ApplyModifier("defense", 1.5f, 0);
        break;
    case CONCENTRATE:
        stats->ApplyModifier("attack", 2.f, 1);
        break;
    case ASSIST:
        // *1.3 damage rest of the team
        break;
    case NO_STANCE:
        break;
    default:
        break;
    }
}

// returns true if the character is dead
bool Character::TakeDamage(float damage)
{
    Stat* defense = stats->GetStat("defense");
	damage -= defense->getValue();
    Stat* health = stats->GetStat("health");
    int hp = health->getValue() - damage;
    if (hp < 0) hp = 0;
	health->setValue(hp);
    if (hp <= 0) return true;
    return false;
}

void Character::CheckModifiers()
{
    for (Stat* stat : stats->stats)
    {
        if (stat->modifierTurnsLeft == -1)
        {
            stat->modifier = 1.f;
            continue;
        }
        stat->modifierTurnsLeft--;
    }
}
