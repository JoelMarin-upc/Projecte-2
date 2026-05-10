#pragma once

#include "Mission.h"

class KillMission : public Mission {
public:
	inline std::string ToString() override
	{
		if (completed) return "Killed " + targetName + ". " + ToStringReward();
		else return "Kill " + targetName + ". " + ToStringReward();
	};
private:

};