#pragma once

#include "Mission.h"

class ReachMission : public Mission {
public:
	inline std::string ToString() override
	{
		if (completed) return "Reached " + targetName + ". " + ToStringReward();
		else return "Reach " + targetName + ". " + ToStringReward();
	};
private:

};