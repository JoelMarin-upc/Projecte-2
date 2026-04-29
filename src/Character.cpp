#include "Character.h"

float Character::Attack()
{
    Stat& attack = stats->GetStat("attack");
    return attack.getValue();
}

std::shared_ptr<Consumable> Character::UseConsumable(std::string type)
{
    std::shared_ptr<Consumable> copy = nullptr;
    std::shared_ptr<InteractableItem> item = inventory->GetItem(type);
    if (std::shared_ptr<Consumable> c = std::dynamic_pointer_cast<Consumable>(item)) copy = std::make_shared<Consumable>(*c);
    inventory->RemoveItem(type);
    return copy;
}

void Character::TakeConsumable(std::shared_ptr<Consumable> consumable)
{
    for (Stat stat : consumable->stats->stats) {
        if (stat.name == "health") stats->AddToStat(stat.name, stat.value);
        else stats->ApplyModifier(stat.name, stat.value, stat.modifierTurnsLeft);
    }
}

void Character::TakeStance(Stance stance)
{
    switch (stance)
    {
    case REST:
        // +1 action next turn
        break;
    case DEFEND:
        stats->ApplyModifier("defense", 2.f, 1);
        break;
    case CONCENTRATE:
        stats->ApplyModifier("attack", 1.8f, 1);
        break;
    case ASSIST:
        // *1.2 damage rest of the team
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
    if (godMode) return false;
    Stat& defense = stats->GetStat("defense");
	damage -= defense.getValue();
    if (damage < 0) damage = 0;
    Stat& health = stats->GetStat("health");
    int hp = health.getValue() - damage;
    if (hp < 0) hp = 0;
	health.setValue(hp);
    if (hp <= 0) return true;
    return false;
}

void Character::CheckModifiers()
{
    for (Stat& stat : stats->stats)
    {
        if (stat.modifierTurnsLeft < 1)
        {
            stat.modifier = 1.f;
            continue;
        }
        stat.modifierTurnsLeft--;
    }
}

void Character::DrawHealthBar(SDL_Texture* texture)
{
    int healthW = texture->w * stats->GetStat("health").getValue() / stats->GetStat("health").maxValue;
    int x = position.getX() - texture->w / 2;
    int y = position.getY() - texture->h / 2;
    Engine::GetInstance().render->DrawRectangle({ x, y - 15, texture->w, 10 }, 255, 0, 0);
    Engine::GetInstance().render->DrawRectangle({ x, y - 15, healthW, 10 }, 0, 255, 0);
}