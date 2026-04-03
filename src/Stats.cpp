#include "Stats.h"

Stat Stats::GetStat(std::string statname)
{
    for (const auto& s : stats) {
        if (s.name == statname) {
            return s;
        }
    }
}

void Stats::ApplyModifierMultiplier(std::string statname, float multiplier)
{
    Stat s = GetStat(statname);
    s.value *= multiplier;
}

void Stats::ApplyModifierAddition(std::string statname, float addition)
{
    Stat s = GetStat(statname);
    s.value += addition;
}
