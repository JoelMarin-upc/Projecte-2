#include "Character.h"
#include "Scene.h"

float Character::Attack()
{
    Stat& attack = stats->GetStat("attack");
    float weaponModifier = 0;
    if (inventory->equippedWeapon) weaponModifier = inventory->equippedWeapon->stats->GetStat("attack").value;
    
    float attackValue = attack.getValue(weaponModifier);
    
    float infectionModifier = 1.f;
    Stat& infection = stats->GetStat("infection");
    if (infection.value >= INFECTION_THRESHOLD_1) infectionModifier = 1.2f;
    if (infection.value >= INFECTION_THRESHOLD_2) infectionModifier = 1.5f;
    if (infection.value >= INFECTION_THRESHOLD_3) infectionModifier = 2.f;
    
    return attackValue * infectionModifier;
}

float Character::Defense()
{
    Stat& defense = stats->GetStat("defense");
    float gearModifier = 0;
    if (inventory->equippedHelmet) gearModifier += inventory->equippedHelmet->stats->GetStat("defense").value;
    if (inventory->equippedBody) gearModifier += inventory->equippedBody->stats->GetStat("defense").value;
    if (inventory->equippedBoots) gearModifier += inventory->equippedBoots->stats->GetStat("defense").value;
    return defense.getValue(gearModifier);
}

float Character::Speed()
{
    Stat& speed = stats->GetStat("speed");
    float gearModifier = 0;
    if (inventory->equippedHelmet) gearModifier += inventory->equippedHelmet->stats->GetStat("speed").value;
    if (inventory->equippedBody) gearModifier += inventory->equippedBody->stats->GetStat("speed").value;
    if (inventory->equippedBoots) gearModifier += inventory->equippedBoots->stats->GetStat("speed").value;
    return speed.getValue(gearModifier);
}

float Character::HP()
{
    Stat& health = stats->GetStat("health");
    return health.getValue();
}

float Character::MaxHP()
{
    Stat& health = stats->GetStat("health");
    return health.maxValue;
}

float Character::Infection()
{
    Stat& infection = stats->GetStat("infection");
    return infection.getValue();
}

float Character::MaxInfection()
{
    Stat& infection = stats->GetStat("infection");
    return infection.maxValue;
}

bool Character::IsInfected()
{
    Stat& infection = stats->GetStat("infection");
    return infection.value >= INFECTION_THRESHOLD_1;
}

bool Character::IsFullyInfected()
{
    Stat& infection = stats->GetStat("infection");
    return infection.value >= infection.maxValue;
}

bool Character::ChangeInfection(int amount)
{
    Stat& infection = stats->GetStat("infection");
    int newValue = infection.value + amount;
    if (newValue < 0) newValue = 0;
    stats->SetStat("infection", newValue);
    return infection.value >= infection.maxValue;
}

std::shared_ptr<Consumable> Character::UseConsumable(std::string type)
{
    if (type == "") return nullptr;
    std::shared_ptr<Consumable> copy = nullptr;
    std::shared_ptr<InteractableItem> item = inventory->GetItem(type);
    if (std::shared_ptr<Consumable> c = std::dynamic_pointer_cast<Consumable>(item)) copy = std::make_shared<Consumable>(*c);
    inventory->RemoveItem(type);
    return copy;
}

void Character::TakeConsumable(std::shared_ptr<Consumable> consumable)
{
    if (!consumable) return;
    for (Stat stat : consumable->stats->stats) {
        if (stat.name == "health" || stat.name == "infection" || stat.name == "cure") stats->AddToStat(stat.name, stat.value);
        else stats->ApplyModifier(stat.name, stat.value, stat.modifierTurnsLeft);
    }
}

void Character::TakeStance(Stance stance, std::vector<std::shared_ptr<Character>> affectedCharacters)
{
    switch (stance)
    {
    case REST:
        // +15% chance to get another turn (handled on Combat.cpp)
        break;
    case DEFEND:
        stats->ApplyModifier("defense", 2.f, 1);
        break;
    case CONCENTRATE:
        stats->ApplyModifier("attack", 1.8f, 1);
        break;
    case ASSIST:
        for (std::shared_ptr<Character> member : affectedCharacters) 
            member->stats->ApplyModifier("attack", 1.2f, 1);
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
    float gearModifier = 0;
    if (inventory->equippedHelmet) gearModifier += inventory->equippedHelmet->stats->GetStat("defense").value;
    if (inventory->equippedBody) gearModifier += inventory->equippedBody->stats->GetStat("defense").value;
    if (inventory->equippedBoots) gearModifier += inventory->equippedBoots->stats->GetStat("defense").value;
    Stat& defense = stats->GetStat("defense");
	damage -= defense.getValue(gearModifier);
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

void Character::DrawHealthBar(const SDL_Rect& rect)
{
    int healthW = rect.w * stats->GetStat("health").getValue() / stats->GetStat("health").maxValue;
    int hx = position.getX() - rect.w / 2;
    int hy = position.getY() - rect.h / 2;
    Engine::GetInstance().render->DrawRectangle({ hx, hy - 15, rect.w, 5 }, 255, 0, 0);
    Engine::GetInstance().render->DrawRectangle({ hx, hy - 15, healthW, 5 }, 0, 255, 0);

    int infectionW = rect.w * stats->GetStat("infection").getValue() / stats->GetStat("infection").maxValue;
    int ix = position.getX() - rect.w / 2;
    int iy = position.getY() - rect.h / 2;
    //Engine::GetInstance().render->DrawRectangle({ ix, iy - 25, rect.w, 5 }, 255, 0, 0);
    Engine::GetInstance().render->DrawRectangle({ ix, iy - 20, infectionW, 5 }, 123, 37, 161);
}

void Character::DrawCombatHealthBar(float worldX, float worldY, int width)
{
    int healthW = width * stats->GetStat("health").getValue() / stats->GetStat("health").maxValue;
    int hx = (int)worldX;
    int hy = (int)worldY + 110;
    Engine::GetInstance().render->DrawRectangle({ hx, hy, width, 10 }, 255, 0, 0);
    Engine::GetInstance().render->DrawRectangle({ hx, hy, healthW, 10 }, 0, 255, 0);

    if (stats->GetStat("infection").getValue() <= 0) return;

    int infectionW = width * stats->GetStat("infection").getValue() / stats->GetStat("infection").maxValue;
    int ix = (int)worldX;
    int iy = (int)worldY + 100;
    //Engine::GetInstance().render->DrawRectangle({ ix, iy, width, 10 }, 255, 0, 0);
    Engine::GetInstance().render->DrawRectangle({ ix, iy, infectionW, 10 }, 255, 0, 255);
}

void Character::DrawHealthBar(SDL_Texture* texture) { DrawHealthBar({0, 0, texture->w, texture->h }); }