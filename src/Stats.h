#include <iostream>
#include <vector>
#include "Stat.h"

class Stats {
public:
	std::vector<Stat> stats;

	Stat GetStat(std::string statname);
	
	void ApplyModifierMultiplier(std::string statname, float multiplier);
	void ApplyModifierAddition(std::string statname, float addition);
};