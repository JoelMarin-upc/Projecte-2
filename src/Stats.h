#include <iostream>
#include <vector>
#include "Stat.h"

class Stats {
public:
	Stats();
	~Stats();

	void AddStat(std::string statname, float value, float maxValue = 0.f);
	void RemoveStat(std::string statname);
	Stat& GetStat(std::string statname);
	void SetStat(std::string statname, int value);

	void ApplyModifier(std::string statname, float multiplier, int turns);

	std::vector<Stat> stats;
};