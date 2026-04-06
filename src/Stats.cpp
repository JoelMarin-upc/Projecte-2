#include "Stats.h"

Stats::Stats()
{
    stats = std::vector<Stat*>();
}

Stats::~Stats()
{
}

void Stats::AddStat(std::string statname, float value, float maxValue)
{
    stats.push_back(new Stat(statname, value, maxValue));
}

void Stats::RemoveStat(std::string statname)
{
	for (int i = 0; i < stats.size(); i++)
	{
		if (stats[i]->name == statname)
		{
			delete stats[i];
			stats.erase(stats.begin() + i);
		}
	}
}

Stat* Stats::GetStat(std::string statname)
{
    for (const auto& s : stats) if (s->name == statname) return s;
    return nullptr;
}

void Stats::ApplyModifier(std::string statname, float multiplier, int turns)
{
    Stat* s = GetStat(statname);
    s->modifier = multiplier;
}
